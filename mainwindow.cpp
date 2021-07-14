#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "progress.h"
#include "handlexlsx.h"

#include <QFileDialog>
#include <QMessageBox>

int MainWindow::status = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("打印");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_opne_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,tr("打开"),NULL,tr("文件(*xls *xlsx)"));
    ui->path->setText(filePath);
}

void MainWindow::on_start_clicked()
{
    if(ui->path->text().length() == 0)
        return;

    ui->start->setEnabled(false);

    HandleXlsx* worker = new HandleXlsx();
    QThread* thread = new QThread();

    connect(this,SIGNAL(startThread(QString)),worker,SLOT(run(QString)));
    connect(worker,SIGNAL(complete()),worker,SLOT(deleteLater()));
    connect(worker,SIGNAL(destroyed(QObject*)),thread,SLOT(quit()));
    connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));
    connect(worker,SIGNAL(send(QString)),this,SLOT(printer(QString)));
    connect(worker,SIGNAL(complete()),this,SLOT(printEnd()));
    connect(worker,SIGNAL(continues(QString)),this,SLOT(tips(QString)));

    worker->moveToThread(thread);

    thread->start();
    this->startThread(ui->path->text());
}

void MainWindow::printer(QString data)
{
    emit sendPrintInfo(data);
}

void MainWindow::printEnd()
{
    ui->start->setEnabled(true);
}

void MainWindow::tips(QString text)
{
//    QMessageBox tips(QMessageBox::NoIcon,"提示",text,QMessageBox::Yes, NULL);
//    int result = tips.exec();
//    if(result == QMessageBox::Yes){
//        MainWindow::status = 0;
//    }else{
//        MainWindow::status = -1;
//    }
    QMessageBox tips;
    tips.setWindowTitle("提示");
    tips.setText(text);
    tips.exec();
    MainWindow::status = 0;
}
