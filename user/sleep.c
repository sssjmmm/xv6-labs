#include "kernel/types.h"
#include "user/user.h"

//argc表示命令行参数的数量，argv是一个字符串数组，其中存储了命令行参数的值
int main(int argc, char* argv[]) {
    if(argc < 2){//检查命令行参数的数量是否小于2
        fprintf(2, "err\n");//将字符串"err\n"输出到标准错误流（stderr）
        exit(1);
    }
    sleep(atoi(argv[1]));//将第1个参数转成整数
    exit(0);
}
