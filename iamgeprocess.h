#ifndef IAMGEPROCESS_H
#define IAMGEPROCESS_H
#include <QString>
#include <QByteArray>
#include <QImage>
#include <QBuffer>
#include <QTextCodec>
class IamgeProcess
{
public:
    IamgeProcess();
    static QByteArray imageBaseTo64ToUrlEncode(QString imagePth);
};

#endif // IAMGEPROCESS_H
