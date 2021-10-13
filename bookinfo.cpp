#include "bookinfo.h"
#include "ui_bookinfo.h"

bookinfo::bookinfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::bookinfo)
{
    ui->setupUi(this);
}

bookinfo::~bookinfo()
{
    delete ui;
}

void bookinfo::receiveSignal(QString signal){

    QSqlQuery query;

    if(signal == "ADD"){
        ui->pushButton->setText("增加");
        ui->label_5->hide();
        ui->lineEdit_5->hide();
        ui->label_6->hide();
        ui->lineEdit_6->hide();
    }else {

        id = signal;
//        qDebug()<<id;

        ui->pushButton->setText("修改");
        ui->label_5->show();
        ui->label_6->show();
        ui->lineEdit_5->show();
        ui->lineEdit_6->show();

        if(query.exec("select * from Book where id = '"+signal+"'")){
            if(query.next()){
                ui->lineEdit->setText(query.value(1).toString());
                ui->lineEdit_2->setText(query.value(2).toString());
                ui->lineEdit_3->setText(query.value(3).toString());
                ui->lineEdit_4->setText(query.value(4).toString());
//                ui->lineEdit_5->setText(query.value(5).toString());
//                ui->lineEdit_6->setText(query.value(6).toString());

                if(query.value(5).toString() == "0"){
                    ui->lineEdit_5->setText("否");
                }else {
                    ui->lineEdit_5->setText("是");
                }

                if(query.value(6).toString() == ""){
                    ui->lineEdit_6->setText("无");
                }else {
                    ui->lineEdit_6->setText(query.value(6).toString());
                    acc = ui->lineEdit_6->text();
                }
            }
        }
    }
}

void bookinfo::on_pushButton_clicked()
{
    QString bookname    = ui->lineEdit->text();
    QString publicdate  = ui->lineEdit_2->text();
    QString input       = ui->lineEdit_3->text();
    QString price       = ui->lineEdit_4->text();
    QString bor         = ui->lineEdit_5->text();
    QString boruser     = ui->lineEdit_6->text();


    QSqlQuery query;

    if(ui->pushButton->text() == "增加"){
        if(bookname == "")
        {
            QMessageBox::information(NULL, "提示", "书籍名称不能为空",
                              QMessageBox::Yes);
        }else if(publicdate  == "")
        {
            QMessageBox::information(NULL, "提示", "出版日期不能为空",
                              QMessageBox::Yes);
        }else if(input == "")
        {
            QMessageBox::information(NULL, "提示", "入库时间不能为空",
                              QMessageBox::Yes);
        }else if(price == "")
        {
            QMessageBox::information(NULL, "提示", "书籍价格不能为空",
                              QMessageBox::Yes);
        }

        if(bookname != "" && publicdate != "" && input != "" && price != ""){
            query.exec("select name from Book where name = '"+bookname+"'");
            if(!query.next()){

                int count;

                query.exec("select max(id) from book");
                if(query.next()){
                    count=query.value(0).toInt()+1;
                }else {
                    count = 159000;
                }

                if(query.exec(QString("insert into Book values(%1,'"+bookname+"','"+publicdate+"'"
                                      ",'"+input+"','"+price+"' , 0 , NULL)").arg(count)))
                {
                    QMessageBox::information(NULL, "提示", "增加成功",
                                      QMessageBox::Yes);
                }

            }else {
                QMessageBox::information(NULL, "提示", "书籍已存在",
                                  QMessageBox::Yes);
            }
        }

    }else {
        if(bookname == "")
        {
            QMessageBox::information(NULL, "提示", "书籍名称不能为空",
                              QMessageBox::Yes);
        }else if(publicdate  == "")
        {
            QMessageBox::information(NULL, "提示", "出版日期不能为空",
                              QMessageBox::Yes);
        }else if(input == "")
        {
            QMessageBox::information(NULL, "提示", "入库时间不能为空",
                              QMessageBox::Yes);
        }else if(price == "")
        {
            QMessageBox::information(NULL, "提示", "书籍价格不能为空",
                              QMessageBox::Yes);
        }else if (bor == "") {
            QMessageBox::information(NULL, "提示", "是否借阅不能为空",
                              QMessageBox::Yes);
        }

        if(bor == "是"){
            if(boruser == ""){
                QMessageBox::information(NULL, "提示", "借阅人不能为空",
                                  QMessageBox::Yes);
            }
        }else {
            if(boruser != ""){
                QMessageBox::information(NULL, "提示", "借阅为否的时候借阅人必须为空！",
                                  QMessageBox::Yes);
                ui->lineEdit_6->clear();
            }
        }

        if(bookname != "" && publicdate != "" && input != "" && price != "" && bor == "否"){
            if(query.exec("update Book set name = '"+bookname+"', public= '"+publicdate+"',input= '"+input+"'"
                                       ", price= '"+price+"', is_stock = '0', stock_user = null where id = '"+id+"' " )
                    && query.exec("update User set stock = stock - 1 where account = '"+acc+"' ")){

                QMessageBox::information(NULL, "提示", "修改成功",QMessageBox::Yes);

            }
        }else if(bookname != "" && publicdate != "" && input != "" && price != "" && bor == "是" && boruser != ""){
            if(query.exec("select account from User where account = '"+boruser+"' ")){
                if(query.next()){
                    qDebug()<<1;
                    if(query.exec("select stock from User where account = '"+boruser+"'")){
                        if(query.next()){
                            qDebug()<<2;
                            if(query.value(0).toInt() == 2){
                                QMessageBox::information(NULL, "提示", "该用户借阅书籍已达上限",
                                                  QMessageBox::Yes);
                            } else {
                                qDebug()<<3;
                                if(query.exec("update Book set name = '"+bookname+"', public= '"+publicdate+"',input= '"+input+"'"
                                                           ", price= '"+price+"', is_stock = 1, stock_user = '"+boruser+"' where id = '"+id+"' ")
                                        && query.exec("update User set stock = stock+1 where account ='"+boruser+"' ")
                                        && query.exec("update User set stock = stock-1 where account = '"+acc+"' ")){
                                    QMessageBox::information(NULL, "提示", "修改成功",
                                                      QMessageBox::Yes);
                                }
                            }
                        }
                    }

                }else {
                   QMessageBox::information(NULL, "提示", "用户不存在",
                                      QMessageBox::Yes);
                }
            }
        }
    }
}
