#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
int
main(int argc, char* argv[]){

    // Tham số không hợp lệ, chương trình sleep cần đúng 1 tham só
    if(argc != 2){
        printf("sleep: sleep <n> (n is the number of ticks)\n");
        // exit với giá trị 1 để thông báo tiến trình đã chạy không thành công 
        exit(1);
    }
    sleep(atoi(argv[1]));
    // exit với giá trị 0 để thông báo tiến trình đã chạy không thành công
    exit(0);
}