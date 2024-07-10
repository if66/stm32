#include "iamgeprocess.h"
#include <QDebug>
IamgeProcess::IamgeProcess()
{

}
QByteArray IamgeProcess::imageBaseTo64ToUrlEncode(QString imagePth)
{
    QImage image(imagePth);
    QByteArray byte;
    //用QByteArray构造QBuffer
    QBuffer buf(&byte);
    buf.open(QIODevice::WriteOnly);
    image.save(&buf,"JPG");
    //对图片做base64编码(不包含编码头)
    QByteArray byteBase64 = byte.toBase64();
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QByteArray imgData = codec->fromUnicode(byteBase64)/*.toPercentEncoding()*/;
    return imgData;
}
