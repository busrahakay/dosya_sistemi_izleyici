//uygulamanın temel yapısı oluşturulur.
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemWatcher> //Qt API'leri kullanıldı.
#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QMap>
#include <QDateTime>
#include <QSet>

class MainWindow : public QMainWindow
{
    Q_OBJECT// Qt'nin sinyal-slot mekanizmasını kullanabilmek için

public:
    explicit MainWindow(QWidget *parent = nullptr);//yapıcı fonk.
    ~MainWindow();//yıkıcı fonk.

private slots:
    void addDirectory();          // İzlenecek dizini ekler
    void removeDirectory();       // Seçili dizini kaldırır
    void scanDirectory();         // Dizin içeriğini kontrol eder
    void onDirectoryChanged(const QString &path); // Dizin değişince tetiklenir
    void onFileChanged(const QString &path);     // Dosya değişince tetiklenir

private:
    QFileSystemWatcher *watcher;
    QVBoxLayout *layout;
    QLineEdit *pathDisplay;  // Dizin adını gösterecek
    QPushButton *addButton;
    QPushButton *removeButton;
    QListWidget *logList;    // Değişiklikleri gösterecek
    QMap<QString, QDateTime> fileTimestamps;
    QSet<QString> directorySet;
    bool isFirstScan = true; // İlk tarama kontrolü

    void setupUI();  // Arayüz
    void logMessage(const QString &message);// Değişiklikleri log'a ekler
};

#endif // MAINWINDOW_H
