#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QMessageBox>
#include <QSettings>
#include <QDebug>
#include "clickablelabel.h"
#include "registerwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ClickableLabel *rLabel = new ClickableLabel("点击注册>>",this);
    QFont font("Microsoft YaHei", 7, 50, true);
    rLabel->setGeometry(QRect(114,340,100,20));
    rLabel->setFont(font);
    rLabel->setCursor(Qt::PointingHandCursor);

    dbCreat();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    str_account = ui->account_edit->text();
    str_password = ui->password_edit->text();

    if(str_account == ""){
        QMessageBox::critical(NULL, "Error", "请先输入账号",
                          QMessageBox::Yes);
        return;
    }

    if(ui->checkBox->isChecked()){
        AdminLogin();
    }else {
        UserLogin();
    }
}

bool MainWindow::dbCreat(){
    if(dbConnect()){
        QSqlDatabase db;
        QSqlQuery query;

        query.exec("create table Book ( id int primary key , name varchar(50) , public int , input int "
                   ", price double , is_stock int, stock_user varchar(30))");
        query.exec("create table User ( id int primary key , name varchar(30), account varchar(30) "
                   ", password varchar(30) , stock int)");
    }
}

bool MainWindow::dbConnect(){

//    QODBC连接
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("2");
    db.setUserName("root");
    db.setPassword("root");

    //访问目录内
//    QSqlDatabase db;
//    if(QSqlDatabase::contains("qt_sql_default_connection")){
//        db = QSqlDatabase::database("qt_sql_default_connection");
//    }else {
//        db = QSqlDatabase::addDatabase("QSQLITE");
//    }
//    db.setDatabaseName("Library.db");


    if(!db.open())
    {
        qDebug() << "数据库创建失败";
        return false;
    }
    return true;
}

void MainWindow::UserLogin(){
    QSqlQuery query;
    query.exec("select password from user where account = '"
                +str_account+"'");

    if(!query.isActive())
    {
        return;
    }

    if(query.next()){
        QString password = query.value(0).toString();
        if(QString::compare(password,str_password)!=0)
        {
            QMessageBox::critical(NULL, "Error", "密码错误",
                                  QMessageBox::Yes);
            ui->password_edit->clear();
            return;
        }
    }
    else
    {
        QMessageBox::critical(NULL, "Error","用户名不存在,是否注册",QMessageBox::Yes | QMessageBox::No);
        ui->account_edit->clear();
        ui->password_edit->clear();
        return;
    }

    user_type=0;
    uiupdate();
}

void MainWindow::AdminLogin(){
    if(str_account == "admin" ){

        if(str_password == "root"){

        }else{
            QMessageBox::critical(NULL, "Error", "密码错误",
                              QMessageBox::Yes);
            return;
        }
    }else{
        QMessageBox::critical(NULL, "Error", "管理员不存在",
                              QMessageBox::Yes);
        return;
    }

    user_type=1;
    uiupdate();
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{

}

void MainWindow::uiupdate(){
    switch (user_type) {
        case 0:  //个人
            ui->account_edit->hide();
            ui->password_edit->hide();
            ui->pushButton->hide();
            ui->label->hide();
            ui->label_2->hide();
            ui->pushButton->hide();
            ui->checkBox->hide();
        case 1: //管理员
            ui->account_edit->hide();
            ui->password_edit->hide();
            ui->pushButton->hide();
            ui->label->hide();
            ui->label_2->hide();
            ui->pushButton->hide();
            ui->checkBox->hide();
    }
}
