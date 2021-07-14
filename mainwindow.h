#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static int status;

signals:
    void sendPrintInfo(QString);

    void startThread(QString);

private slots:
    void on_opne_clicked();

    void on_start_clicked();

    void printEnd();

    void printer(QString);

    void tips(QString);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
