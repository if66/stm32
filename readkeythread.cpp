#include "readkeythread.h"
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <QDebug>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>


#define DEVICE_KEY			"/dev/input/event5"
#define BOARD_TYPE_FSMP1A            1
#define BOARD_TYPE_FSMP1C            2

static int get_board_type(void)
{
    char searchText[]="-fsmp1a";
    int len_searchText;
    FILE *file;
    char string[128];
    int len_string;
    int i = 0;

    memset(string, 0x0, sizeof(string));

    file = fopen("/proc/device-tree/compatible", "r");
    if (file == NULL) {
        qDebug("fails to open /proc/device-tree/compatible\n");
        return -1;
    }

    len_searchText = strlen(searchText);

    while(fgets(string, sizeof(string), file) != 0)
    {
        len_string = strlen(string);
        for(i = 0 ; i < len_string ; i++) {
            if(strncmp(searchText, (string + i), len_searchText) == 0) {
                fclose(file);
                return BOARD_TYPE_FSMP1A;
            }
        }
    }
    fclose(file);
    return BOARD_TYPE_FSMP1C;
}
ReadKeyThread::ReadKeyThread()
{

}
void ReadKeyThread::run()
{  
    int fd1;
    char buf1[32];
    int board_type = get_board_type();
    if(board_type==BOARD_TYPE_FSMP1A)
       {
        while (1) {
            system("gpioget gpiochip5 9 > key1.txt");
            fd1 = open("./key1.txt",O_RDONLY);
            read(fd1,buf1,sizeof(buf1));
            emit keySignal(buf1[0]);
            bzero(buf1,sizeof (buf1));
            close(fd1);
            //system("rm key1.txt");
        }
    }
    else if(board_type==BOARD_TYPE_FSMP1C)
   {
        int fd = -1, ret = -1;
        struct input_event ev;
        char buf1[32];
        // 第1步：打开设备文件
        fd = open(DEVICE_KEY, O_RDONLY);
        if (fd < 0) {
            perror("open");
            return;
        }

        while (1) {
            // 第2步：读取一个event事件包
            bzero(buf1,sizeof (buf1));
            memset(&ev, 0, sizeof(struct input_event));
            ret = read(fd, &ev, sizeof(struct input_event));
            if (ret != sizeof(struct input_event))
            {
                perror("read");
                close(fd);
                return ;
            }
            if(ev.code ==44)
            {
                if(ev.value==1)
                    strcpy(buf1,"1");
                else
                    strcpy(buf1,"0");
            }
                emit keySignal(buf1[0]);
        }
        // 第4步：关闭设备
        close(fd);
    }

}
