#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QMessageBox>
#include <QSettings>
#include <QDebug>
#include "clickablelabel.h"
#include "registerwidget.h"
#include "bookinfo.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ClickableLabel *rLabel = new ClickableLabel("点击注册>>",this);
    QFont font("Microsoft YaHei", 7, 50, true);
    rLabel->setGeometry(QRect(110,580,100,20));
    rLabel->setFont(font);
    rLabel->setCursor(Qt::PointingHandCursor);

    dbCreat();
    updateTablewidget();

    user_type = VISTOR;

    ui->name_label->setText("");
    ui->name_label->hide();
    ui->UEdit_button->hide();
    ui->logout_button->hide();
    ui->check_button_2->hide();
    ui->add_button_2->hide();
    ui->del_button_2->hide();
    ui->edit_button_2->hide();
    ui->lineEdit->hide();

    ui->account_edit->setAttribute(Qt::WA_InputMethodEnabled,false);
    ui->password_edit->setAttribute(Qt::WA_InputMethodEnabled,false);

    ui->account_edit->setMaxLength(10);
    ui->password_edit->setMaxLength(15);

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_2->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->sqlite_checkBox->setCheckState(Qt::Checked);
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
        QMessageBox::information(NULL, "提示", "请先输入账号",
                          QMessageBox::Yes);
        return;
    } else if (str_password == "") {
        QMessageBox::information(NULL, "提示", "请先输入密码",
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
//        qDebug()<<true;
        QSqlQuery query;

        query.exec("create table Book ( id int primary key , name varchar(50) , public int , input int "
                   ", price double , is_stock int, stock_user varchar(10))");
        query.exec("create table User ( id int primary key , name varchar(20), account varchar(10) "
                   ", password varchar(15) , stock int)");

    }
}

bool MainWindow::dbConnect(){

    if(ui->Mysql_checkBox->isChecked()){  //    QODBC连接
//        QSqlDatabase::removeDatabase("qt_sql_default_connection");
//        qDebug()<<1;

        db = QSqlDatabase::addDatabase("QODBC");
        db.setHostName("127.0.0.1");
        db.setPort(3306);
        db.setDatabaseName("2"); //改成ODBC的名称
        db.setUserName("root");
        db.setPassword("root");

    }else {   //访问目录内
//        QSqlDatabase::removeDatabase("qt_sql_default_connection");

        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(QApplication::applicationDirPath() +"library.db");

    }

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
            QMessageBox::information(NULL, "提示", "密码错误",
                                  QMessageBox::Yes);
            ui->password_edit->clear();
            return;
        }
    }
    else
    {
        QMessageBox::information(NULL, "提示","该用户不存在",QMessageBox::Yes);
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
            QMessageBox::information(NULL, "提示", "密码错误",
                              QMessageBox::Yes);
            return;
        }
    }else{
        QMessageBox::information(NULL, "提示", "管理员不存在",
                              QMessageBox::Yes);
        return;
    }

    ACCOUNT = ADMIN;
    user_type=ADMIN;
    uiupdate();
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
            ui->check_button_2->hide();
            ui->add_button_2->hide();
            ui->del_button_2->hide();
            ui->edit_button_2->hide();
            ui->lineEdit->hide();
            updateTablewidget();
            ui->name_label->raise();
            ui->name_label->show();
            ui->name_label->setText("您好，" + User_name);
            ui->logout_button->show();
            ui->UEdit_button->show();
            ui->account_edit->clear();
            ui->password_edit->clear();
        break;
        case ADMIN: //管理员
            ui->account_edit->hide();
            ui->password_edit->hide();
            ui->pushButton->hide();
            ui->label->hide();
            ui->label_2->hide();
            ui->pushButton->hide();
            ui->checkBox->hide();
            ui->check_button_2->show();
            ui->add_button_2->show();
            ui->del_button_2->show();
            ui->edit_button_2->show();
            ui->lineEdit->show();
            updateTablewidget();
            ui->name_label->raise();
            ui->name_label->show();
            ui->name_label->setText("您好，管理员");
            ui->logout_button->show();
            ui->account_edit->clear();
            ui->password_edit->clear();
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
            ui->check_button_2->hide();
            ui->add_button_2->hide();
            ui->del_button_2->hide();
            ui->edit_button_2->hide();
            ui->lineEdit->hide();
            ui->account_edit->clear();
            ui->password_edit->clear();
    }
}

void MainWindow::updateTablewidget(){

    ui->tableWidget->clear();
    ui->tableWidget_2->clear();

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
            ui->tableWidget_2->clearContents();
            ui->tableWidget_2->setRowCount(0);
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
            ui->tableWidget_2->clearContents();
            ui->tableWidget_2->setRowCount(0);
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

    if(book_name == ""){
        QMessageBox::information(NULL, "提示", "请先输入",
                          QMessageBox::Yes);
        return;
    }

    query.exec("select * from Book where name LIKE '%"+book_name+"%'"); //bool
    if(!query.isActive()){
        QMessageBox::information(NULL, "提示", "该书籍不存在",
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

    //判断用户类型
    if(user_type == VISTOR){
        QMessageBox::information(NULL, "提示", "请先登陆",
                          QMessageBox::Yes);
        return;
    } else if (user_type == ADMIN) {
        QMessageBox::information(NULL, "提示", "管理员不要捣乱",
                          QMessageBox::Yes);
        return;
    } else {
        // 获取用户的已借书籍数量
        query_user.exec("select stock from User where account = '"+ACCOUNT+"'");
        query_user.next();
        UserStock = query_user.value(0).toInt();
//        qDebug()<<UserStock;
        if(UserStock < 2){  //每个用户最多借两本书

            //获取Qtablewidget选中的Item
            QList<QTableWidgetItem*> items = ui->tableWidget->selectedItems();
            //Item可能有多个，判断
            int count = items.count();
//            qDebug()<<count;
            for(int i = 0; i < count; i++)
            {
                //获取选中行
                row = ui->tableWidget->row(items.at(i));
//                qDebug()<<row;
                //获取选中行的第一列 书名
                QTableWidgetItem *item = ui->tableWidget->item(row,0);
                book_name = item->text();
//                qDebug()<<book_name;
            }

            //查找BOOK中书为上面选中的书名，获取is_stock
            query_book.exec("select is_stock from Book where name = '"+book_name+"'");
            if(query_book.next()){
                query_book.value(0).toInt();
                if(query_book.value(0).toInt() == 1){ //如果is_stock = 1
                    QMessageBox::information(NULL, "提示", "该书已被借阅",
                                          QMessageBox::Yes);
                } else{
//                    qDebug()<<query_book.exec("update Book set is_stock = 1"
//                                         ", stock_user = '"+ACCOUNT+"' where name = '"+book_name+"'");
//                    qDebug()<<query_user.exec("update User set stock = stock + 1 where account = '"+ACCOUNT+"'");

                    //更新书籍 用户
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

void MainWindow::on_check_button_2_clicked()
{
    QSqlQuery query_user,query_book;
    QString editText;
    QSqlRecord rec;

    editText = ui->lineEdit->text();

    if(editText == ""){
        QMessageBox::information(NULL, "提示", "请先输入",
                          QMessageBox::Yes);
        return;
    }

    query_user.exec("select * from User where name LIKE '%"+editText+"%'"
                             "OR account LIKE '%"+editText+"%'"); //bool
    if(!query_user.isActive()){
        QMessageBox::information(NULL, "提示", "该用户不存在",
                          QMessageBox::Yes);
        return;
    }

    ui->tableWidget_2->clearContents();
    ui->tableWidget_2->setRowCount(0);
    ui->tableWidget_2->setColumnCount(6);

    QStringList header;
    header<<tr("id")<<tr("昵称")<<tr("账号")<<tr("密码")<<tr("已借书籍")<<tr("已借书籍");
    ui->tableWidget_2->setHorizontalHeaderLabels(header);
    ui->tableWidget_2->horizontalHeader()->setStretchLastSection(true);

    while (query_user.next()) {
        int rowCount = ui->tableWidget_2->rowCount();

        ui->tableWidget_2->insertRow(rowCount);

        rec = query_user.record();

        int c1 = rec.indexOf("id");
        int c2 = rec.indexOf("name");
        int c3 = rec.indexOf("account");
        int c4 = rec.indexOf("password");


        QTableWidgetItem *AUserItem0 = new QTableWidgetItem(
                                    query_user.value(c1).toString());
        QTableWidgetItem *AUserItem1 = new QTableWidgetItem(
                                    query_user.value(c2).toString());
        QTableWidgetItem *AUserItem2 = new QTableWidgetItem(
                                    query_user.value(c3).toString());
        QTableWidgetItem *AUserItem3 = new QTableWidgetItem(
                                    query_user.value(c4).toString());

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
    }
}

void MainWindow::on_add_button_2_clicked()
{
    registerwidget *res = new registerwidget();
    connect(this,SIGNAL(sendSignal(QString)),res,SLOT(receiveSignal(QString)));
    connect(res,SIGNAL(updatetw()),this,SLOT(updateTablewidget()));
    emit sendSignal("ADD");
    res->show();
}

void MainWindow::on_edit_button_2_clicked()
{
    registerwidget *res = new registerwidget();
    connect(this,SIGNAL(sendSignal(QString)),res,SLOT(receiveSignal(QString)));
    connect(res,SIGNAL(updatetw()),this,SLOT(updateTablewidget()));
    emit sendSignal("AEDIT"); //管理员修改信号
    res->show();
}

void MainWindow::on_UEdit_button_clicked()
{
    registerwidget *res = new registerwidget();
    connect(this,SIGNAL(sendSignal(QString)),res,SLOT(receiveSignal(QString)));
    connect(res,SIGNAL(updatetw()),this,SLOT(updateTablewidget()));
    emit sendSignal(ACCOUNT); //用户修改信号
    res->show();
}

void MainWindow::on_add_button_clicked()
{
    if(user_type == ADMIN){
        bookinfo *bif = new bookinfo();
        connect(this,SIGNAL(sendSignal(QString)),bif,SLOT(receiveSignal(QString)));
        connect(bif,SIGNAL(updatetw()),this,SLOT(updateTablewidget()));
        emit sendSignal("ADD");
        bif->show();
    }else {
        QMessageBox::information(NULL, "提示", "非管理员无权操作",QMessageBox::Yes);
        return;
    }

}

void MainWindow::on_edit_button_clicked()
{
    if(user_type == ADMIN){
        bookinfo *bif = new bookinfo();
        connect(this,SIGNAL(sendSignal(QString)),bif,SLOT(receiveSignal(QString)));
        connect(bif,SIGNAL(updatetw()),this,SLOT(updateTablewidget()));

        int row;
        QString bookid,bookname;
        QSqlQuery query;

        QList<QTableWidgetItem*> items = ui->tableWidget->selectedItems();
        int count = items.count();
        for(int i = 0; i < count; i++)
        {
            //获取选中行
            row = ui->tableWidget->row(items.at(i));
    //                qDebug()<<row;
            //获取选中行的第一列 书名
            QTableWidgetItem *item = ui->tableWidget->item(row,0);
            bookname = item->text();
    //                qDebug()<<book_name;
        }

        if(query.exec("select id from Book where name = '"+bookname+"'")){
            if(query.next()){
                bookid = query.value(0).toString();
            }
        }

    //    qDebug()<<bookid;

        emit sendSignal(bookid);

        bif->show();
    }else {
        QMessageBox::information(NULL, "提示", "非管理员无权操作",QMessageBox::Yes);
        return;
    }

}

void MainWindow::on_del_button_clicked()
{
    QSqlQuery query;
    QString bookname,acc;

    if(user_type == ADMIN){
        QList<QTableWidgetItem*> items = ui->tableWidget->selectedItems();
        if(!items.empty()){
            int count = items.count(),row;
            for(int i = 0; i < count; i++){
                row = ui->tableWidget->row(items.at(i));
                QTableWidgetItem *item = ui->tableWidget->item(row,0);
                bookname = item->text();
            }
            qDebug()<<bookname;

                if(query.exec("select stock_user from Book where name = '"+bookname+"'")){

                    query.next();
                    acc = query.value(0).toString();
//                    qDebug()<<acc;
                    if(acc != ""){
                        if(query.exec("update User set stock = stock-1 where account = '"+acc+"' ") &&
                                query.exec("delete from Book where name = '"+bookname+"' ")){
                            QMessageBox::information(NULL, "提示", "删除成功",QMessageBox::Yes);
                        }

                    }else {
                        if(query.exec("delete from Book where name = '"+bookname+"' ")){
                            QMessageBox::information(NULL, "提示", "删除成功",QMessageBox::Yes);
                        }
                    }
                }

        }else {
            QMessageBox::information(NULL, "提示", "请先选中需要删除的书籍",QMessageBox::Yes);
            return;
        }
    }else {
        QMessageBox::information(NULL, "提示", "非管理员无权操作",QMessageBox::Yes);
        return;
    }

    updateTablewidget();
}

void MainWindow::on_del_button_2_clicked()
{
    QString useracc;
    QSqlQuery query;

    QList<QTableWidgetItem*> items = ui->tableWidget_2->selectedItems();
    if(!items.empty()){
        int count = items.count(),row;
        for(int i = 0; i < count; i++){
            row = ui->tableWidget_2->row(items.at(i));
            QTableWidgetItem *item = ui->tableWidget_2->item(row,2);
            useracc = item->text();
//            qDebug()<<useracc;
        }

        if(query.exec("select stock_user from Book where stock_user = '"+useracc+"'")){

            qDebug()<<1;
            query.exec("update Book set is_stock = 0, stock_user = null where stock_user = '"+useracc+"'");
            if(query.exec("delete from User where account = '"+useracc+"' ")){
                QMessageBox::information(NULL, "提示", "删除成功",QMessageBox::Yes);
            }

        }else {
            if(query.exec("delete from User where account = '"+useracc+"' ")){
                QMessageBox::information(NULL, "提示", "删除成功",QMessageBox::Yes);
            }
        }
    }else {
        QMessageBox::information(NULL, "提示", "请先选中需要删除的用户",QMessageBox::Yes);
        return;
    }

    updateTablewidget();
}

void MainWindow::on_aboutAction_triggered(){
    QMessageBox aboutBox;
    aboutBox.setWindowTitle(tr("关于"));
    aboutBox.setText("陈嘉乐主导，谢景庭、林浩贤摆烂的图书管理系统");
    aboutBox.setInformativeText(tr("一个基于qt的图书管理系统。<br><a href=\"https://github.com/channile/LibrarySystem\">项目地址</a>"));
    aboutBox.exec();
}

void MainWindow::on_importAction_triggered(){
    QString bookFile = QFileDialog::getOpenFileName(this,
                tr("导入图书数据文件"), "./", tr("csv 文件 (*.csv)"));
    if(bookFile.isEmpty()){
        return;
    }

    QFile file(bookFile);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::information(NULL, "提示", "请先解除文件占用",QMessageBox::Yes);
        return;
    }

    QTextStream in(&file);
    QSqlQuery query;

    while (!in.atEnd()) {
        QString line = in.readLine();
//        qDebug()<<line;
        QStringList list = line.split(",");
        if(list.at(0) == "id"){
            continue;
        }

        query.exec("select name from Book where name = '"+list.at(1)+"'");
        if(query.next()){
            query.exec("update Book set id = '"+list.at(0)+"', public = '"+list.at(2)+"', input  = '"+list.at(3)+"'"
           ", price = '"+list.at(4)+"', is_stock = '"+list.at(5)+"', stock_user = '"+list.at(6)+"' where name = '"+list.at(1)+"'");
        }else {
            query.exec("insert into Book values('"+list.at(0)+"', '"+list.at(1)+"', '"+list.at(2)+"', '"+list.at(3)+"', "
                                  "'"+list.at(4)+"', '"+list.at(5)+"', '"+list.at(6)+"')");
        }

    }

    query.exec("update Book set stock_user = NULL where stock_user = ''");

    QMessageBox::information(this, tr("导入数据成功"), tr("信息源文件%1！\n警告！导入后请仔细检查用户与书籍表，否则会"
                                                    "导致数据不一致！").arg(bookFile), tr("确定"));

    file.close();
}

void MainWindow::on_exportAction_triggered(){
    QString filename = QFileDialog::getOpenFileName(this,tr("导出图书数据文件"),qApp->applicationDirPath(),tr("csv 文件 (*.csv)"));
    if(filename.isEmpty()){
        return;
    }
//    qDebug()<<filename;

    QFile file(filename);
    QSqlQuery query;
    QSqlRecord rec;

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QMessageBox::information(NULL, "提示", "请先解除文件占用",QMessageBox::Yes);
        return;
    }

    QTextStream out(&file);

    out<<tr("id,")<<tr("书名,")<<tr("出版日期,")<<tr("入库日期,")<<tr("价格,")<<tr("已借，")<<tr("借阅人,")<<",\n";  //表头
    query.exec("select * from Book");
    while (query.next()) {
        rec = query.record();

        int num = rec.indexOf("id");
        int num1 = rec.indexOf("name");
        int num2 = rec.indexOf("public");
        int num3 = rec.indexOf("input");
        int num4 = rec.indexOf("price");
        int num5 = rec.indexOf("is_stock");
        int num6 = rec.indexOf("stock_user");

        out<<tr("%1,").arg(query.value(num).toString())<<tr("%2,").arg(query.value(num1).toString())
          <<tr("%3,").arg(query.value(num2).toString())<<tr("%4,").arg(query.value(num3).toString())
         <<tr("%5,").arg(query.value(num4).toString())<<tr("%6,").arg(query.value(num5).toString())
        <<tr("%7,").arg(query.value(num6).toString())<<"\n";
    }

    QMessageBox::information(this, tr("导出数据成功"), tr("信息已保存在%1！").arg(filename), tr("确定"));

    file.close();


}

void MainWindow::on_Mysql_checkBox_stateChanged(int arg1)
{
    qDebug()<<arg1;
}

void MainWindow::on_sqlite_checkBox_clicked()
{
    if(ui->sqlite_checkBox->isChecked()){
        ui->Mysql_checkBox->setCheckState(Qt::Unchecked);
    }
    user_type = VISTOR;
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
//    db.close();

    dbCreat();
    uiupdate();
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    qDebug()<<arg1;
}

void MainWindow::on_Mysql_checkBox_clicked()
{
    if(ui->Mysql_checkBox->isChecked()){
        ui->sqlite_checkBox->setCheckState(Qt::Unchecked);

    }
    user_type = VISTOR;

    QSqlDatabase::removeDatabase("qt_sql_default_connection");
//    db.close();


    dbCreat();
    uiupdate();
}
