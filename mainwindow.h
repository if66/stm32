#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>
#include "v4l2api.h"
#include "iamgeprocess.h"
#include "http.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QZXing>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "syszuxpinyin.h"
#include "my_lineedit.h"
#include "readpe15thread.h"
#include "approach.h"
#include "autocheck.h"
#include "readkeythread.h"
#include <QtMqtt/qmqttclient.h>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <linux/input.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    SyszuxPinyin *syszuxpinyin_faceInfo;
    SyszuxPinyin *syszuxpinyin_adminName;
    SyszuxPinyin *syszuxpinyin_adminPassword;
    ~MainWindow();
private slots:

    void on_openBt_clicked();

    void on_closeBt_clicked();
    void recvImage(QImage image);
    void on_saveBt_clicked();

    void on_recognitionBt_clicked();

    void on_pushButton_ewm_clicked();

    void on_saveInfoBt_clicked();

    void keyboardshow_faceInfo(QString data);
    void confirmString_faceInfo(QString gemfield);
    /*****admin****/
    void keyboardshow_adminName(QString data);
    void confirmString_adminName(QString gemfield);
    void keyboardshow_adminPassword(QString data);
    void confirmString_adminPassword(QString gemfield);
    void on_exitBt_clicked();

    void on_loginBt_clicked();
//    光电开关
    void photoelectricSwitchSlot(bool);
//接近
    void ApproachADSlot(QString ad);
    void beepring();
    void beepunring();

    void on_pushButton_autoCheck_clicked();
    void setKeySlot(char key1);
    void autocheck_timeInit_slot(uint,uint);
    void on_pushButton_facesave_clicked();
    void on_quitapp_clicked();

private:
    Ui::MainWindow *ui;
    V4l2Api vapi;
    QImage Saveimage;
    bool cameraTorF = false;
    bool Keypadding = false;
    int faceMatching =-1; //判断是否人脸是否和人脸库匹配;
    QString accessToken;
    QString imgUrl;
    int picNum;
    void relay13Pin();
    void Ioctl(int fd, unsigned long cmd, void* arg);

    /*******SQL******/
    QSqlDatabase initDb();
    void createTable();
    void insertFaceInfo(int id);
    QString selectFaceInfo(int id);
    My_lineEdit *lineEdit_faceInfo;
    void initLineEdit();
    QString faceInfo;//全局变量，用来存储lineEdit获取的数据
//    创建管理员表
    void createadminTable();
    void insertadminUser(QString user,QString password);
    bool selectadminUser(QString user,QString password);
    My_lineEdit *lineEdit_adminName;
    My_lineEdit *lineEdit_adminPassword;
    void initadminLineEdit();

    void normalUserShow();
    void adminShow();
    //判断是否填充秘钥
    void detecKey();
    //光电开关
    ReadPE15Thread pe15thread;
    Approach approachAD;
    void showPic();
    //key
    ReadKeyThread readKeythread;
    //autoCheck
    AutoCheck autocheck;
    bool peopleApproach =false;
    uint startTime;
    uint stopTime;
    /***************************   连接百度云模块  **************************************/
    void baiduInit();
    QMqttClient  *baidu_client;//mqtt client指针
    void baidu_pubInfoSlot(QString baidupubTopic,QString baidupubText);

    void btn_setTransparent();
};
#endif // MAINWINDOW_H
