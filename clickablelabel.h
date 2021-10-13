#ifndef CLICKABLEBEL_H
#define CLICKABLEBEL_H

#include <QLabel>
#include <QWidget>
#include <Qt>

class ClickableLabel : public QLabel {
    Q_OBJECT

public:
    explicit ClickableLabel(const QString &Titter,QWidget *parent=0);
    ~ClickableLabel();

    int maxid;

signals:
    void clicked();

    void sendSignal(QString);

public slots:
    //点击信号响应槽
    void slotClicked();

//    void thisHide();

protected:
    void mousePressEvent(QMouseEvent* event);

};

#endif // CLICKABLELABEL_H
