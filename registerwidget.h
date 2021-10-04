#ifndef REGISTERWIDGET_H
#define REGISTERWIDGET_H

#include <QWidget>

namespace Ui {
class registerwidget;
}

class registerwidget : public QWidget
{
    Q_OBJECT

public:
    explicit registerwidget(QWidget *parent = nullptr);
    ~registerwidget();

public slots:
    void receiveSignal(QString signal);

private:
    Ui::registerwidget *ui;
};

#endif // REGISTERWIDGET_H
