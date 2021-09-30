#include "registerwidget.h"
#include "ui_registerwidget.h"

registerwidget::registerwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::registerwidget)
{
    ui->setupUi(this);
}

registerwidget::~registerwidget()
{
    delete ui;
}
