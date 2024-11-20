#include "kernel/types.h"
#include "user/user.h"

// Cảnh báo hàm đệ quy này sẽ không return
void primes(int p[2]) __attribute__((noreturn));

void primes(int p[2]) {
    int prime, n;
    
    close(p[1]);

    // Đọc số đầu tiên (đây chắc chắn luôn là một số nguyên tố)
    if (read(p[0], &prime, sizeof(prime)) == 0) {
        // Trường hợp pipe trống, không còn số để xử lý -> kết thúc
        close(p[0]);
        exit(0);
    }

    printf("prime %d\n", prime);

    // Tạo một pipe mới cho process tiếp theo
    int new_p[2];
    pipe(new_p);
    int pid = fork();
    if(pid < 0){
        printf("primes: Failed to create a new process\n");
        exit(1);
    }
    else if (pid == 0) {
        // Trước khi gọi đệ quy để xử lý cần phải đóng đầu đọc ở pipe hiện tại
        close(p[0]);
        primes(new_p);
    } else {
        close(new_p[0]);
        // Thực hiện truyền các số các khả năng sẽ là số nguyên tố từ pipe cũ qua pipe mới
        while (read(p[0], &n, sizeof(n)) > 0) {
            if (n % prime != 0) {
                write(new_p[1], &n, sizeof(n));
            }
        }
        close(p[0]);
        close(new_p[1]);
        wait(0);
        exit(0);
    }
}

int main() {
    int p[2];
    pipe(p);
    int pid = fork();
    if(pid < 0){
        printf("primes: Failed to create a new process\n");
        exit(1);
    }
    else if (pid == 0) {
        primes(p);
    } else {
        
        close(p[0]); 
        // Ghi các số vào pipe
        for (int i = 2; i <= 280; i++) {
            write(p[1], &i, sizeof(i));
        }

        close(p[1]);  
        wait(0);      
        exit(0);
    }
}
