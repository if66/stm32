#ifndef APPROACH_H
#define APPROACH_H
#include <QThread>
#include <stdio.h>
#include <stdlib.h>
class Approach : public QThread
{
    Q_OBJECT
public:
    Approach();
    virtual void run();
signals:
    void sendApproachAD(QString ad);
};

#endif // APPROACH_H
