#include "readpe15thread.h"
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <QDebug>
ReadPE15Thread::ReadPE15Thread()
{

}
void ReadPE15Thread::run()
{
    system("touch pe15.txt");
    //system("gpioget gpiochip4 15 > pe15.txt");
    qDebug()<<"PE15 success";
    int fd;
    char buf[32];
    while (1) {
        system("gpioget gpiochip4 15 > pe15.txt");
        fd = open("./pe15.txt",O_RDONLY);
        read(fd,buf,sizeof(buf));
        if(strcmp(buf,"1\n")==0)
        {
            emit pesig(true);
        }
        else{
             emit pesig(false);
        }
        sleep(1);
        close(fd);
    }
}

