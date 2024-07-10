#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtMqtt/qmqttclient.h>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardItemModel>
#include <QScrollBar>
#include <QTimer>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/input.h>
#include <unistd.h>
#include "set_wifi.h"
#include "collentdatathread.h"
#include "readpe15thread.h"
#include "login.h"
#include "register.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
     Set_Wifi *WifiDlg;
     void Get_Wifi_Name();
     SyszuxPinyin *syszuxpinyin_ledlv1;
     SyszuxPinyin *syszuxpinyin_ledlv2;
     SyszuxPinyin *syszuxpinyin_ledlv3;
     SyszuxPinyin *syszuxpinyin_fan;
     SyszuxPinyin *syszuxpinyin_iotCoreid;
     SyszuxPinyin *syszuxpinyin_iotDevKey;
     SyszuxPinyin *syszuxpinyin_iotDevSecret;
private slots:
    void on_connectBTN_clicked();
    void on_connectBTN_2_clicked();
    void replyFinished(QNetworkReply *reply); //天气数据处理槽函数
    void weather_cilcked_Slot();
    void set_humAdtemAdill(QString tem,QString hum,QString ill);
    /******************************    控制模块  *********************************/
    void led1_on_btnSlot();
    void led1_off_btnSlot();
    void led2_on_btnSlot();
    void led2_off_btnSlot();
    void led3_on_btnSlot();
    void led3_off_btnSlot();
    void fan_on_btnSlot();
    void fan_off_btnSlot();
    void fan_2_btnSlot();
    void fan_3_btnSlot();
    void beep_on_btnSlot();
    void beep_off_btnSlot();
    /***************************** 设置异常阈值 *****************************/
     void pushButton_abn_ledSlot();
     void pushButton_abn_fanSlot();
     void abn_keyboardshow_ledlv1(QString data);
     void abn_keyboardshow_ledlv2(QString data);
     void abn_keyboardshow_ledlv3(QString data);
     void abn_keyboardshow_fan(QString data);
     void abn_confirmString_ledlv1(QString gemfield);   //接收键盘发过来的数据
     void abn_confirmString_ledlv2(QString gemfield);   //接收键盘发过来的数据
     void abn_confirmString_ledlv3(QString gemfield);   //接收键盘发过来的数据
     void abn_confirmString_fan(QString gemfield);   //接收键盘发过来的数据
     void abn_pushbutton_ONSlot();
     void abn_pushbutton_OFFSlot();
     /***************************   连接百度云模块  **************************************/
     void pushButton_gettimeSlot();
     void pushButton_calculateSlot();
     void pushButton_connectmqttSlot();
     void ito_keyboardshow_coreldSlot(QString data);
     void ito_keyboardshow_devKeySlot(QString data);
     void ito_keyboardshow_devSecretSlot(QString data);
     void ito_confirmString_coreldSlot(QString gemfield);   //接收键盘发过来的数据
     void ito_confirmString_devKeySlot(QString gemfield);   //接收键盘发过来的数据
     void ito_confirmString_devSecretSlot(QString gemfield);   //接收键盘发过来的数据
     void messageReceived(const QByteArray &message, const QMqttTopicName &topic);
     void TimertimeOut();
     void mqttconnectSlot();
     /********************************  门禁登陆  *****************************************/
     void loginSlot();
     void loginfailedSlot();
     void loginsuccessSlot();
     void logincloseSlot();
     /******************     time   **********/
     void timeUpdate();

     void on_quitappioT_clicked();

     void on_quitappabnormal_clicked();

     void on_quitappcotrol_clicked();

     void on_quitapphumiture_clicked();

     void on_quitappweather_clicked();

     void on_quitappwifi_clicked();

private:
    Ui::MainWindow *ui;
    /****************************** 天气模块 ***************************************/
    //请求句柄
    QNetworkAccessManager *manager;
    void sendQuest(QString cityStr);
    /*****************************  数据采集模块  ***********************************/
    QString hum;
    QString tem;
    QString ill;
    //数据采集线程
    CollentdataThread thread_collentdata;
    /***************************** 异常阈值 *****************************/
    QString tem_max;
    QString ill_lv1;
    QString ill_lv2;
    QString ill_lv3;
    My_lineEdit *lineEdit_led_lv1;
    My_lineEdit *lineEdit_led_lv2;
    My_lineEdit *lineEdit_led_lv3;
    My_lineEdit *lineEdit_fan_tem;
    /***************************   连接百度云模块  **************************************/
    QMqttClient  *m_client;//mqtt client指针
    QString currentTimestamp;
    QString ioTCoreld;
    QString deviceKey;
    QString deviceSecret;
    QString brokerAddr;
    QString brokerPort;
    QString clientId;
    QString username;
    QString password;
    QString password_md5;
    QString mqttTopic;
    QString mqttMessage;
    My_lineEdit *lineEdit_coreid;
    My_lineEdit *lineEdit_devkey;
    My_lineEdit *lineEdit_devsecret;
    QTimer *m_timer;
    void InitTimer();
    /********************************  门禁登陆  *****************************************/
    ReadPE15Thread pe15thread;
    Login *login;
    void beepring();
    void beepunring();
    Register *regist;
      /******************     time   **********/
    QTimer *currentTime;


    void Ioctl(unsigned long cmd, void* arg);
    int board_type = 0;
};

#endif // MAINWINDOW_H
