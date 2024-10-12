#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"

void find(char *dir, char *filename) {
  char bufferPath[MAXPATH]; // Độ dài tối đa của một đường dẫn là MAXPATH
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(dir, O_RDONLY)) < 0) {
    printf("find: cannot open %s\n", dir);
    return;
  }

  // Lấy thông tin của thư mục
  if (fstat(fd, &st) < 0) {
    printf("find: cannot stat %s\n", dir);
    close(fd);
    return;
  }

  // Kiểm tra nếu dir không phải là thư mục thì dừng
  if (st.type != T_DIR) {
    printf("find: %s is not a directory\n", dir);
    close(fd);
    return;
  }

  // Chuẩn bị buffer chứa đường dẫn
  if (strlen(dir) + 1 + DIRSIZ + 1 > MAXPATH) {
    printf("find: path too long\n");
    close(fd);
    return;
  }
  // Xử lý đường dẫn thư mục
  strcpy(bufferPath, dir);
  char* endPath;
  endPath = bufferPath + strlen(bufferPath);
  *endPath++ = '/';

  // Đọc các phần tử trong thư mục
  while (read(fd, &de, sizeof(de)) == sizeof(de)) {
    if (de.inum == 0) // bỏ qua các phần tử không hợp lệ, đã bị xóa, v.v.
      continue;

    // Sao chép tên file vào đường dẫn
    memmove(endPath, de.name, DIRSIZ);
    endPath[DIRSIZ] = 0;

    // Bỏ qua "." và ".."
    if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
      continue;

    // Lấy thông tin chi tiết của phần tử
    if (stat(bufferPath, &st) < 0) {
      printf("find: cannot stat %s\n", bufferPath);
      continue;
    }

    // So sánh tên file với filename
    if (st.type == T_FILE && strcmp(de.name, filename) == 0) {
      printf("%s\n", bufferPath); 
    }

    // Nếu phần tử là thư mục, đệ quy để tìm trong thư mục con
    if (st.type == T_DIR) {
      find(bufferPath, filename);
    }
  }

  close(fd);
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(2, "To use find: find <dir> <filename>\n");
    exit(1);
  }
  
  find(argv[1], argv[2]);
  exit(0);
}
