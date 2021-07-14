#ifndef PROGRESS_H
#define PROGRESS_H

#include <QDialog>

namespace Ui {
class Progress;
}

class Progress : public QDialog
{
    Q_OBJECT

public:
    explicit Progress(QWidget *parent = nullptr);
    ~Progress();

    void setValue(int value);

private:
    Ui::Progress *ui;
};

#endif // PROGRESS_H
