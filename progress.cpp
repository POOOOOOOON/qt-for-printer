#include "progress.h"
#include "ui_progress.h"

Progress::Progress(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Progress)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
}

Progress::~Progress()
{
    delete ui;
}

void Progress::setValue(int value)
{
    ui->progressBar->setValue(value);
}
