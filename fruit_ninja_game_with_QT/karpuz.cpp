#include "karpuz.h"

karpuz::karpuz(QWidget *parent): QPushButton(parent) {
    tiklandimi = false;
    connect(this, &QPushButton::clicked, this, &karpuz::tikla);
}

void karpuz::tikla() {
    tiklandimi = !tiklandimi;
}

void karpuz::mousePressEvent(QMouseEvent *event) {
    // Tıklama olayını burada işleyin
    QPushButton::mousePressEvent(event);
}
