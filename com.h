#ifndef COM_H
#define COM_H

#include "mainwindow.h"
#include <QDialog>
#include <QSerialPort>

namespace Ui {
class Com;
}

class Com : public QDialog
{
    Q_OBJECT

public:
    explicit Com(QWidget *parent = nullptr);
    ~Com();

    static int cts;

private slots:
    void on_open_clicked();
    void receivePrintInfo(QString);

private:
    Ui::Com *ui;
    MainWindow *w;
    QSerialPort *serial;
    QSerialPort::PinoutSignals pin;

    void initPrinter();
};

#endif // COM_H
