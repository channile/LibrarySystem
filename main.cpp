#include "mainwindow.h"

#include <QApplication>

#include <QStyleFactory>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QStyleFactory::create("");
    qApp->setStyle("Fusion");

    w.show();
    return a.exec();
}
