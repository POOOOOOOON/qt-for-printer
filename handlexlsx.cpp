#include "com.h"
#include "mainwindow.h"
#include "handlexlsx.h"
#include "xlsxdocument.h"

#include <QDateTime>

HandleXlsx::HandleXlsx(QObject *parent) : QObject(parent)
{
}

void HandleXlsx::run(QString path)
{
    //打开需要打开的excel文件，filePath为打开的文件名
    QXlsx::Document xlsx(path);
    QStringList sheetLists = xlsx.sheetNames();
    for(auto sheet : sheetLists){
        xlsx.selectSheet(sheet);
        QXlsx::CellRange range = xlsx.dimension();

        int rows = range.lastRow ();
        for (int i = 2;i <= rows; i++) {

            //等待打印机状态
            while (Com::cts) {
                QThread::msleep(500);
                emit send("");
            }
        
            if (xlsx.cellAt(i, 5) != nullptr){
                gross = xlsx.cellAt(i, 5)->value().toString().trimmed();
            }

            if (xlsx.cellAt(i, 6) != nullptr){
                tare = xlsx.cellAt(i, 6)->value().toString().trimmed();
            }

            if (xlsx.cellAt(i, 7) != nullptr){
                net = xlsx.cellAt(i, 7)->value().toString().trimmed();
            }

            if (xlsx.cellAt(i, 8) != nullptr){
                rfid = xlsx.cellAt(i, 8)->value().toString().trimmed();
            }

            if (xlsx.cellAt(i, 9) != nullptr){
                time = xlsx.cellAt(i, 9)->dateTime().toString("yyyy-MM-dd hh:mm:ss");
            }

            //生成打印数据
            createPrintData();
            QThread::msleep(200);
        }

        MainWindow::status = 1;//待定状态
        emit continues(QString::fromLocal8Bit("'%1'已完成扫描，点击确定按钮继续打印").arg(sheet));
        while(1){
            QThread::sleep(1);
            if(MainWindow::status == 0)
                break;
            else if(MainWindow::status == 1)
                continue;
        }
    }

    emit complete();
}

void HandleXlsx::createPrintData()
{
    QString data;
    data.append(QString::fromLocal8Bit("\n\n          第1联 共1联\n\n"));
    data.append(QString::fromLocal8Bit("产生单位名称:未注册桶标签\n"));
    data.append(QString::fromLocal8Bit("收运车辆:\n"));
    data.append(QString::fromLocal8Bit("容器卡号:%1\n").arg(rfid));
    data.append(QString::fromLocal8Bit("毛重:%1.%2Kg\n").arg(formatFloatLeft(gross),3,QLatin1Char('0')).arg(formatFloatRight(gross,1)));
    data.append(QString::fromLocal8Bit("皮重:%1.%2Kg\n").arg(formatFloatLeft(tare),3,QLatin1Char('0')).arg(formatFloatRight(tare,1)));
    data.append(QString::fromLocal8Bit("净重:%1.%2Kg\n").arg(formatFloatLeft(net),3,QLatin1Char('0')).arg(formatFloatRight(net,1)));
    data.append(QString::fromLocal8Bit("收运时间:%1\n\n").arg(time));
    data.append(QString::fromLocal8Bit("收运单位签字:\n\n\n"));
    data.append(QString::fromLocal8Bit("产生单位签字:\n\n\n"));
    data.append(QString::fromLocal8Bit("城市管理单位签字:\n\n\n\n\n"));
    data.append("--------------------------------\n\n");
    qDebug()<<data;
    emit send(data);
}

QString HandleXlsx::formatFloatLeft(QString value)
{
    int pos = value.indexOf(".");
        return value.left(pos);
}

QString HandleXlsx::formatFloatRight(QString value,int fixed)
{
    int pos = value.indexOf(".");
    if(pos != -1){
        return value.mid(pos+1,fixed);
    }else {
        return "0";
    }
}
