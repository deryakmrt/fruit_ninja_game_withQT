#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// DERYA NAİLİYE KIMIRTI
// 21100011001


#include <QMainWindow>
#include <QLabel>
#include <QStyle>
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QList>
#include <QTimer>
#include <QDateTime>
#include <QtWidgets>
#include "karpuz.h"
#include <QMouseEvent>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private slots:
    void updateTimer();
    void dosyaOku();
    void karpuzOlustur();
    void karpuzKesme();
    void karpuzHareket();
    void karpuzSil();
    void skorKaydet();

private:

    Ui::MainWindow *ui;
    QString konumYol;
    QString skorYol;
    QTimer timer1;
    QTimer timer2;
    QTimer timer3;
    QTimer timer4;
    QTimer timer5;

    int timeLeft;
    int maxSkor;
    QList<QString> konumlar;
    int kesilmisSayisi=0;
    int kacirilanSayisi=0;
    //int sayac=0;
    int sure;
    QIcon icon;
    QPushButton karpuz;
    int konumIndex; // konumlar listesindeki son kullanılan konumun indeksi


public:

    QFile skorFile;
    QFile konumFile;
    QString dosyadakiDegerler="";
    QList<QPushButton*> karpuzlar_list;
    QList<QPushButton*> karpuzlar_list2;
    QList<QPushButton*> karpuzlar_list3;
    QList<int> degerler;
    QList<int> degerler2;
    bool tiklandimi;


public slots:
    //........

};
#endif // MAINWINDOW_H
//----------------------------------------------------------------------
// karpuz.h dosyasının içeriği
/*#ifndef KARPUZ_H
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
*/
