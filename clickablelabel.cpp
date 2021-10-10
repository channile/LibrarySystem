#include "clickablelabel.h"
#include <QMessageBox>
#include <QMouseEvent>
#include <QDebug>
#include <mainwindow.h>
#include "registerwidget.h"

ClickableLabel::ClickableLabel(const QString &Titter, QWidget *parent)
    : QLabel(parent){
    this->setText(Titter);
    connect(this, SIGNAL(clicked()), this, SLOT(slotClicked()));
}

ClickableLabel::~ClickableLabel(){

}

void ClickableLabel::slotClicked(){
//    this->hide();
    registerwidget *pic = new registerwidget();
    connect(this,SIGNAL(sendSignal(QString)),pic,SLOT(receiveSignal(QString)));
    emit sendSignal("RES");
    pic->show();

    /*QSqlQuery query;
    maxid = query.exec("select max(id) from User");
    int count = maxid;
    if(count < maxid){
        this->hide();
    }*/
}

void ClickableLabel::mousePressEvent(QMouseEvent *event){
    emit clicked();
}
