# 🗂️ Dosya Sistemi İzleyici (File System Watcher)  
Belirli bir klasörü izleyerek, klasör içinde gerçekleşen dosya/dizin değişikliklerini anlık olarak tespit edip kullanıcıya bildiren modern bir masaüstü uygulaması.

> 💻 Geliştirme Ortamı: Windows 10 + Qt Creator  
> 🎓 Proje, "İşletim Sistemleri" dersi kapsamında gerçekleştirilmiştir.

---

## ✨ Özellikler

📁 Belirli bir dizindeki dosya ve klasörlerin izlenmesi  
🔄 Gerçek zamanlı dosya/dizin değişikliği takibi  
📝 Anlamlı ve filtrelenmiş loglama sistemi  
🖥️ Koyu (dark) tema arayüz desteği  
🎯 Modern ve kullanıcı dostu tasarım  
🔒 Bellek sızıntılarına karşı geliştirilmiş bellek yönetimi

---

## 📦 Kullanılan Teknolojiler

- 🖥️ **Qt Framework** – GUI ve dosya sistemi izleme (QFileSystemWatcher)  
- 📝 **C++** – Performanslı sistem seviyesi programlama  
- 🎨 **Qt Style Sheets (QSS)** – Arayüz temalandırma (dark mode)  
- 🧪 **Signal & Slot Mekanizması** – Olay tabanlı programlama yapısı

---

## 🛠️ Derleme ve Çalıştırma (Qt Creator)

1. Proje klasörünü Qt Creator ile açın (`.pro` dosyası üzerinden).  
2. Gerekli Qt sürümünü seçin (örneğin: Qt 5.15).  
3. "Run" tuşuna basarak uygulamayı başlatın.  

---

## 🔧 Uygulama çalıştığında, kullanıcı belirli bir dizini seçerek bu dizin içindeki değişiklikleri (oluşturma, silme, yeniden adlandırma vb.) gerçek zamanlı olarak log ekranında görebilir. (Uygulama arayüzü ve işlevi görseli result_image.jpg)
