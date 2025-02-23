// MainWindow sınıfının tanımlamalarını içeren başlık dosyası dahil edilir.
#include "mainwindow.h"

// Qt'nin yatay düzenleri, dosya seçim penceresi ve dizin işlemleri için gerekli kütüphaneler dahil edilir.
#include <QHBoxLayout>
#include <QFileDialog>
#include <QDir>

// 📌 **MainWindow Yapıcı Fonksiyonu**
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), watcher(new QFileSystemWatcher(this)) // Dosya sistemindeki değişiklikleri izleyen nesneyi oluşturur.
{
    setupUI(); // Arayüzü başlatır.

    // Dosya ve dizin değişikliklerini algılamak için QFileSystemWatcher sinyallerine bağlanır.
    connect(watcher, &QFileSystemWatcher::directoryChanged, this, &MainWindow::onDirectoryChanged);
    connect(watcher, &QFileSystemWatcher::fileChanged, this, &MainWindow::onFileChanged);
}

// 📌 **Yıkıcı Fonksiyon (Destructor)**
MainWindow::~MainWindow()
{
    delete watcher; // Bellek sızıntısını önlemek için QFileSystemWatcher nesnesi silinir.
}

// 📌 **Arayüzü oluşturan fonksiyon**
void MainWindow::setupUI()
{
    // Ana pencerenin içeriğini içeren widget oluşturulur.
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Dikey düzen oluşturuluyor.
    layout = new QVBoxLayout(centralWidget);

    // "İzlenen Dizin" başlığı ekleniyor.
    QLabel *titleLabel = new QLabel("İzlenen Dizin:", this);
    layout->addWidget(titleLabel);

    // Seçilen dizini göstermek için salt okunur bir giriş kutusu oluşturuluyor.
    pathDisplay = new QLineEdit(this);
    pathDisplay->setReadOnly(true); // Kullanıcı tarafından değiştirilemez.
    layout->addWidget(pathDisplay);

    // **Dizin seçme butonu**
    addButton = new QPushButton("Dizin Seç", this);
    addButton->setStyleSheet("background-color: #4CAF50; color: white; border-radius: 5px; padding: 5px;");
    layout->addWidget(addButton);
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addDirectory); // Butona tıklanınca addDirectory çağrılır.

    // **İzlemeyi durdurma butonu**
    removeButton = new QPushButton("İzlemeyi Durdur", this);
    removeButton->setStyleSheet("background-color: #f44336; color: white; border-radius: 5px; padding: 5px;");
    layout->addWidget(removeButton);
    connect(removeButton, &QPushButton::clicked, this, &MainWindow::removeDirectory);

    // "Değişiklikler" başlığı ekleniyor.
    QLabel *logTitle = new QLabel("Değişiklikler:", this);
    layout->addWidget(logTitle);

    // Değişiklikleri göstermek için liste oluşturuluyor.
    logList = new QListWidget(this);
    layout->addWidget(logList);

    // Pencere başlığını ve boyutunu belirleme
    setWindowTitle("Windows Dosya Dizin İzleyici");
    resize(400, 300);
}

// 📌 **Kullanıcının seçtiği dizini izlemeye ekler**
void MainWindow::addDirectory()
{
    // Kullanıcıdan bir dizin seçmesini isteyen pencere açılır.
    QString dirPath = QFileDialog::getExistingDirectory(this, "Dizin Seç");

    // Eğer kullanıcı geçerli bir dizin seçtiyse ve daha önce eklenmediyse
    if (!dirPath.isEmpty() && !directorySet.contains(dirPath)) {
        logMessage("İzlenmeye başlandı: " + dirPath);
        watcher->addPath(dirPath); // Seçilen dizin QFileSystemWatcher ile izlenmeye başlar.
        directorySet.insert(dirPath); // Dizinin izlenenler listesine eklenmesi

        pathDisplay->setText(dirPath); // Seçilen dizin arayüzde gösterilir.

        fileTimestamps.clear();  // Önceki kayıtlar temizlenir.
        isFirstScan = true; // İlk tarama yapılacağını belirtir.
        scanDirectory(); // Dizin içeriğini tarar.
    }
}

// 📌 **Seçili dizini listeden kaldırır ve izlemeyi durdurur**
void MainWindow::removeDirectory()
{
    QString dirPath = pathDisplay->text();
    if (!dirPath.isEmpty()) {
        watcher->removePath(dirPath); // İzleme durdurulur.
        directorySet.remove(dirPath); // İzlenenler listesinden çıkarılır.

        // Arayüzü temizle
        pathDisplay->clear();
        logList->clear();
        fileTimestamps.clear();

        isFirstScan = true; // Yeni bir dizin eklenirse tekrar ilk tarama yapılacak
        logMessage("İzleme durduruldu: " + dirPath);
    }
}

// 📌 **Dizin değiştiğinde tarama yap**
void MainWindow::onDirectoryChanged(const QString &path)
{
    logMessage("Dizin değişti: " + path);
    scanDirectory(); // Dizin içeriğini kontrol eder.
}

// 📌 **Dosya değiştiğinde kaydet**
void MainWindow::onFileChanged(const QString &path)
{
    logMessage("Dosya güncellendi: " + path);
}

// 📌 **Dizindeki tüm dosya ve klasörleri kontrol edip değişiklikleri yakalar**
void MainWindow::scanDirectory()
{
    QString dirPath = pathDisplay->text();
    if (dirPath.isEmpty()) return;

    QDir dir(dirPath);
    QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

    QMap<QString, QDateTime> currentFiles;

    for (const QFileInfo &file : fileList) {
        QString filePath = file.absoluteFilePath();
        QDateTime lastModified = file.lastModified();

        currentFiles[filePath] = lastModified;

        // Yeni eklenen dosya veya klasörleri tespit eder.
        if (!fileTimestamps.contains(filePath)) {
            if (file.isDir()) {
                logMessage(isFirstScan ? "Dizin içerisinde bulunan klasör: " + file.fileName() : "Yeni klasör eklendi: " + file.fileName());
            } else {
                logMessage(isFirstScan ? "Dizin içerisinde bulunan dosya: " + file.fileName() : "Yeni dosya eklendi: " + file.fileName());
            }
        }
        // Dosya güncellendi mi kontrol edilir.
        else if (fileTimestamps[filePath] != lastModified) {
            logMessage("Dosya güncellendi: " + file.fileName());
        }
    }

    // Silinen dosya veya klasörleri tespit eder
    for (const QString &oldFilePath : fileTimestamps.keys()) {
        if (!currentFiles.contains(oldFilePath)) {
            QFileInfo info(oldFilePath);
            logMessage(info.isDir() ? "Klasör silindi: " + info.fileName() : "Dosya silindi: " + info.fileName());
        }
    }

    fileTimestamps = currentFiles;
    isFirstScan = false; // İlk tarama tamamlandı.
}

// 📌 **Değişiklikleri logList adlı listeye ekler**
void MainWindow::logMessage(const QString &message)
{
    logList->addItem(message); // Listeye mesaj eklenir.
}
