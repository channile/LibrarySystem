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

    ui->name_label->setText("");
    ui->name_label->hide();
    ui->UEdit_button->hide();
    ui->logout_button->hide();
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
    query.exec("select password from user where account = '"+str_account+"'");

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
        QMessageBox::critical(NULL, "Error","该用户不存在",QMessageBox::Yes | QMessageBox::No);
        ui->account_edit->clear();
        ui->password_edit->clear();
        return;
    }

    query.exec("select name from user where account = '"
               +str_account+"'");
    if(query.next()){
        User_name = query.value(0).toString();
    }
//    qDebug()<<User_name;

    ACCOUNT = str_account;
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

    ACCOUNT = ADMIN;
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
            ui->name_label->raise();
            ui->name_label->show();
            ui->name_label->setText("您好，" + User_name);
            ui->logout_button->show();
            ui->UEdit_button->show();
        break;
        case ADMIN: //管理员
            ui->account_edit->hide();
            ui->password_edit->hide();
            ui->pushButton->hide();
            ui->label->hide();
            ui->label_2->hide();
            ui->pushButton->hide();
            ui->checkBox->hide();
            updateTablewidget();
            ui->name_label->raise();
            ui->name_label->show();
            ui->name_label->setText("您好，管理员");
            ui->logout_button->show();
        break;
            case VISTOR:
            ui->account_edit->show();
            ui->password_edit->show();
            ui->pushButton->show();
            ui->label->show();
            ui->label_2->show();
            ui->pushButton->show();
            ui->checkBox->show();
            updateTablewidget();
            ui->name_label->hide();
            ui->logout_button->hide();
            ui->UEdit_button->hide();
    }
}

void MainWindow::updateTablewidget(){

    /*Book ( id int primary key , name varchar(50) , public int , input int "
                       ", price double , is_stock int, stock_user varchar(30))*/

    QSqlQuery query_user,query_book;

    if(query_book.exec("select * from Book")){
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);
        ui->tableWidget->setColumnCount(4);

        QStringList header;
        header<<tr("书名")<<tr("出版日期")<<tr("价格")<<tr("是否可借");
        ui->tableWidget->setHorizontalHeaderLabels(header);
        ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

        while (query_book.next()) {
            int rowCount1 = ui->tableWidget->rowCount();
//            qDebug()<<rowCount1;
            ui->tableWidget->insertRow(rowCount1);

//            QString info = query.value(1).toString();
//            qDebug()<<info;

            QTableWidgetItem *book_columnItem0 = new QTableWidgetItem(
                                        query_book.value(1).toString());
            QTableWidgetItem *book_columnItem1 = new QTableWidgetItem(
                                        query_book.value(2).toString());
            QTableWidgetItem *book_columnItem2 = new QTableWidgetItem(
                                        query_book.value(4).toString());

            if(query_book.value(5).toString() == '0'){
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


//    if(query_user.exec("select * from User")){
//        ui->tableWidget_2->clearContents();
//        ui->tableWidget_2->setRowCount(0);

        if(user_type == ADMIN){
            ui->tableWidget_2->setColumnCount(6);
            QStringList header;
            header<<tr("id")<<tr("昵称")<<tr("账号")<<tr("密码")<<tr("已借书籍")<<tr("已借书籍");
            ui->tableWidget_2->setHorizontalHeaderLabels(header);
            ui->tableWidget_2->horizontalHeader()->setStretchLastSection(true);

            query_user.exec("select * from User");

            while (query_user.next()) {
                int rowCount = ui->tableWidget_2->rowCount();

                ui->tableWidget_2->insertRow(rowCount);

                QTableWidgetItem *AUserItem0 = new QTableWidgetItem(
                                            query_user.value(0).toString());
                QTableWidgetItem *AUserItem1 = new QTableWidgetItem(
                                            query_user.value(1).toString());
                QTableWidgetItem *AUserItem2 = new QTableWidgetItem(
                                            query_user.value(2).toString());
                QTableWidgetItem *AUserItem3 = new QTableWidgetItem(
                                            query_user.value(3).toString());

                ui->tableWidget_2->setItem(rowCount,0,AUserItem0);
                ui->tableWidget_2->setItem(rowCount,1,AUserItem1);
                ui->tableWidget_2->setItem(rowCount,2,AUserItem2);
                ui->tableWidget_2->setItem(rowCount,3,AUserItem3);

                QString UserAcc = query_user.value(2).toString();

                int num = 4;

                query_book.exec("select name from Book where stock_user = '"+UserAcc+"'");

                while (query_book.next()) {

                    QTableWidgetItem *bn = new QTableWidgetItem(query_book.value(0).toString());


                    ui->tableWidget_2->setItem(rowCount,num,bn);

                    num++;
                }

//                ui->tableWidget_2->setItem(rowCount,0,AUserItem0);
//                ui->tableWidget_2->setItem(rowCount,1,AUserItem1);
//                ui->tableWidget_2->setItem(rowCount,2,AUserItem2);
//                ui->tableWidget_2->setItem(rowCount,3,AUserItem3);
//                ui->tableWidget_2->setItem(rowCount,4,AUserItem4);

            }
        } else if(user_type == USER){
            ui->tableWidget_2->setColumnCount(5);
            QStringList header;
            header<<tr("id")<<tr("昵称")<<tr("账号")<<tr("已借书籍")<<tr("已借书籍");
            ui->tableWidget_2->setHorizontalHeaderLabels(header);
            ui->tableWidget_2->horizontalHeader()->setStretchLastSection(true);

            query_user.exec("select * from User where account = '"+ACCOUNT+"'");

            while (query_user.next()) {
                int rowCount = ui->tableWidget_2->rowCount();

                ui->tableWidget_2->insertRow(rowCount);

                QTableWidgetItem *AUserItem0 = new QTableWidgetItem(
                                            query_user.value(0).toString());
                QTableWidgetItem *AUserItem1 = new QTableWidgetItem(
                                            query_user.value(1).toString());
                QTableWidgetItem *AUserItem2 = new QTableWidgetItem(
                                            query_user.value(2).toString());

                ui->tableWidget_2->setItem(rowCount,0,AUserItem0);
                ui->tableWidget_2->setItem(rowCount,1,AUserItem1);
                ui->tableWidget_2->setItem(rowCount,2,AUserItem2);

                QString UserAcc = query_user.value(2).toString();

                int i = 3;

                query_book.exec("select name from Book where stock_user = '"+UserAcc+"'");

                while (query_book.next()) {

                    QTableWidgetItem *bn = new QTableWidgetItem(query_book.value(0).toString());

                    ui->tableWidget_2->setItem(rowCount,i,bn);
                    i++;

//                ui->tableWidget_2->setItem(rowCount,0,AUserItem0);
//                ui->tableWidget_2->setItem(rowCount,1,AUserItem1);
//                ui->tableWidget_2->setItem(rowCount,2,AUserItem2);
//                ui->tableWidget_2->setItem(rowCount,3,AUserItem3);

                }

            }
        }else {
            ui->tableWidget_2->clear();
        }

}

void MainWindow::on_check_button_clicked()
{
    QSqlQuery query;
    QString book_name;
    QSqlRecord rec;

    book_name = ui->book_lineEdit->text();

    query.exec("select * from Book where name LIKE '%"+book_name+"%'");
    if(!query.isActive()){
        QMessageBox::critical(NULL, "Error", "该书籍不存在",
                          QMessageBox::Yes);
        return;
    }

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(4);

    QStringList header;
    header<<tr("书名")<<tr("出版日期")<<tr("价格")<<tr("是否可借");
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

    while (query.next()) {
        rec = query.record();

        int num1 = rec.indexOf("name");
        int num2 = rec.indexOf("public");
        int num3 = rec.indexOf("price");
        int num4 = rec.indexOf("is_stock");

        int rowCount = ui->tableWidget->rowCount();
//            qDebug()<<rowCount1;
        ui->tableWidget->insertRow(rowCount);

        QTableWidgetItem *book_columnItem0 = new QTableWidgetItem(
                                    query.value(num1).toString());
        QTableWidgetItem *book_columnItem1 = new QTableWidgetItem(
                                    query.value(num2).toString());
        QTableWidgetItem *book_columnItem2 = new QTableWidgetItem(
                                    query.value(num3).toString());

//        qDebug()<<query.value(num1).toString();

        if(query.value(num4).toString() == '0'){
            QTableWidgetItem *book_columnItem3 = new QTableWidgetItem(tr("是"));

            ui->tableWidget->setItem(rowCount,0,book_columnItem0);
            ui->tableWidget->setItem(rowCount,1,book_columnItem1);
            ui->tableWidget->setItem(rowCount,2,book_columnItem2);
            ui->tableWidget->setItem(rowCount,3,book_columnItem3);
        }else {
            QTableWidgetItem *book_columnItem3 = new QTableWidgetItem(tr("否"));

            ui->tableWidget->setItem(rowCount,0,book_columnItem0);
            ui->tableWidget->setItem(rowCount,1,book_columnItem1);
            ui->tableWidget->setItem(rowCount,2,book_columnItem2);
            ui->tableWidget->setItem(rowCount,3,book_columnItem3);
        }
    }
}

void MainWindow::on_logout_button_clicked()
{
    user_type = VISTOR;
    uiupdate();
    ACCOUNT = "";
    ui->account_edit->clear();
    ui->password_edit->clear();
    ui->checkBox->setCheckState(Qt::Unchecked);
}

void MainWindow::on_refresh_button_clicked()
{
    uiupdate();
    updateTablewidget();
    ui->book_lineEdit->setText("");
}

void MainWindow::on_bor_button_clicked()
{
    QSqlQuery query_user,query_book;
    QString book_name;

    int row;
    int UserStock = 0;

    if(user_type == VISTOR){
        QMessageBox::information(NULL, "提示", "请先登陆",
                          QMessageBox::Yes);
        return;
    } else if (user_type == ADMIN) {
        QMessageBox::information(NULL, "提示", "管理员不要捣乱",
                          QMessageBox::Yes);
        return;
    } else {
        query_user.exec("select stock from User where account = '"+ACCOUNT+"'");
        query_user.next();
        UserStock = query_user.value(0).toInt();
//        qDebug()<<UserStock;
        if(UserStock < 2){

            QList<QTableWidgetItem*> items = ui->tableWidget->selectedItems();
            int count = items.count();
//            qDebug()<<count;
            for(int i = 0; i < count; i++)
            {
                row = ui->tableWidget->row(items.at(i));
//                qDebug()<<row;
                QTableWidgetItem *item = ui->tableWidget->item(row,0);
                book_name = item->text();
//                qDebug()<<book_name;
            }

            qDebug()<<query_book.exec("select is_stock from Book where name = '"+book_name+"'");
            if(query_book.next()){
                qDebug()<<query_book.value(0).toInt();
                if(query_book.value(0).toInt() == 1){
                    QMessageBox::information(NULL, "提示", "该书已被借阅",
                                          QMessageBox::Yes);
                } else{
//                    qDebug()<<query_book.exec("update Book set is_stock = 1"
//                                         ", stock_user = '"+ACCOUNT+"' where name = '"+book_name+"'");
//                    qDebug()<<query_user.exec("update User set stock = stock + 1 where account = '"+ACCOUNT+"'");

                     if(query_book.exec("update Book set is_stock = 1,"
                                           " stock_user = '"+ACCOUNT+"' where name = '"+book_name+"'") &&
                        query_user.exec("update User set stock = stock + 1 where account = '"+ACCOUNT+"'")){
                        QMessageBox::information(NULL, "提示", "借阅成功",
                                              QMessageBox::Yes);
                        updateTablewidget();
                     }
                 }
            }

        }else{
            QMessageBox::information(NULL, "提示", "借阅书籍已达上限",
                                  QMessageBox::Yes);
            return;
        }
    }
}

void MainWindow::on_retire_button_clicked()
{
    int UserStock = 0;
    int row;

    QSqlQuery query_user,query_book;
    QString book_name,user_name;

    if(user_type == VISTOR){
        QMessageBox::information(NULL, "提示", "请先登陆",
                          QMessageBox::Yes);
        return;
    } else if (user_type == ADMIN) {
        QMessageBox::information(NULL, "提示", "管理员不要捣乱",
                          QMessageBox::Yes);
        return;
    } else {
        query_user.exec("select stock from User where account = '"+ACCOUNT+"'");
        query_user.next();
        UserStock = query_user.value(0).toInt();

        if(UserStock == 0){
            QMessageBox::information(NULL, "提示", "没有需要退还的书籍",
                                  QMessageBox::Yes);
//            return;
        } else {
            QList<QTableWidgetItem*> items = ui->tableWidget->selectedItems();
            int count = items.count();

            for(int i = 0; i < count; i++)
            {
                row = ui->tableWidget->row(items.at(i));
//                qDebug()<<row;
                QTableWidgetItem *item = ui->tableWidget->item(row,0);
                book_name = item->text();
//                qDebug()<<book_name;

                query_book.exec("select stock_user from Book where name = '"+book_name+"'");
                if(query_book.next()){
                    user_name = query_book.value(0).toString();
//                    qDebug()<<user_name;
                    if(ACCOUNT == user_name){
                        query_book.exec("update Book set is_stock = 0,"
                                        " stock_user = NULL where stock_user = '"+ACCOUNT+"'"
                                                     " AND name = '"+book_name+"'");
                        query_user.exec("update User set stock = stock - 1 "
                                        "where account = '"+ACCOUNT+"'");
                        QMessageBox::information(NULL, "提示", "书籍退还完成",
                                              QMessageBox::Yes);
                        updateTablewidget();
                    }
                }
            }
        }
    }
}
