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

    QString str_account,str_password,User_name,ACCOUNT;
    int maxid;
    int user_type;
    enum{USER,ADMIN,VISTOR};
//    ClickableLabel *rLabel;
    QSqlDatabase db;

private slots:
    void on_pushButton_clicked();

    void on_checkBox_stateChanged(int arg1);

    bool dbCreat();

    bool dbConnect();

    void UserLogin();

    void AdminLogin();

    void uiupdate();

    void updateTablewidget();

    void on_check_button_clicked();

    void on_logout_button_clicked();

    void on_refresh_button_clicked();

    void on_bor_button_clicked();

    void on_retire_button_clicked();

    void on_check_button_2_clicked();

    void on_add_button_2_clicked();

    void on_edit_button_2_clicked();

    void on_UEdit_button_clicked();

    void on_add_button_clicked();

    void on_edit_button_clicked();

signals:
    void sendSignal(QString);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
