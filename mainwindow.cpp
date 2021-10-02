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
    rLabel->setGeometry(QRect(110,640,100,20));
    rLabel->setFont(font);
    rLabel->setCursor(Qt::PointingHandCursor);

    dbCreat();
    updateTablewidget();

    user_type = VISTOR;
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
//        qDebug()<<password;
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

    user_type=USER;
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

    user_type=ADMIN;
    uiupdate();
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{

}

void MainWindow::uiupdate(){
    switch (user_type) {
        case USER:  //个人
            ui->account_edit->hide();
            ui->password_edit->hide();
            ui->pushButton->hide();
            ui->label->hide();
            ui->label_2->hide();
            ui->pushButton->hide();
            ui->checkBox->hide();
            updateTablewidget();
        case ADMIN: //管理员
            ui->account_edit->hide();
            ui->password_edit->hide();
            ui->pushButton->hide();
            ui->label->hide();
            ui->label_2->hide();
            ui->pushButton->hide();
            ui->checkBox->hide();
            updateTablewidget();
    }
}

void MainWindow::updateTablewidget(){

    /*Book ( id int primary key , name varchar(50) , public int , input int "
                       ", price double , is_stock int, stock_user varchar(30))*/

    QSqlQuery query;
    if(query.exec("select * from Book")){
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);
        ui->tableWidget->setColumnCount(4);

        QStringList header;
        header<<tr("书名")<<tr("出版日期")<<tr("价格")<<tr("是否可借");
        ui->tableWidget->setHorizontalHeaderLabels(header);
        ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

        while (query.next()) {
            int rowCount1 = ui->tableWidget->rowCount();
//            qDebug()<<rowCount1;
            ui->tableWidget->insertRow(rowCount1);

//            QString info = query.value(1).toString();
//            qDebug()<<info;

            QTableWidgetItem *book_columnItem0 = new QTableWidgetItem(
                                        query.value(1).toString());
            QTableWidgetItem *book_columnItem1 = new QTableWidgetItem(
                                        query.value(2).toString());
            QTableWidgetItem *book_columnItem2 = new QTableWidgetItem(
                                        query.value(4).toString());

            if(query.value(5).toString() == '0'){
                QTableWidgetItem *book_columnItem3 = new QTableWidgetItem(tr("是"));

                ui->tableWidget->setItem(rowCount1,0,book_columnItem0);
                ui->tableWidget->setItem(rowCount1,1,book_columnItem1);
                ui->tableWidget->setItem(rowCount1,2,book_columnItem2);
                ui->tableWidget->setItem(rowCount1,3,book_columnItem3);
            }else {
                QTableWidgetItem *book_columnItem3 = new QTableWidgetItem(tr("否"));

                ui->tableWidget->setItem(rowCount1,0,book_columnItem0);
                ui->tableWidget->setItem(rowCount1,1,book_columnItem1);
                ui->tableWidget->setItem(rowCount1,2,book_columnItem2);
                ui->tableWidget->setItem(rowCount1,3,book_columnItem3);
            }


        }
    }

    /*User ( id int primary key , name varchar(30), account varchar(30) "
                       ", password varchar(30) , stock int)*/

    if(query.exec("select * from User")){
        ui->tableWidget_2->clearContents();
        ui->tableWidget_2->setRowCount(0);
        if(user_type == ADMIN){
            ui->tableWidget_2->setColumnCount(5);
            QStringList header;
            header<<tr("id")<<tr("昵称")<<tr("账号")<<tr("密码")<<tr("已借书籍");
            ui->tableWidget_2->setHorizontalHeaderLabels(header);
            ui->tableWidget_2->horizontalHeader()->setStretchLastSection(true);

            while (query.next()) {
                int rowCount = ui->tableWidget_2->rowCount();

                ui->tableWidget_2->insertRow(rowCount);

                QTableWidgetItem *AUserItem0 = new QTableWidgetItem(
                                            query.value(0).toString());
                QTableWidgetItem *AUserItem1 = new QTableWidgetItem(
                                            query.value(1).toString());
                QTableWidgetItem *AUserItem2 = new QTableWidgetItem(
                                            query.value(2).toString());
                QTableWidgetItem *AUserItem3 = new QTableWidgetItem(
                                            query.value(3).toString());
                QTableWidgetItem *AUserItem4 = new QTableWidgetItem(
                                            query.value(4).toString());

                ui->tableWidget_2->setItem(rowCount,0,AUserItem0);
                ui->tableWidget_2->setItem(rowCount,1,AUserItem1);
                ui->tableWidget_2->setItem(rowCount,2,AUserItem2);
                ui->tableWidget_2->setItem(rowCount,3,AUserItem3);
                ui->tableWidget_2->setItem(rowCount,4,AUserItem4);
            }
        } else {
            ui->tableWidget_2->setColumnCount(4);
            QStringList header;
            header<<tr("id")<<tr("昵称")<<tr("账号")<<tr("已借书籍");
            ui->tableWidget_2->setHorizontalHeaderLabels(header);
            ui->tableWidget_2->horizontalHeader()->setStretchLastSection(true);

            while (query.next()) {
                int rowCount = ui->tableWidget_2->rowCount();

                ui->tableWidget_2->insertRow(rowCount);

                QTableWidgetItem *AUserItem0 = new QTableWidgetItem(
                                            query.value(0).toString());
                QTableWidgetItem *AUserItem1 = new QTableWidgetItem(
                                            query.value(1).toString());
                QTableWidgetItem *AUserItem2 = new QTableWidgetItem(
                                            query.value(2).toString());
                QTableWidgetItem *AUserItem3 = new QTableWidgetItem(
                                            query.value(4).toString());

                ui->tableWidget_2->setItem(rowCount,0,AUserItem0);
                ui->tableWidget_2->setItem(rowCount,1,AUserItem1);
                ui->tableWidget_2->setItem(rowCount,2,AUserItem2);
                ui->tableWidget_2->setItem(rowCount,3,AUserItem3);
            }
        }
    }

}

void MainWindow::on_check_button_clicked()
{

}
