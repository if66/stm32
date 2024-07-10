#include "mainwindow.h"
#include "ui_mainwindow.h"
void MainWindow::baiduInit()
{
    baidu_client = new QMqttClient(this);
    QString baidu_username;
    QString baidu_password;
    QString baidu_password_md5;
    QString baidu_brokerAddr;
    QString baidu_ioTCoreld = "afeclpw";
    QString baidu_deviceKey = "AiFace";
    QString baidu_deviceSecret = "PNzvsfYKXGJenTnw";
    QString baidu_brokerPort = "1883";
    QString currentTimestamp="0";
    QString baidu_clientId = "AiFace";
    baidu_brokerAddr = QString(baidu_brokerAddr.append(baidu_ioTCoreld).append(".iot.").append("gz").append(".baidubce.com"));
    baidu_username = baidu_username.append("thingidp").append("@").append(baidu_ioTCoreld).append("|").append(baidu_deviceKey)\
            .append("|").append(currentTimestamp).append("|").append("MD5");
    baidu_password = baidu_password.append(baidu_deviceKey).append("&").append(currentTimestamp).append("&").append("MD5")\
            .append(baidu_deviceSecret);
    QByteArray baidu_password_md5result;
    QCryptographicHash md(QCryptographicHash::Md5);
    md.addData(baidu_password.toUtf8());
    baidu_password_md5result = md.result();
    baidu_password_md5.append(baidu_password_md5result.toHex());
    //未连接服务器则连接
    if (baidu_client->state() == QMqttClient::Disconnected) {
        baidu_client->setHostname(baidu_brokerAddr);
        baidu_client->setPort(baidu_brokerPort.toInt());
        baidu_client->setUsername(baidu_username);
        baidu_client->setPassword(baidu_password_md5);
        baidu_client->connectToHost();
    }
    if(baidu_client->state()!=QMqttClient::Disconnected){
        qDebug()<<"MQTT connecting";
    }
}
void MainWindow::baidu_pubInfoSlot(QString baidupubTopic,QString baidupubText)
{
    if(baidu_client->publish(baidupubTopic,baidupubText.toUtf8()) == -1)
    {
        return ;
    }
}
