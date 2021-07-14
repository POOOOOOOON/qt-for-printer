#include <QApplication>
//#include <QTextCodec>
#include "com.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    QTextCodec *codec = QTextCodec::codecForName("GBK");
//    QTextCodec::setCodecForLocale(codec);

    Com com;
    com.show();

    return a.exec();
}
