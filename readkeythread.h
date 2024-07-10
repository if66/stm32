#ifndef READKEYTHREAD_H
#define READKEYTHREAD_H
#include <QThread>

class ReadKeyThread : public QThread
{
    Q_OBJECT
public:
    ReadKeyThread();
    virtual void run();
signals:
    void keySignal(char);
};

#endif // READKEYTHREAD_H
