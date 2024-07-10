#include "approach.h"
#include <QFile>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <QDebug>
Approach::Approach()
{

}
void Approach::run()
{
    QString device;
    QString select_device ="/sys/bus/iio/devices/%1/name";
    //遍历设备
    for(int i =0;i<4;i++)
    {
        QString select = select_device.arg(QString("iio:device").append(QString::number(i)));
        QFile device_name(select);
        device_name.open(QIODevice::ReadOnly);
        QString ret = device_name.readAll();
        if(ret=="ap3216c\n"){
            switch (i) {
            case 0:
                device ="iio:device0";
                break;
            case 1:
                device ="iio:device1";
                break;
            case 2:
                device ="iio:device2";
                break;
            case 3:
                device ="iio:device3";
                break;
            }
        }
        device_name.close();
    }
    QString inproximity_raw_file = "/sys/bus/iio/devices/%1/in_proximity_raw";
    inproximity_raw_file = inproximity_raw_file.arg(device);
    int fd;
    char buf[32];
    while (1) {
        fd = open(inproximity_raw_file.toUtf8(),O_RDONLY);
        read(fd,buf,sizeof(buf));
        buf[strlen(buf)-1]='\0';
        QString str=QString::fromUtf8(buf);
        emit sendApproachAD(str);
        bzero(buf,sizeof (buf));
        sleep(3);
        close(fd);
    }
}
