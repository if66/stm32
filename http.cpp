#include "http.h"
#include <QDebug>
Http::Http(QObject *parent) : QObject(parent)
{

}
bool Http::post_sync(QString url,QMap<QString,QString>header,QByteArray requestData,QByteArray &replyData)
{

//    发送请求的对象
    QNetworkAccessManager manager;
//    请求 对象
    QNetworkRequest request;
    request.setUrl(url);
    QMapIterator<QString,QString> it(header);
    while (it.hasNext()) {
        it.next();
        request.setRawHeader(it.key().toLatin1() ,it.value().toLatin1());
    }
    //设置openssl签名配置,否则在ARM上会报错
    QSslConfiguration conf = request.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
    conf.setProtocol(QSsl::TlsV1_0);
#else
    conf.setProtocol(QSsl::TlsV1);
#endif
    request.setSslConfiguration(conf);
    QNetworkReply *reply = manager.post(request,requestData);
    QEventLoop l;
    //一旦服务器返回，reply会发出信号

    connect(reply,&QNetworkReply::finished,&l,&QEventLoop::quit);

    l.exec();

    if(reply != nullptr && reply->error() == QNetworkReply::NoError)
    {

        replyData = reply->readAll();

        return true;
    }
    else
    {
        qDebug()<<"request error!";
        return false;
    }
}
