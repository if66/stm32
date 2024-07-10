#include "mainwindow.h"
#include "ui_mainwindow.h"
void MainWindow::createadminTable()
{
    QSqlDatabase db = initDb();
    //创建一个数据库操作对象
    QSqlQuery sq(db);
    QString sql = "CREATE TABLE IF NOT EXISTS adminTable(user TEXT PRIMARY KEY,password TEXT);";
    if(sq.exec(sql))
        qDebug() << "admin CREATE TABLE success";
    else
        qDebug() << "adminTable exist";
    db.close();
}
void MainWindow::insertadminUser(QString user,QString password)
{
    QSqlDatabase db = initDb();
    QSqlQuery sq(db);
    //防止sql注入问题
    QString sql = "INSERT INTO adminTable VALUES(?,?)";
    //预处理
    sq.prepare(sql);
    //占位符数据替换
    sq.addBindValue(user);
    sq.addBindValue(password);
    //执行sql语句
    if(sq.exec())
    {
       qDebug()<<"admin success";
    }
    else
    {
        QString errorMsg = sq.lastError().text();
        qDebug()<<"errorMsg:"<<errorMsg;
    }
    db.close();
}
bool MainWindow::selectadminUser(QString user,QString password)
{
    QString sqlPassword;
    QSqlDatabase db = initDb();
    QSqlQuery query(db);
    //防止sql注入问题
    QString sql = "select * from adminTable where user=?;";
    //预处理
    query.prepare(sql);
    //占位符数据替换
    query.addBindValue(user);
    if(!query.exec()){
        db.close();
        return false;
    }
    while(query.next()){
        sqlPassword =   query.value(1).toString();
    }
    db.close();
    if(sqlPassword == password&&sqlPassword != NULL)
        return true;
    else
        return false;
}
void MainWindow::initadminLineEdit()
{
    lineEdit_adminName = new My_lineEdit(this);
    lineEdit_adminName->setObjectName("lineEdit_adminName");
    lineEdit_adminName->setGeometry(QRect(590, 160, 291, 41));
    lineEdit_adminName->setStyleSheet("QLineEdit{background-color:transparent}"
                                        "QLineEdit{border-width:0;border-style:outset}");
    syszuxpinyin_adminName =new SyszuxPinyin();
    connect(lineEdit_adminName,SIGNAL(send_show(QString)),this,SLOT(keyboardshow_adminName(QString)));
    connect(syszuxpinyin_adminName,SIGNAL(sendPinyin(QString)),this,SLOT(confirmString_adminName(QString)));

    lineEdit_adminPassword = new My_lineEdit(this);
    lineEdit_adminPassword->setObjectName("lineEdit_adminPassword");
    lineEdit_adminPassword->setGeometry(QRect(590, 215, 291, 41));
    lineEdit_adminPassword->setEchoMode(QLineEdit::Password);
    lineEdit_adminPassword->setStyleSheet("QLineEdit{background-color:transparent}"
                                        "QLineEdit{border-width:0;border-style:outset}");
    syszuxpinyin_adminPassword =new SyszuxPinyin();
    connect(lineEdit_adminPassword,SIGNAL(send_show(QString)),this,SLOT(keyboardshow_adminPassword(QString)));
    connect(syszuxpinyin_adminPassword,SIGNAL(sendPinyin(QString)),this,SLOT(confirmString_adminPassword(QString)));

//    lineEdit_adminName->setText("user");
//    lineEdit_adminPassword->setText("123");
}
void MainWindow::keyboardshow_adminName(QString data)
{
    syszuxpinyin_adminName->lineEdit_window->setText(data);
    syszuxpinyin_adminName->resize(800,330);
    syszuxpinyin_adminName->move(100,280);
    syszuxpinyin_adminName->show();
}
void MainWindow::confirmString_adminName(QString gemfield)
{
    lineEdit_adminName->setText(gemfield);
}
void MainWindow::keyboardshow_adminPassword(QString data)
{
    syszuxpinyin_adminPassword->lineEdit_window->setText(data);
    syszuxpinyin_adminPassword->resize(800,330);
    syszuxpinyin_adminPassword->move(100,280);
    syszuxpinyin_adminPassword->show();
}
void MainWindow::confirmString_adminPassword(QString gemfield)
{
    lineEdit_adminPassword->setText(gemfield);
}
void MainWindow::normalUserShow()
{
    ui->label_background->setStyleSheet("QLabel{border-image: url(:/icon/images/猫眼_03.jpg);}");
    ui->pushButton_ewm->setVisible(false);
    ui->label_ewm->setVisible(false);

    ui->saveBt->setVisible(false);
    ui->saveInfoBt->setVisible(false);
    lineEdit_faceInfo->setVisible(false);
    ui->exitBt->setVisible(false);
    ui->pushButton_autoCheck->setVisible(false);
    ui->pushButton_facesave->setVisible(false);

    ui->textEdit->setVisible(true);
    lineEdit_adminName->setVisible(true);
    lineEdit_adminPassword->setVisible(true);
    ui->loginBt->setVisible(true);
    ui->recognitionBt->setVisible(true);
}
void MainWindow::adminShow()
{
    ui->label_background->setStyleSheet("QLabel{border-image: url(:/icon/images/猫眼_01.jpg);}");
    ui->pushButton_ewm->setVisible(true);
    ui->label_ewm->setVisible(true);

    ui->saveBt->setVisible(true);
    ui->saveInfoBt->setVisible(true);
    lineEdit_faceInfo->setVisible(true);
    ui->exitBt->setVisible(true);
    ui->pushButton_autoCheck->setVisible(true);
    ui->pushButton_facesave->setVisible(true);

    ui->textEdit->setVisible(false);
    lineEdit_adminName->setVisible(false);
    lineEdit_adminPassword->setVisible(false);
    ui->loginBt->setVisible(false);
    ui->recognitionBt->setVisible(false);
}
void MainWindow::btn_setTransparent()
{
    ui->recognitionBt->setStyleSheet("QPushButton{background: transparent;}");
    ui->recognitionBt->setFlat(true);
    ui->loginBt->setStyleSheet("QPushButton{background: transparent;}");
    ui->loginBt->setFlat(true);
    ui->openBt->setStyleSheet("QPushButton{background: transparent;}");
    ui->openBt->setFlat(true);
    ui->closeBt->setStyleSheet("QPushButton{background: transparent;}");
    ui->closeBt->setFlat(true);
    ui->exitBt->setStyleSheet("QPushButton{background: transparent;}");
    ui->exitBt->setFlat(true);
    ui->saveBt->setStyleSheet("QPushButton{background: transparent;}");
    ui->saveBt->setFlat(true);
    ui->pushButton_ewm->setStyleSheet("QPushButton{background: transparent;}");
    ui->pushButton_ewm->setFlat(true);
    ui->pushButton_facesave->setStyleSheet("QPushButton{background: transparent;}");
    ui->pushButton_facesave->setFlat(true);
    ui->pushButton_autoCheck->setStyleSheet("QPushButton{background: transparent;}");
    ui->pushButton_autoCheck->setFlat(true);
}
