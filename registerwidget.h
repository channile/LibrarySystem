#ifndef REGISTERWIDGET_H
#define REGISTERWIDGET_H

#include <QWidget>
#include <QMainWindow>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QString>
#include <QSqlError>
#include <QDateTime>
#include <QDate>
#include <QTimer>
#include <QSqlRecord>
#include <QSqlQuery>

namespace Ui {
class registerwidget;
}

class registerwidget : public QWidget
{
    Q_OBJECT

public:
    explicit registerwidget(QWidget *parent = nullptr);
    ~registerwidget();

    int UTYPE,state;
    enum{ADMIN,USER,SUCCESS,ERROR};

public slots:
    void receiveSignal(QString signal);

private slots:
    void on_pushButton_clicked();

signals:
    void updatetw();

private:
    Ui::registerwidget *ui;
};

#endif // REGISTERWIDGET_H
