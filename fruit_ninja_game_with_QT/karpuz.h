#ifndef KARPUZ_H
#define KARPUZ_H

#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>

class karpuz: public QPushButton
{
    Q_OBJECT
public:
    karpuz(QWidget *parrent=0);
    bool tiklandimi;

public slots:

    void tikla();

protected:
    void mousePressEvent(QMouseEvent *event) override; // Fare tıklama olayını işlemek için yeniden tanımlanan olay işleyicisi

};

#endif // KARPUZ_H
