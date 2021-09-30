#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
#include "registerwidget.h"
#include "clickablelabel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString str_account,str_password;
    int maxid;
    int user_type;

private slots:
    void on_pushButton_clicked();

    void on_checkBox_stateChanged(int arg1);

    bool dbCreat();

    bool dbConnect();

    void UserLogin();

    void AdminLogin();

    void uiupdate();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
