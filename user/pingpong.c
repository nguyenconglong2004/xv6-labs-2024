#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
int
main(int argc, char* argv[]){

    int parentSendDirection[2];
    int childSendDirection[2];
    pipe(parentSendDirection);
    pipe(childSendDirection);

    int pid = fork();
    char byteToSend = '.';
    // Lỗi khi tạo mới một tiến trình con
    if(pid < 0){
        printf("Failed to create a new process\n");
        exit(1);
    }
    // Tiến trình cha
    else if(pid > 0){
        // Đóng các đầu đọc/ghi không dùng đến
        close(childSendDirection[1]);
        close(parentSendDirection[0]);

        write(parentSendDirection[1], &byteToSend, 1);
        close(parentSendDirection[1]);
        wait(0);
        char receivedByte;
        read(childSendDirection[0], &receivedByte, 1);
        close(childSendDirection[0]);
        printf("%d: received pong\n", getpid());
        exit(0);
    }
    // Tiến trình con
    else{
        // Đóng các đầu đọc/ghi không dùng đến
        close(childSendDirection[0]);
        close(parentSendDirection[1]);
        close(parentSendDirection[0]);

        printf("%d: received ping\n", getpid());
        write(childSendDirection[1], &byteToSend, 1);
        close(childSendDirection[1]);
        exit(0);
    }

}