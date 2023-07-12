#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char const *argv[]) {

    char bite = 'x';//用来传递的字节

    int ch_to_fa[2], fa_to_ch[2];//声明了两个整型数组,用来存储管道的文件描述符
    //利用pipe系统调用，创建两个管道
    //读端是0，写端是1
    pipe(ch_to_fa);
    pipe(fa_to_ch);

    // fork系统调用
    int pid = fork();
    int exitNum = 0;

    if (pid == 0) { //子进程，要写给父
        close(fa_to_ch[1]);//关闭父管道写端
        close(ch_to_fa[0]);//关闭子管道读端

        if (read(fa_to_ch[0], &bite, sizeof(char)) != sizeof(char)) {//子读有错误
            printf("child read err\n");
            exitNum = 1;
        }
        else{//子进程成功读到，输出信息
            printf("%d: received ping\n", getpid());
        }

        if (write(ch_to_fa[1], &bite, sizeof(char)) != sizeof(char)) {//父写有错误
            printf("child write err\n");
            exitNum = 1;
        }

        exit(exitNum);
    }
    else { //父进程
        close(fa_to_ch[0]);//关闭父读端
        close(ch_to_fa[1]);//关闭子写端
        if (write(fa_to_ch[1], &bite, sizeof(char)) != sizeof(char)) {//父写有错误
            printf("parent write err\n");
            exitNum = 1; 
        }
        if (read(ch_to_fa[0], &bite, sizeof(char)) != sizeof(char)) {//子读有错误
            printf("parent read err\n");
            exitNum = 1;
        }
        else {
            printf("%d: received pong\n", getpid());//父成功读，打印信息
        }

        exit(exitNum);
    }
}