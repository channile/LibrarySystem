#ifndef CLICKABLEBEL_H
#define CLICKABLEBEL_H

#include <QLabel>
#include <QWidget>
#include <Qt>
#include "registerwidget.h"

class ClickableLabel : public QLabel {
    Q_OBJECT

public:
    explicit ClickableLabel(const QString &Titter,QWidget *parent=0);
    ~ClickableLabel();

    int maxid;

signals:
    void clicked();

public slots:
    //点击信号响应槽
    void slotClicked();

protected:
    void mousePressEvent(QMouseEvent* event);

};

#endif // CLICKABLELABEL_H
