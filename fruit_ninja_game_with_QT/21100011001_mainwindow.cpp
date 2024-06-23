#include "mainwindow.h"
#include "ui_mainwindow.h"

// DERYA NAİLİYE KIMIRTI
// 21100011001

/*
    ek özellik olarak oyunuma messageBox ile sorulan zorluk seviyeleri eklendi.
    kolay - zor olarak iki seçeneği olan oyunda seçenkelr arasındaki fark oluşan karpuzların hareket hızı.
    ikinci eklediğim şey ise mainwindow ekranına farklı görseller ekleyerek oyun görselliğini zenginleştirmek :))
*/
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->sure_skor->setText("30"); // Başlangıç süresini ayarla
    ui->kesilen_skor_lbl->setText("0"); // Kesilen karpuzların başlangıç sayısını sıfırla
    ui->kacirilan_skor_lbl->setText("0"); // Kaçırılan karpuzların başlangıç sayısını sıfırla
    dosyaOku();
    konumIndex = 0; // konumIndex'i başlat

    //----------------------------------------------------------------------

    // KALAN SURE        &    SURELERI BAGLAMA (connect)

    // Zorluk seviyesine göre karpuz hareket hızını ayarla
    QString zorluk;
    QMessageBox messageBox;
    messageBox.setWindowTitle("Fruit Ninja Game");
    messageBox.setText("Oyunun zorluk seviyesini seçiniz:");
    messageBox.setIcon(QMessageBox::Question);
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    messageBox.setButtonText(QMessageBox::Yes, "Kolay");
    messageBox.setButtonText(QMessageBox::No, "Zor");

    int secilen = messageBox.exec();

    if (secilen == QMessageBox::Yes) {
        zorluk = "Kolay";
        timer3.setInterval(500); // Kolay modda karpuz hareket hızı

    }
    else if (secilen == QMessageBox::No)
    {
        zorluk = "Zor";
        timer3.setInterval(100); // Zor modda karpuz hareket hızı
    }
    timer3.start();
    connect(&timer3, &QTimer::timeout, this, &MainWindow::karpuzHareket);

//---------------------------------------------------------

    connect(&timer1, &QTimer::timeout, this, &MainWindow::updateTimer);
    timer1.start(1000); // Timer'ı başlat ve her 1 saniyede bir çalıştır
    timeLeft = 30; // Başlangıçta kalan süre 30 saniye

    timer2.start(1000); //olusma sıklığı
    connect(&timer2, &QTimer::timeout, this, &MainWindow::karpuzOlustur);

    timer4.setInterval(50); //silinmeyi algılaması (kesilmis görüntüye dönüsme hızı)
    timer4.start();
    connect(&timer4,&QTimer::timeout,this,&MainWindow::karpuzKesme);

    timer5.start(3000);
    connect(&timer5,&QTimer::timeout,this,MainWindow::karpuzSil);//(kesilen görüntünün var olma süresi)
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTimer()
{
    timeLeft--; // Kalan zamanı birer saniye azalt
    ui->sure_skor->setText(QString::number(timeLeft));  // Arayüzdeki süre metnini güncelle

    if(timeLeft==0) // Zaman dolduğunda
    {
        if(ui->kesilen_skor_lbl->text().toInt()>maxSkor) // max skor geçilmişse:
        {
            QString txt="Oyun Bitti!Tebrikler Maksimum Skor Sizde!\n";
            QString txt1=+"Kesilen Karpuz Sayısı: "+ui->kesilen_skor_lbl->text()+"\n";
            QString txt2="Kaçırılan Karpuz Sayısı: "+ui->kacirilan_skor_lbl->text()+"\n";
            QString txt3="Maksimum Skor: "+QString::number(maxSkor);
            timer2.stop();
            timer1.stop();
            timer3.stop();
            skorKaydet();
            QMessageBox::information(this,"Bilgi!",txt+txt1+txt2+txt3);
            this->close(); // Ana pencereyi kapat


        }

        else // max skor geçilemediysee:
        {
            QString txt="Oyun Bitti!Maksimum Skoru Geçemediniz!\n";
            QString txt1=+"Kesilen Karpuz Sayısı: "+ui->kesilen_skor_lbl->text()+"\n";
            QString txt2="Kaçırılan Karpuz Sayısı: "+ui->kacirilan_skor_lbl->text()+"\n";
            QString txt3="Maksimum Skor: "+QString::number(maxSkor);

            timer2.stop();
            timer1.stop();
            timer3.stop();
            skorKaydet();
            QMessageBox::information(this,"Bilgi!",txt+txt1+txt2+txt3);
            this->close(); // Ana pencereyi kapat

        }
    }
}

//--------------------------------------------------------

void MainWindow::dosyaOku()
{
    konumYol=":/txt/txtler/konumlar.txt"; //dosya yollarını tanımladım
    skorYol=":/txt/txtler/skorlar.txt";

    QFile skorFile(skorYol); // skor dosyasını açıp

    if(!skorFile.open(QIODevice::ReadOnly)) // Dosya açılamazsa hata mesajı göster ve çık
    {
        QMessageBox::critical(this,"Hata","Skor txt dosyasi okunamadi!");
        return;
    }
    //**************************************************
    QTextStream streamS(&skorFile);// Dosyayi okuma ve yazma

    QList<int> skorlar; // Skorları tutacak bir liste oluştur

    while(streamS.atEnd()==false) // Dosya sonuna gelinene kadar devam et
    {
        QString okunan=streamS.readLine()+"\n"; // Satırı oku ve geçici bir değişkene ata
        dosyadakiDegerler+=okunan; // Dosyadaki değerlere ekle
        int skor=streamS.readLine().toInt(); // Skoru oku ve tam sayıya dönüştür
        skorlar<<skor; // Skorları listeye ekle
    }

    maxSkor=0; // Maksimum skoru sıfırla

    foreach (int anlikSkor, skorlar)// Her bir skor --> anlikSkor
    {
        if(anlikSkor>maxSkor) // Eğer anlikSkor'um max skoru aşıyorsa
        {
            maxSkor=anlikSkor; // Max skoru güncelle
        }
    }
    skorFile.close(); // Skor dosyasını kapat

    //----------------------------------------------------
    QFile konumFile(konumYol); // Konum dosyasını aç

    if(!konumFile.open(QIODevice::ReadOnly)) //Dosya açılamazsa hata mesajı göster ve çık
    {
        QMessageBox::critical(this,"Hata","Konum txt dosyasi okunamadi!");
        return;
    }

    QTextStream streamK(&konumFile);

    while(streamK.atEnd()==false) // Dosya sonuna gelinene kadar devam et
    {
        QString okunanKonum=streamK.readLine(); // Satırı oku ve geçici bir değişkene ata
        konumlar<<okunanKonum; // Konumları listeye ekle
    }

    //qInfo()<<konumlar; // Konsola konumları yaz -gerek yok
    konumFile.close(); // Konum dosyasını kapat

}

//-------------------------------------------------------

void MainWindow::karpuzOlustur()
{
    QIcon icon(":/icon/images/1.png"); // Karpuz simgesini yükle

    QFile konumFile(":/txt/txtler/konumlar.txt");  // Konum dosyasını aç

    if (!konumFile.open(QIODevice::ReadOnly | QIODevice::Text)) // Dosya açılamazsa hata mesajı göster ve çık
    {
        QMessageBox::critical(this, "Hata", "Konumlar dosyası açılamadı!");
        return;
    }

    QTextStream stream(&konumFile);
    QStringList konumlar; // Konumları tutacak bir liste oluştur
    while (!stream.atEnd()) // Dosya sonuna gelinene kadar devam et
    {
        konumlar.append(stream.readLine()); // Konumları listeye ekle
    }
    konumFile.close();  // Konum dosyasını kapat

    // Şu anki konumu kullanarak bir karpuz oluştur
    konumIndex++; // konumIndex'i arttır

    if (konumIndex >= konumlar.size()) // Eğer konumIndex konumların boyutunu aşıyorsa
    {
        konumIndex = 0; // konumIndex'i sıfırla, böylece konumları tekrar kullanmaya başla
    }
        QStringList hucre = konumlar[konumIndex].split(" "); // Konumu boşluklara göre parçala --> hucreye ata
        int x = hucre[0].toInt(); // X koordinatını al
        int y = hucre[1].toInt(); // Y koordinatını al

        QPushButton *karpuz = new QPushButton(this); // Yeni bir karpuz düğmesi oluştur
        karpuz->setIcon(QIcon(":/icon/images/1.png")); // Karpuz simgesini ata
        karpuz->setStyleSheet("background-color: transparent; border: none;"); // Arka plan rengini ayarla
        karpuz->setIconSize(QSize(70, 70)); // Simge boyutunu ayarla
        karpuz->setGeometry(x, y, 70, 70); // Geometriyi ayarla
        karpuz->setCheckable(true); // Kontrol edilebilir yap

        karpuz->show(); // Karpuzu göster

        /*karpuzlar_list: Bu liste, oyun alanında görünen ve kullanıcının karpuzları kesmeye çalıştığı
        karpuzların depolandığı ANA liste. Kullanıcı karpuzları kestiğinde veya kaçırdığında bu listeden
        ilgili karpuzlar çıkarılır.*/

        karpuzlar_list.append(karpuz); // Karpuzu listeye ekle
}


//-------------------------------------------------------
void MainWindow::karpuzKesme()
{

    int boyutK=karpuzlar_list.size();

    QString kesilenKarpuz=":/icon/images/2.png";

    for (int i=0 ; i<boyutK ; i++)
    {
        if(karpuzlar_list[i]->isChecked()) // Eğer karpuz işaretliyse
        {
            kesilmisSayisi+=1; // Kesilen karpuz sayısını arttır

            QPixmap kesilmisK (kesilenKarpuz); // Kesilen karpuz simgesini yükle
            kesilmisK=kesilmisK.scaled(70,70); // Boyutu ayarla

            QIcon kesilmisIcon; // İkon oluştur
            kesilmisIcon.addPixmap(kesilmisK); // İkona karpuz resmini ekle

            karpuzlar_list[i]->setIcon(kesilmisK); // Karpuzun simgesini değiştir
            karpuzlar_list[i]->setChecked(false); // Karpuzun kontrol durumunu kapat
            ui->kesilen_skor_lbl->setText(QString::number(kesilmisSayisi)); // Arayüzde kesilen karpuz sayısını güncelle
            degerler.append(i); // İndisleri listeye ekle

            /*degerler: Bu liste, kesilen karpuzların indekslerini depolar. Kullanıcı bir karpuzu kestiğinde,
            bu karpuzun indeksi bu listede saklanır. Daha sonra bu indekslere göre karpuzlar karpuzlar_list
            listesinden çıkarılır.*/
        }
    }
    int boyutD=degerler.size(); // Değerler listesinin boyutunu al
    int m=0;

    while (m<boyutD)
    {
        int n=degerler[m]; // İndisleri al
        karpuzlar_list2.append(karpuzlar_list[n]); // Karpuzları ikinci liste ekle

        /*karpuzlar_list2: Bu liste, kesilen veya kaçırılan karpuzların geçici olarak
        depolandığı liste. Bu listedeki karpuzlar, ekrandan kaldırılmak için bekletilir ve sonra temizlenir.

*/
        karpuzlar_list.removeAt(n); // İlk listeden karpuzları kaldırın
        m+=1;
    }


    boyutK=karpuzlar_list.size(); // Karpuzlar listesinin boyutunu al

    for (int i=0; i<boyutK; i++)
    {
        if(karpuzlar_list[i]->y()>ui->scrollArea->height()-50) // Eğer karpuz scroll alanının altında ise
        {
            karpuzlar_list3.append(karpuzlar_list[i]); // Karpuzları üçüncü listeye ekle

            /*karpuzlar_list3: Bu liste, ekrandan "kaçırılan karpuzların" geçici olarak depolandığı
            liste. Bu listedeki karpuzlar, ekrandan kaldırılmak için bekletilir ve sonra temizlenir.*/

            degerler2.append(i); // İndisleri ikinci listede tut

            /*degerler2: Bu liste, ekrandan "kaçırılan karpuzların indekslerini" depolar.
            Kullanıcı bir karpuzu kaçırdığında, bu karpuzun indeksi bu listede saklanır.
            Daha sonra bu indekslere göre karpuzlar karpuzlar_list listesinden çıkarılır.*/
        }
    }

    boyutD=degerler2.size(); // Değerler2 listesinin boyutunu al
    m=0;

    while (m<boyutD)
    {
        int s=degerler2[m]; // İndisleri al
        karpuzlar_list2.append(karpuzlar_list[s]); // Karpuzları ikinci liste ekleyin
        karpuzlar_list.removeAt(s);  // İlk listeden karpuzları kaldır
        m+=1;
    }

    boyutK=karpuzlar_list3.size(); // Karpuzlar listesi 3 boyutunu al

    for (int i=0; i<boyutK; i++)
    {

        kacirilanSayisi+=1;  // Kaçırılan karpuz sayısını arttır
        ui->kacirilan_skor_lbl->setText(QString::number(kacirilanSayisi));// Arayüzde kaçırılan karpuz sayısını güncelle
    }

    karpuzlar_list3.clear(); // Karpuzlar listesini temizle
    degerler.clear(); // Değerler listesini temizle
    degerler2.clear(); // Değerler2 listesini temizle
}

//-------------------------------------------------------
void MainWindow::karpuzHareket()
{
    int boyutK=karpuzlar_list.size(); // Karpuzlar listesinin boyutunu al

    for(int i=0; i<boyutK; i++)
    {
        //karpuzlar_list[i]->setGeometry(karpuzlar_list[i]->x(),karpuzlar_list[i]->y()+15,100,100);
        karpuzlar_list[i]->move(karpuzlar_list[i]->x(), karpuzlar_list[i]->y() + 15); // Karpuzun pozisyonunu güncelle
    }
}

//-------------------------------------------------------
void MainWindow::karpuzSil()
{
    int boyutK=karpuzlar_list2.size(); // Karpuzlar listesi 2 boyutunu al

    if (boyutK>0)
    {
        for(int i=0;i<boyutK;i++)
        {
            karpuzlar_list2[i]->setIcon(QIcon()); // Karpuz simgesini kaldır
            karpuzlar_list2[i]->close(); // Karpuzu kapat
        }

        karpuzlar_list2.clear(); // Karpuzlar listesini temizle
    }
}
//-------------------------------------------------------
void MainWindow::skorKaydet()
{
    QString skorYol = "D:/qt klasör/Fruit_ninja_game/txtler/skorlar.txt"; //burası farklı bilgisayarda güncelleme isteyebilir!!

    QFile skorFile(skorYol); // Skor dosyasını aç

    if (skorFile.open(QIODevice::Append | QIODevice::Text)) // Dosya açıldıysa
    {
        QTextStream stream(&skorFile);
        stream << ui->kesilen_skor_lbl->text() + "\n"; // Güncel skoru dosyaya yaz
        skorFile.close();
    }
    else //dosya açılmaz ise
    {
        QMessageBox::critical(this, "Hata", "Skor txt dosyasina deger yazilamadi!");
        return;
    }
}
//----------------------------------------------------------------------
/*// karpuz.cpp dosyasının içeriği
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
*/
