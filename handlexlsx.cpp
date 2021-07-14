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
    //����Ҫ�򿪵�excel�ļ���filePathΪ�򿪵��ļ���
    QXlsx::Document xlsx(path);
    QStringList sheetLists = xlsx.sheetNames();
    for(auto sheet : sheetLists){
        xlsx.selectSheet(sheet);
        QXlsx::CellRange range = xlsx.dimension();

        int rows = range.lastRow ();
        for (int i = 2;i <= rows; i++) {

            //�ȴ���ӡ��״̬
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

            //���ɴ�ӡ����
            createPrintData();
            QThread::msleep(200);
        }

        MainWindow::status = 1;//����״̬
        emit continues(QString::fromLocal8Bit("'%1'�����ɨ�裬���ȷ����ť������ӡ").arg(sheet));
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
    data.append(QString::fromLocal8Bit("\n\n          ��1�� ��1��\n\n"));
    data.append(QString::fromLocal8Bit("������λ����:δע��Ͱ��ǩ\n"));
    data.append(QString::fromLocal8Bit("���˳���:\n"));
    data.append(QString::fromLocal8Bit("��������:%1\n").arg(rfid));
    data.append(QString::fromLocal8Bit("ë��:%1.%2Kg\n").arg(formatFloatLeft(gross),3,QLatin1Char('0')).arg(formatFloatRight(gross,1)));
    data.append(QString::fromLocal8Bit("Ƥ��:%1.%2Kg\n").arg(formatFloatLeft(tare),3,QLatin1Char('0')).arg(formatFloatRight(tare,1)));
    data.append(QString::fromLocal8Bit("����:%1.%2Kg\n").arg(formatFloatLeft(net),3,QLatin1Char('0')).arg(formatFloatRight(net,1)));
    data.append(QString::fromLocal8Bit("����ʱ��:%1\n\n").arg(time));
    data.append(QString::fromLocal8Bit("���˵�λǩ��:\n\n\n"));
    data.append(QString::fromLocal8Bit("������λǩ��:\n\n\n"));
    data.append(QString::fromLocal8Bit("���й���λǩ��:\n\n\n\n\n"));
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
