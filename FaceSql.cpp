#include "mainwindow.h"
#include "ui_mainwindow.h"
QSqlDatabase MainWindow::initDb()
{
     QSqlDatabase db;
//    if(QSqlDatabase::contains("qt_sql_default_connection")){
//        db = QSqlDatabase::database("qt_sql_default_connection");
//    }
//    else {
        db = QSqlDatabase::addDatabase("QSQLITE");
//    }
    db.setDatabaseName("faceInfo.db");
    if(!db.open()){
        QMessageBox::warning(nullptr, tr("Warning"), db.lastError().text());
        return QSqlDatabase();
    }
    else
        return db;
}
void MainWindow::createTable()
{
    QSqlDatabase db = initDb();
    //创建一个数据库操作对象
    QSqlQuery sq(db);
    QString sql = "CREATE TABLE IF NOT EXISTS faceInfo(id int PRIMARY KEY,name TEXT);";
    if(sq.exec(sql))
        qDebug() << "face CREATE TABLE success";
    else
        qDebug() << sq.lastError().text();
    db.close();
}
void MainWindow::insertFaceInfo(int id)
{
    QSqlDatabase db = initDb();
    QSqlQuery sq(db);
    //防止sql注入问题
    QString sql = "INSERT INTO faceInfo VALUES(?,?)";
    //预处理
    sq.prepare(sql);
    //占位符数据替换
    sq.addBindValue(id);
    sq.addBindValue(faceInfo);
    //执行sql语句
    if(sq.exec())
    {
       qDebug()<<"face success";
    }
    else
    {
        QString errorMsg = sq.lastError().text();
        qDebug()<<"errorMsg:"<<errorMsg;
    }
    db.close();
}
QString MainWindow::selectFaceInfo(int id)
{
    QString faceInformation;
    QString iDString;

    QSqlDatabase db = initDb();
    QSqlQuery query(db);
    //防止sql注入问题
    QString sql = "select * from faceInfo where id=?;";
    //预处理
    query.prepare(sql);
    //占位符数据替换
    query.addBindValue(id);

    if(!query.exec())
    {
        db.close();
        return "error";
    }
    while(query.next())
    {
        iDString =   query.value(0).toString();
        faceInformation =   query.value(1).toString();

    }
    db.close();
    return faceInformation;
}
