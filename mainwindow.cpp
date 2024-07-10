#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <unistd.h>
#include <QMovie>
const QString baiduTokenUrl = "https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=%1&client_secret=%2&";
static QString client_id;
static QString secret_id;
const QString baiduImageUrl = "https://aip.baidubce.com/rest/2.0/face/v3/match?access_token=%1";
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->quitapp->setStyleSheet("QPushButton{border:none;outline:none;background-color:transparent;}");
    ui->quitapp->setFixedSize(30,30);
    ui->quitapp->setIcon(QIcon(":/icon/images/exit1.png"));
    ui->quitapp->setIconSize(QSize(50,50));
//    初始化自定义LineEdit
    initLineEdit();
    initadminLineEdit();
    normalUserShow();
//   创建管理员用户表
    createadminTable();
    insertadminUser("user","123");
    qDebug()<<"password success";
    /**判断是否填充过秘钥**/
    detecKey();
    on_openBt_clicked();
    connect(&pe15thread,SIGNAL(pesig(bool)),this,SLOT(photoelectricSwitchSlot(bool)));
    pe15thread.start();
    connect(&approachAD,SIGNAL(sendApproachAD(QString)),this,SLOT(ApproachADSlot(QString)));
    approachAD.start();

    connect(&readKeythread,SIGNAL(keySignal(char)),this,SLOT(setKeySlot(char)));
    readKeythread.start();

    connect(&autocheck,SIGNAL(sendTime(uint,uint)),this,SLOT(autocheck_timeInit_slot(uint,uint)));
    //MQTT Connect
    baiduInit();
    //set BTN
    btn_setTransparent();
}
void MainWindow::setKeySlot(char key1)
{
    if(key1 == '0')
    {
        on_openBt_clicked();
        on_recognitionBt_clicked();
    }else{
        return ;
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openBt_clicked()
{
    cameraTorF = true;
    connect(&vapi, &V4l2Api::sendImage, this, &MainWindow::recvImage);
    vapi.start();
}

void MainWindow::on_closeBt_clicked()
{
    cameraTorF = false;
    disconnect(&vapi, &V4l2Api::sendImage, this, &MainWindow::recvImage);
    ui->label->clear();
    showPic();
}
void MainWindow::recvImage(QImage image)
{
    Saveimage = image;
    QPixmap mmp = QPixmap::fromImage(image);
    ui->label->setPixmap(mmp);
    ui->label->setScaledContents(true);
}

void MainWindow::on_saveBt_clicked()
{
    if(cameraTorF == false){
        QMessageBox::warning(this, "警告", "请先采集图像");
        return ;
    }
    disconnect(&vapi, &V4l2Api::sendImage, this, &MainWindow::recvImage);
    //cameraTorF = false;
    //    判断是否录入姓名
    if(faceInfo ==""){
        QMessageBox::warning(this, "警告", "请先录入姓名");
        //        使摄像头重新采集图像
        on_openBt_clicked();
        return ;
    }

    QFile file("./face/num.txt");
    if(file.exists()) {
        file.open(QIODevice::ReadWrite);
        QByteArray buf = file.readLine();
        picNum = buf.toUInt();
        picNum++;
        qDebug()<<picNum;
        file.close();
        QString echoNumTxt = "echo %1 > ./face/num.txt";
        system(echoNumTxt.arg(picNum).toUtf8());
        //        如果不存在表，创建
        createTable();
        //        将人脸信息插入到数据库
        insertFaceInfo(picNum);
        //        清空全局变量
        faceInfo.clear();

        QString faceNpath ="./face/face%1.jpg";
        Saveimage.save(faceNpath.arg(picNum),"JPG",100);
        ui->label->setPixmap(QPixmap::fromImage(Saveimage));
    }else{
        system("mkdir face");
        system("echo 0 > ./face/num.txt");
        Saveimage.save("./face/face0.jpg","JPG",100);
        ui->label->setPixmap(QPixmap::fromImage(Saveimage));
        //        如果不存在表，创建
        createTable();
        //        将人脸信息插入到数据库
        insertFaceInfo(0);
        //        清空全局变量
        faceInfo.clear();
    }
   QMessageBox::information(this, "提示", "人脸录入成功");
   on_openBt_clicked();
}

void MainWindow::on_recognitionBt_clicked()
{
    ui->textEdit->clear();
    if(Keypadding ==false)
    {
        QMessageBox::warning(this, "警告", "请先填充秘钥");
        return ;
    }
    //    人脸识别的图像
    if(cameraTorF == false)
    {
        QMessageBox::warning(this, "警告", "请先采集图像");
        return ;
    }
    disconnect(&vapi, &V4l2Api::sendImage, this, &MainWindow::recvImage);
    //cameraTorF = false;
    Saveimage.save("./face/now.jpg","JPG",100);
    ui->label->setPixmap(QPixmap::fromImage(Saveimage));
//只获取一次access_token
    //获取access_token
    QByteArray replyData;                 //保存回复信息
    QString url = QString(baiduTokenUrl).arg(client_id).arg(secret_id);
    QMap<QString, QString> header;        //封装头部信息
    header.insert(QString("Content-Type"), QString("application/json"));

    bool result = Http::post_sync(url, header, "image", replyData);
    if (result)
    {
        QJsonObject obj = QJsonDocument::fromJson(replyData).object();
        accessToken = obj.value("access_token").toString();
        imgUrl = baiduImageUrl.arg(accessToken);
        qDebug()<<accessToken;
    }else{
          QMessageBox::warning(this, "警告", "请检查网络");
          qDebug()<<"accessToken error";
          return ;
    }
//遍历人脸库
    QFile file("./face/num.txt");
    if(file.exists())
    {
        ui->textEdit->append("正在识别......");
        file.open(QIODevice::ReadWrite);
        QByteArray buf = file.readLine();
        picNum = buf.toUInt();
        QString faceNpath = "./face/face%1.jpg";
        for(int i=0;i<=picNum;i++)
        {
            //        组合Json字符串
            QByteArray beforeimg = IamgeProcess::imageBaseTo64ToUrlEncode(faceNpath.arg(i));
            QByteArray nowimg = IamgeProcess::imageBaseTo64ToUrlEncode("./face/now.jpg");
            QString sendDATA = "[{\"image\":";
            sendDATA.append("\"").append(beforeimg).append("\",")
                    .append("\"image_type\":").append("\"BASE64\"").append(",")
                    .append("\"face_type\":").append("\"LIVE\"").append(",")
                    .append("\"quality_control\":").append("\"NONE\"").append(",")
                    .append("\"liveness_control\":").append("\"NONE\"").append("},{")
                    .append("\"image\":").append("\"").append(nowimg).append("\",")
                    .append("\"image_type\":").append("\"BASE64\"").append(",")
                    .append("\"face_type\":").append("\"LIVE\"").append(",")
                    .append("\"quality_control\":").append("\"NONE\"").append(",")
                    .append("\"liveness_control\":").append("\"NONE\"").append("}]");
            qDebug()<<imgUrl;
            bool result = Http::post_sync(imgUrl, header, sendDATA.toLatin1(), replyData);
            if (result)
            {
                QJsonObject obj = QJsonDocument::fromJson(replyData).object();
                QJsonValue val = obj.value("result");
                qDebug()<<obj;
                if(val.isObject())
                {
                    QJsonObject jsonObject = val.toObject();
                    if(jsonObject.contains("score")){
                        double score = jsonObject.value("score").toDouble();
                        qDebug()<<score;
                        if(score >=80){
                            faceMatching =i;
                            //  结束循环，提高效率
                            break;
                        }else{
                            faceMatching =-1;
                        }
                    }
                }
            }else{
                ui->textEdit->clear();
                ui->textEdit->append("请确保网络通畅");
//                重新启动摄像头
                on_openBt_clicked();
                return ;
            }
        }
        //    判断人脸是否匹配成功
        if(faceMatching!=-1){
            ui->textEdit->clear();
            QString successSelect = selectFaceInfo(faceMatching);
            ui->textEdit->append("匹配成功");
            ui->textEdit->append(QString("欢迎用户").append(successSelect).append("，门锁已打开,祝您身体健康！"));
            relay13Pin();
        }else{
            ui->textEdit->clear();
            ui->textEdit->append("人脸不存在");
           //重新启动摄像头
            on_openBt_clicked();
        }
    }
    else{
        QMessageBox::warning(this, "警告", "请告知管理员添加人脸库");
    }
}
#define D1_WRITE (_IOW('f', 16, unsigned char))
void MainWindow::relay13Pin()
{
    unsigned long cmd_ctrl;
    unsigned char arg;
    int fd;
    fd = open("/dev/sensor-manage", O_RDWR);
    if(fd < 0) {
        qDebug("open");
        return ;
    }
    cmd_ctrl = D1_WRITE;
    arg = 1;
    Ioctl(fd, cmd_ctrl, &arg);

    cmd_ctrl = D1_WRITE;
    arg = 0;
    Ioctl(fd, cmd_ctrl, &arg);

    cmd_ctrl = D1_WRITE;
    arg = 1;
    Ioctl(fd, cmd_ctrl, &arg);
}
void MainWindow::Ioctl(int fd, unsigned long cmd, void* arg)
{
    if(ioctl(fd, cmd, arg))
        perror("ioctl error");
}
void MainWindow::on_pushButton_ewm_clicked()
{

    if(cameraTorF == false)
    {
        QMessageBox::warning(this, "警告", "请先采集图像");
        return ;
    }
    disconnect(&vapi, &V4l2Api::sendImage, this, &MainWindow::recvImage);
   // cameraTorF = false;

    Saveimage.save("./ewm.jpg","JPG",100);
    ui->label->setPixmap(QPixmap::fromImage(Saveimage));

     QZXing zxing;
     QString imagePth ="./ewm.jpg";
     QImage image(imagePth);
     QByteArray byte = zxing.decodeImage(image).toUtf8();//该方法返回QString串，标识图片二维码的内容
     QJsonObject obj = QJsonDocument::fromJson(byte).object();
//     system("touch client_id.txt");
//     system("touch secret_id.txt");
     client_id = obj.value("client_id").toString();
     secret_id = obj.value("secret_id").toString();
//     client_id = "oEXUYtRAw4iip8SBDcRzhYpr";
//     secret_id = "49glCtdsCZCjXBKmGXpD9dXhaKPiYcRt";
     detecKey();
     qDebug()<< "client_id:"<<client_id;
     qDebug()<< "secret_id:"<<secret_id;

     if(client_id =="" || secret_id == "")
     {
         QMessageBox::warning(this, "警告", "请重新填充秘钥");
         on_openBt_clicked();
         return ;
     }
     else
     {
         QMessageBox::information(this, "提示", "填充秘钥成功");
         ui->pushButton_ewm->setIcon(QIcon(":/icon/images/08.png"));
         ui->pushButton_ewm->setIconSize(QSize(171,51));
         ui->pushButton_ewm->setEnabled(false);
         Keypadding = true;
         on_openBt_clicked();
     }
     /**将秘钥保存到本地****/

//     QFile client_idFile("./client_id.txt");
//     client_idFile.open(QIODevice::ReadWrite);
//     client_idFile.write(client_id.toUtf8());
//     client_idFile.close();
//     QFile secret_idFile("./secret_id.txt");
//     secret_idFile.open(QIODevice::ReadWrite);
//     secret_idFile.write(secret_id.toUtf8());
//     secret_idFile.close();
}

void MainWindow::on_saveInfoBt_clicked()
{
    faceInfo.clear();
    faceInfo =lineEdit_faceInfo->text();
    QMessageBox::information(this, "提示", "姓名填充成功");
}
void MainWindow::keyboardshow_faceInfo(QString data)
{
    syszuxpinyin_faceInfo->lineEdit_window->setText(data);
    syszuxpinyin_faceInfo->resize(800,348);
    syszuxpinyin_faceInfo->move(100,280);
    syszuxpinyin_faceInfo->show();
}
void MainWindow::confirmString_faceInfo(QString gemfield)
{
    lineEdit_faceInfo->setText(gemfield);
}
void MainWindow::initLineEdit()
{
    lineEdit_faceInfo = new My_lineEdit(this);
    lineEdit_faceInfo->setObjectName("lineEdit_faceInfo");
    lineEdit_faceInfo->setGeometry(QRect(560, 235, 241, 35));
    lineEdit_faceInfo->setStyleSheet("QLineEdit{background-color:transparent}"
                                        "QLineEdit{border-width:0;border-style:outset}");
    syszuxpinyin_faceInfo =new SyszuxPinyin();
    connect(lineEdit_faceInfo,SIGNAL(send_show(QString)),this,SLOT(keyboardshow_faceInfo(QString)));
    connect(syszuxpinyin_faceInfo,SIGNAL(sendPinyin(QString)),this,SLOT(confirmString_faceInfo(QString)));
}

void MainWindow::on_exitBt_clicked()
{
    normalUserShow();
//    清空
    lineEdit_adminName->clear();
    lineEdit_adminPassword->clear();
}

void MainWindow::on_loginBt_clicked()
{

    bool ret = selectadminUser(lineEdit_adminName->text(),lineEdit_adminPassword->text());
    if(ret == true){
        QMessageBox::information(this, "提示", "登陆成功");
        adminShow();
    }else{
        QMessageBox::warning(this, "警告", "请输入正确的管理员用户和密码");
        return ;
    }
}
void MainWindow::detecKey()
{

    /**判断是否填充过秘钥**/
    QFile client_idFile("/root/client_id.txt");
    if(client_idFile.exists()){
        client_idFile.open(QIODevice::ReadWrite);
        QByteArray cli_id = client_idFile.readAll();
        if (!cli_id.isEmpty() && cli_id.endsWith('\n')) {
            cli_id.chop(1); // 删除末尾的换行符
        }
        client_id = cli_id;
        Keypadding = true;
        client_idFile.close();
    }else{
        QMessageBox::warning(this, "警告", "没有找到client_id.txt，请在root目录下创建并填写内容");
        Keypadding = false;
    }

    QFile secret_idFile("/root/secret_id.txt");
    if(secret_idFile.exists()){
        secret_idFile.open(QIODevice::ReadWrite);
        QByteArray sec_id = secret_idFile.readAll();
        if (!sec_id.isEmpty() && sec_id.endsWith('\n')) {
            sec_id.chop(1); // 删除末尾的换行符
        }
        secret_id = sec_id;

        Keypadding = true ;
        secret_idFile.close();
    }else{
        QMessageBox::warning(this, "警告", "没有找到secret_id.txt，请在root目录下创建并填写内容");
        Keypadding = false;
    }
}
void MainWindow::photoelectricSwitchSlot(bool Switch)
{
    QDateTime time = QDateTime::currentDateTime();   //获取当前时间
    uint timeT = time.toTime_t();   //将当前时间转为时间戳
    if(startTime<=timeT&&timeT<=stopTime){
        if(Switch == true){
            if(peopleApproach == true){
                /*..存储数据库 ，连接百度云
                 * .非法闯入.*/
                AutoCheck::createTable();
                beepring();
                sleep(2);
                beepunring();
                AutoCheck::insertInfo(QString(QDateTime::currentDateTime().toString()).append(" 有人非法闯入"));
                //上传百度云
                baidu_pubInfoSlot("$iot/AiFace/user/sendMsg",QString(QDateTime::currentDateTime().toString()).append(" 有人非法闯入"));
            }
        }else{
            return ;
        }
        peopleApproach = false;
    }
}
void MainWindow::beepring()
{

    int fd;
    struct input_event event;
    struct timeval time;
    fd = open("/dev/input/by-path/platform-beeper-event", O_RDWR);
    event.type = EV_SND;
    event.code = SND_TONE;
    event.value = 1000;
    time.tv_sec = 1;
    time.tv_usec = 0;
    event.time = time;
    write(fd, &event, sizeof(struct input_event));
    qDebug()<<"beepr end";
}
void MainWindow::beepunring()
{
    int fd;
    struct input_event event;
    struct timeval time;
    fd = open("/dev/input/by-path/platform-beeper-event", O_RDWR);
    event.type = EV_SND;
    event.code = SND_TONE;
    event.value = 0;
    time.tv_sec = 0;
    time.tv_usec = 0;
    event.time = time;
    write(fd, &event, sizeof(struct input_event));
}
void MainWindow::ApproachADSlot(QString ad)
{
    if(ad.toInt()>=800){
        peopleApproach = true;
    }else{
        peopleApproach = false;
    }
}
void MainWindow::showPic()
{
    //图片
    QPixmap pix0(":/icon/images/猫眼04_02.jpg");
    //设置图片
    ui->label->setPixmap(pix0);
}
//显示设置布防时间界面
void MainWindow::on_pushButton_autoCheck_clicked()
{

    autocheck.show();
    autocheck.move(383,0);
}
void MainWindow::autocheck_timeInit_slot(uint startTime,uint stopTime)
{

    this->startTime = startTime;
    this->stopTime = stopTime;
}

void MainWindow::on_pushButton_facesave_clicked()
{

    autocheck.close();
}

void MainWindow::on_quitapp_clicked()
{
    this->close();
    QApplication::quit();
}
