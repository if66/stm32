#include "autocheck.h"
#include "ui_autocheck.h"
#include <QDateTime>
#include <QMessageBox>
#include <QTimer>
AutoCheck::AutoCheck(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AutoCheck)
{
    ui->setupUi(this);
    //初始化定时器
    m_clearTimer = new QTimer(this);
    m_clearTimer->setInterval(1000);
    initTime();
    selectFaceInfo();
    ui->pushButton_save->setStyleSheet("QPushButton{background: transparent;}");
    ui->pushButton_save->setFlat(true);
    ui->pushButton_exit->setStyleSheet("QPushButton{background: transparent;}");
    ui->pushButton_exit->setFlat(true);

}

AutoCheck::~AutoCheck()
{
    delete ui;
}
void AutoCheck::initTime()
{
    ui->timeEdit->setDisplayFormat("HH:mm:ss");
    ui->timeEdit->setDateTime(QDateTime::currentDateTime());
    ui->timeEdit_stop->setDisplayFormat("HH:mm:ss");
    ui->timeEdit_stop->setDateTime(QDateTime::currentDateTime());
}

void AutoCheck::on_pushButton_save_clicked()
{
    selectFaceInfo();
    QMessageBox::information(nullptr, tr("设置"), "设置成功");
    qDebug()<<ui->timeEdit->dateTime().toTime_t();
    qDebug()<<ui->timeEdit_stop->dateTime().toTime_t();
    emit sendTime(ui->timeEdit->dateTime().toTime_t(),ui->timeEdit_stop->dateTime().toTime_t());
}

//启动定时器
void AutoCheck::showEvent(QShowEvent *event){
    QWidget::showEvent(event);
    m_clearTimer->start();
}

QSqlDatabase AutoCheck::initDb()
{
    QSqlDatabase db;
//   if(QSqlDatabase::contains("qt_sql_default_connection")){
//       db = QSqlDatabase::database("qt_sql_default_connection");
//   }
//   else {
       db = QSqlDatabase::addDatabase("QSQLITE");
//   }
    db.setDatabaseName("alarmHistory.db");
    if(!db.open()){
        QMessageBox::warning(nullptr, tr("Warning"), db.lastError().text());
        return QSqlDatabase();
    }
    else
        return db;
}

void AutoCheck::createTable()
{
    QSqlDatabase db = initDb();
    //创建一个数据库操作对象
    QSqlQuery sq(db);
    QString sql = "CREATE TABLE IF NOT EXISTS alarmHistory(info TEXT);";
    if(sq.exec(sql))
        qDebug() << "autocheck CREATE TABLE success";
    else
        qDebug() << sq.lastError().text();
    db.close();
}
void AutoCheck::insertInfo(QString info)
{
    QSqlDatabase db = initDb();
    QSqlQuery sq(db);
    //防止sql注入问题
    QString sql = "INSERT INTO alarmHistory VALUES(?)";
    //预处理
    sq.prepare(sql);
    //占位符数据替换
    sq.addBindValue(info);
    //执行sql语句
    if(sq.exec())
    {
       qDebug()<<"success";
    }
    else
    {
        QString errorMsg = sq.lastError().text();
        qDebug()<<"errorMsg:"<<errorMsg;
    }
    db.close();
}

void AutoCheck::selectFaceInfo()
{
    ui->textEdit->clear();
    QString alarmHistoryInformation;

    QSqlDatabase db = initDb();
    QSqlQuery query(db);
    //防止sql注入问题
    QString sql = "select * from alarmHistory;";
    //预处理
    query.prepare(sql);
    if(!query.exec())
    {
        db.close();
        return;
    }
    while(query.next())
    {
        alarmHistoryInformation =   query.value(0).toString();
        qDebug()<<alarmHistoryInformation;
        ui->textEdit->append(alarmHistoryInformation);
    }
    db.close();
}

void AutoCheck::on_pushButton_clicked()
{
    QSqlDatabase db = initDb();
    QSqlQuery sq(db);
    QString sql = "DELETE FROM alarmHistory;";//删除数据库的数据
    if(sq.exec(sql)){
        qDebug()<<"clearDatabase success";
    }else{
        qDebug()<<sq.lastError().text();
    }
    db.close();
    ui->textEdit->clear();
}

void AutoCheck::on_pushButton_exit_clicked()
{
    close();
}


