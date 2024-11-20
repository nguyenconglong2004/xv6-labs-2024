#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAX_LENGTH_OF_LINE 256 // Chiều dài tối đa một dòng
int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("xargs: requires least 1 argument\n");
        exit(1);
    }
    // Kiểm tra hàm xử lý quá mức số lượng tham số cho phép
    if (argc > MAXARG)
    {
        printf("xargs: too much arguments\n");
        exit(1);
    }

    // Tính toán vị trí của tên chương trình trong dãy tham số
    int argStartIndex = 1;
    if (strcmp(argv[1], "-n") == 0)
        argStartIndex = 3;

    // Số lượng tham số chỉ tính trên xargs gọi
    int subArgc = argc - argStartIndex - 1;

    // Copy các tham số này lưu vào bộ đệm để truyền vào hàm exec thực hiện gọi chương trình
    char *subArgumentsBuffer[MAXARG];
    for (int i = 0; i <= subArgc; i++)
    {
        subArgumentsBuffer[i] = argv[i + argStartIndex];
    }
    char inputBuffer[MAX_LENGTH_OF_LINE];
    char c;
    int l = 0;
    while (read(0, &c, 1) > 0)
    {
        // Mặc định phân cách bởi dấu xuống dòng thì được xử lý
        if (c != '\n')
            inputBuffer[l++] = c;
        else
        {
            inputBuffer[l] = 0;
            // Thêm giá trị vừa đọc được ở stdin vào bộ đệm
            subArgumentsBuffer[subArgc + 1] = inputBuffer;
            subArgumentsBuffer[MAXARG - 1] = 0;
            int pid = fork();
            if (pid < 0)
            {
                printf("xargs: failed to create new process\n");
                exit(1);
            }
            else if (pid == 0)
            {
                exec(argv[argStartIndex], subArgumentsBuffer);
                exit(0);
            }
            else
            {
                l = 0;
                wait(0);
            }
        }
    }

    exit(0);
}
