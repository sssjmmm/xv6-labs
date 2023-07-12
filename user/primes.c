#include "kernel/types.h"
#include "user/user.h"

//递归寻找素数
void primes(int lpipe[2])
{
    int firstNum, data;
    close(lpipe[1]);//关闭左管写端

    if (read(lpipe[0], &firstNum, sizeof(int)) != sizeof(int)) {//读取有误
        printf("read err\n");
        exit(1);
    }
    printf("prime %d\n", firstNum);
    
    //当前的管道
    int rpipe[2];
    pipe(rpipe);

    //依次读取前一个进程的数据，把不能被第一个数（质数）整除的数加入下一级进程
    while (read(lpipe[0], &data, sizeof(int)) == sizeof(int)) {// 从左管道读取数据
        if (data % firstNum != 0)// 将无法整除的数据传递入右管道
            write(rpipe[1], &data, sizeof(int));
    }

    close(lpipe[0]);
    close(rpipe[1]);

    //继续递归找质数
    if (fork() == 0) {
        primes(rpipe);
    }
    else {
        close(rpipe[0]);
        wait(0);
    }
    exit(0);
}

int main(int argc, char const *argv[])
{
    int p[2];
    pipe(p);//建立管道用于进程间通信
    // 写入初始数据
    for (int i = 2; i <= 35; i++)
        write(p[1], &i, sizeof(int));
    // 针对父子进程不同的操作
    if (fork() == 0) {//对于下一级子进程，递归调用函数输出质数
        primes(p);
    }
    else {//父进程要等待子进程结束
        close(p[1]);
        close(p[0]);
        wait(0);
    }
    exit(0);
}