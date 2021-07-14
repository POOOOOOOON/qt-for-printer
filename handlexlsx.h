#ifndef HANDLEXLSX_H
#define HANDLEXLSX_H

#include <QObject>
#include <QDebug>
#include <QThread>

class HandleXlsx : public QObject
{
    Q_OBJECT
public:
    explicit HandleXlsx(QObject *parent = nullptr);

signals:
    void complete();
    void send(QString);
    void continues(QString);
public slots:
    void run(QString);//ºÄÊ±²Ù×÷
private:
    QString gross;
    QString tare;
    QString net;
    QString rfid;
    QString time;

    void createPrintData();
    QString formatFloatLeft(QString);
    QString formatFloatRight(QString,int);
};

#endif // HANDLEXLSX_H
