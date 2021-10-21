#include "registerwidget.h"
#include "ui_registerwidget.h"
#include <QDebug>

registerwidget::registerwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::registerwidget)
{
    ui->setupUi(this);
    ui->pushButton->setText("注册");
    state = ERROR;

    ui->lineEdit_2->setAttribute(Qt::WA_InputMethodEnabled,false);
    ui->lineEdit_3->setAttribute(Qt::WA_InputMethodEnabled,false);
    ui->lineEdit_4->setAttribute(Qt::WA_InputMethodEnabled,false);
}

registerwidget::~registerwidget()
{
    delete ui;
}

void registerwidget::receiveSignal(QString signal){
    if(signal == "ADD"){
        ui->pushButton->setText("增加");
        ui->label_4->hide();
        ui->lineEdit_4->hide();
        this->setWindowTitle("增加");
    }else if (signal == "RES") {
        ui->pushButton->setText("注册");
        ui->label_4->show();
        ui->lineEdit_4->show();
        this->setWindowTitle("注册");
    }else if (signal == "AEDIT") {
        ui->pushButton->setText("修改");
        ui->label->setText("新用户名：");
        ui->label_2->setText("原账号：");
        ui->label_3->setText("新密码：");
        ui->label_4->hide();
        ui->lineEdit_4->hide();
        this->setWindowTitle("管理员修改");
        UTYPE = ADMIN;
    }else {
        ui->pushButton->setText("修改");
        ui->label->setText("新用户名：");
        ui->label_2->setText("原账号：");
        ui->lineEdit_2->setText(signal);
        ui->lineEdit_2->setEnabled(false);
        ui->label_3->setText("新密码：");
        ui->label_4->show();
        ui->lineEdit_4->show();
        this->setWindowTitle("用户修改");
        UTYPE = USER;
    }
}

void registerwidget::on_pushButton_clicked()
{
    QString name = ui->lineEdit->text();
    QString acc = ui->lineEdit_2->text();
    QString paw = ui->lineEdit_3->text();
    QString pawa = ui->lineEdit_4->text();

    QSqlQuery query;

    if(ui->pushButton->text() == "增加"){

        if(name == ""){
            QMessageBox::information(NULL, "提示", "用户名不能为空",
                              QMessageBox::Yes);
        }else if(acc == ""){
            QMessageBox::information(NULL, "提示", "账号不能为空",
                              QMessageBox::Yes);
        }else if(paw == ""){
            QMessageBox::information(NULL, "提示", "密码不能为空",
                              QMessageBox::Yes);
        }


        if(name != "" && acc != "" && paw != ""){

            query.exec("select account from User where account = '"+acc+"'");
            if(query.next()){
                if(query.value(0).toString() == acc){
                    QMessageBox::information(NULL, "提示", "用户已存在",
                                      QMessageBox::Yes);
                }
            }

            query.exec("select max(id) from User");
            int count;

            if(query.next()){
                count = query.value(0).toInt() + 1;
//            qDebug()<<count;
            } else {
                count = 0;
            }

            if(query.exec(QString("insert into User values(%1,'"+name+"','"+acc+"'"
                                  ",'"+paw+"',0)").arg(count))){
                QMessageBox::information(NULL, "提示", "增加成功",
                                                    QMessageBox::Yes);
                state = SUCCESS;
                emit updatetw();
            }
        }
    }else if (ui->pushButton->text() == "注册") {

        if(name == ""){
            QMessageBox::information(NULL, "提示", "用户名不能为空",
                              QMessageBox::Yes);
        }else if(acc == ""){
            QMessageBox::information(NULL, "提示", "账号不能为空",
                              QMessageBox::Yes);
        } else if (paw == "") {
            QMessageBox::information(NULL, "提示", "密码不能为空",
                              QMessageBox::Yes);
        } else if (pawa == "") {
            QMessageBox::information(NULL, "提示", "验证密码不能为空",
                              QMessageBox::Yes);
        }

        if(acc == "admin"){
            QMessageBox::information(NULL, "提示", "账号不能为管理员账号",
                              QMessageBox::Yes);
            ui->lineEdit->clear();
            ui->lineEdit_2->clear();
            ui->lineEdit_3->clear();
            ui->lineEdit_4->clear();
        }

        if(name != "" && acc != "" && paw != "" && pawa != ""){

            if(paw != pawa){
                QMessageBox::information(NULL, "提示", "两次输入密码不相等，请重新输入",
                                  QMessageBox::Yes);
                ui->lineEdit_3->clear();
                ui->lineEdit_4->clear();
            }

            query.exec("select account from User where account = '"+acc+"'");
            if(query.next()){
                if(query.value(0).toString() == acc){
                    QMessageBox::information(NULL, "提示", "用户已存在",
                                      QMessageBox::Yes);
                }
            }

            query.exec("select max(id) from User");
            int count;

            if(query.next()){
                count = query.value(0).toInt() + 1;
//            qDebug()<<count;
            } else {
                count = 0;
            }

            if(query.exec(QString("insert into User values(%1,'"+name+"','"+acc+"'"
                                  ",'"+paw+"',0)").arg(count))){
                QMessageBox::information(NULL, "提示", "注册成功",
                                                    QMessageBox::Yes);
                state = SUCCESS;
                emit updatetw();
             }
        }
    }else if(UTYPE == ADMIN){

        if(name == ""){
            QMessageBox::information(NULL, "提示", "用户名不能为空",
                              QMessageBox::Yes);
        }else if(acc == ""){
            QMessageBox::information(NULL, "提示", "账号不能为空",
                              QMessageBox::Yes);
        }else if(paw == ""){
            QMessageBox::information(NULL, "提示", "密码不能为空",
                              QMessageBox::Yes);
        }

        if(name != "" && acc != "" && paw != ""){

            if(query.exec("select account from User where account = '"+acc+"'")){
                if(query.next()){

                    if(query.exec("update User set name= '"+name+"',account= '"+acc+"',password= '"+paw+"' where account= '"+acc+"' ")){
                         QMessageBox::information(NULL, "提示", "修改成功",
                                               QMessageBox::Yes);
                         state = SUCCESS;
                         emit updatetw();
                     }
                 } else {
                    QMessageBox::information(NULL, "提示", "账号不存在",
                                      QMessageBox::Yes);
                    ui->lineEdit->clear();
                    ui->lineEdit_2->clear();
                    ui->lineEdit_3->clear();
                 }
            }
        }

    } else if (UTYPE == USER) {
        if(query.exec("select account from User where account = '"+acc+"'")){
            if(name == ""){
                QMessageBox::information(NULL, "提示", "新用户名不能为空",
                                  QMessageBox::Yes);
            }

            if(name != "" && paw != "" && pawa != ""){

                if(paw != pawa){
                    QMessageBox::information(NULL, "提示", "两次输入密码不相等，请重新输入",
                                      QMessageBox::Yes);
                    ui->lineEdit_4->setText("");
                }


                if(query.next()){
                         if(query.exec("update User set name= '"+name+"',password= '"+paw+"' where account= '"+acc+"' ")){
                             QMessageBox::information(NULL, "提示", "修改成功",
                                               QMessageBox::Yes);
                             state = SUCCESS;
                             emit updatetw();
                         }
                }
            }
        }else {
            QMessageBox::information(NULL, "提示", "账号不存在",
                                QMessageBox::Yes);
        }
    }

    if(state == SUCCESS){
        this->close();
    }
}
