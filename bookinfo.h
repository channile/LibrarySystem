#ifndef BOOKINFO_H
#define BOOKINFO_H

#include <QWidget>
#include <QDebug>
#include <QtSql>
#include <QMessageBox>

namespace Ui {
class bookinfo;
}

class bookinfo : public QWidget
{
    Q_OBJECT

public:
    explicit bookinfo(QWidget *parent = nullptr);
    ~bookinfo();

    QString id,acc;

public slots:
    void receiveSignal(QString signal);

private slots:
    void on_pushButton_clicked();

private:
    Ui::bookinfo *ui;
};

#endif // BOOKINFO_H
