
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int sieve(int p) {
  int num;
  int pipe_read;
  int pipe_write;
  int pid;

  // 读取当前进程的输入管道
  pipe_read = p;

  while (1) {
    // 读取下一个数字
    if (read(pipe_read, &num, sizeof(num)) == 0) {
      // 如果读取失败，则说明管道已经关闭，退出进程
      exit(0);
    }

    printf("prime %d\n", num);

    // 创建一个新的输出管道
    int pipe_fds[2];
    pipe(pipe_fds);
    pipe_write = pipe_fds[1];

    // 创建一个新的子进程
    pid = fork();
    if (pid < 0) {
      printf("fork failed\n");
      exit(0);
    } else if (pid == 0) {
      // 子进程负责筛选出num的倍数并写入输出管道
      close(pipe_fds[0]);
      int n;
      while (read(pipe_read, &n, sizeof(n)) != 0) {
        if (n % num != 0) {
          write(pipe_write, &n, sizeof(n));
        }
      }
      close(pipe_write);
      exit(0);
    } else {
      // 父进程关闭不需要的文件描述符
      close(pipe_read);
      close(pipe_write);

      // 等待子进程退出
      wait(0);
    }    
     pipe_read = pipe_fds[0];
  }
  return pipe_read;
}

int main(int argc, char *argv[]) {
  int pipe_fds[2];

  // 创建输入管道
  pipe(pipe_fds);

  // 写入数字2到35到输入管道
  for (int i = 2; i <= 35; i++) {
    write(pipe_fds[1], &i, sizeof(i));
  }

  // 关闭输入管道的写端
  close(pipe_fds[1]);

  // 开始筛法
  close(sieve(pipe_fds[0]));  

  exit(0);
}
