#include "com.h"
#include "ui_com.h"

#include <QDebug>
#include <QSerialPortInfo>
#include <QMessageBox>

int Com::cts = 0;

Com::Com(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Com)
{
    ui->setupUi(this);
    this->setWindowTitle("打开串口");
    serial = new QSerialPort;

    //检测可用端口
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
        ui->comValue->addItem(info.portName());

}

Com::~Com()
{
    serial->close();
    delete serial;
    delete ui;
}

void Com::on_open_clicked()
{
    //设置串口名
    serial->setPortName(ui->comValue->currentText());
    //设置波特率
    serial->setBaudRate(QSerialPort::Baud9600);
    //设置数据位数
    serial->setDataBits(QSerialPort::Data8);
    //设置奇偶校验
    serial->setParity(QSerialPort::NoParity);
    //设置停止位
    serial->setStopBits(QSerialPort::OneStop);
    //设置流控制
    serial->setFlowControl(QSerialPort::HardwareControl);

    if(serial->open(QIODevice::ReadWrite) == false){
        QMessageBox::about(NULL, tr("提示"), "打开失败");
        return;
    }

    //初始化打印机参数
    initPrinter();

    w = new MainWindow;
    connect(w,SIGNAL(sendPrintInfo(QString)),this,SLOT(receivePrintInfo(QString)));
    this->hide();
    w->show();
}

void Com::receivePrintInfo(QString data)
{
    pin = serial->pinoutSignals();
    if(QSerialPort::ClearToSendSignal&pin)
        cts = 0;//打印机空闲状态
    else
        cts = 1;//打印机忙碌状态

    //打印
    if(!cts && data.size() > 0)
        serial->write(data.toLocal8Bit());
}

void Com::initPrinter()
{
    QByteArray data;

    //设置行间距
    data.resize(3);
    data[0] = 0x1B;
    data[1] = 0x31;
    data[2] = 16;
    serial->write(data);
}
