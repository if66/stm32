#ifndef AUTOCHECK_H
#define AUTOCHECK_H

#include <QWidget>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "syszuxpinyin.h"
#include "my_lineedit.h"
namespace Ui {
class AutoCheck;
}

class AutoCheck : public QWidget
{
    Q_OBJECT

public:
    explicit AutoCheck(QWidget *parent = nullptr);
    ~AutoCheck() override;
    static QSqlDatabase initDb();
    static void createTable();
    static void insertInfo(QString info);
private:
    Ui::AutoCheck *ui;
    QTimer *m_clearTimer;
    void initTime();

    void selectFaceInfo();

private slots:
    void on_pushButton_save_clicked();

    void on_pushButton_exit_clicked();
    void on_pushButton_clicked();

protected:
    void showEvent(QShowEvent *event) override;

signals:
    void sendTime(uint startTime,uint stopTime);
};

#endif // AUTOCHECK_H
