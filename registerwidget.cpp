#include "registerwidget.h"
#include "ui_registerwidget.h"
#include <QDebug>

registerwidget::registerwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::registerwidget)
{
    ui->setupUi(this);
    ui->pushButton->setText("注册");
}

registerwidget::~registerwidget()
{
    delete ui;
}

void registerwidget::receiveSignal(QString signal){
    if(signal == "ADD"){
        ui->pushButton->setText("增加");
    }
}
