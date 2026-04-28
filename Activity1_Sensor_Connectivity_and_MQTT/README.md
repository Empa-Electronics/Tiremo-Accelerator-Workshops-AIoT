<p align="center">
    <img src="../Additionals/Empa-Accelerator-Workshops-Template-Banner.png" alt="Tiremo® Accelerator Workshops" 
    style="display: block; margin: 0 auto"/>
</p>

# 1) Sensör Bağlanabilirliği ve MQTT
Empa Electronics tarafından düzenlenen Accelerators Workshops University etkinliğimize hoş geldiniz. Bu kılavuz, modern makine öğrenimi kütüphaneleri ve standart yaklaşımlarla geliştilecek "Uçtan Uca MQTT Veri Toplayıcı" uygulamamızın geliştirme adımlarında size rehberlik edecektir.

## Kurulum
Öncelikle, aşağıdaki bağlantıyı kullanarak çalışma ortamı kurulum adımlarını takip ediniz.
### ↳ [Çalışma Ortamı Kurulumu](../Kurulum.md)
Her iki aktivite için ortak olan gerekli program ve gereçlerin kurulum adımlarını içerir.

## MCU Projesi
Aktivite firmware'i `Project_MQTT/MQTT_Project/` klasöründe yer almaktadır. STM32CubeIDE'de bu klasörü açarak derleme ve yükleme adımlarını gerçekleştirebilirsiniz.

## Sensör Bağlanabilirliği ve MQTT

**Sensör Bağlanabilirliği**  
Sensör bağlanabilirliği, bir sistemin çevreden veri toplamasını ve bu verileri işleyebilmesini sağlayan temel bir özelliktir. IoT projelerinde, sensörlerin sisteme entegrasyonu genellikle analog ya da dijital veri çıkışlarına uygun arabirimler üzerinden gerçekleştirilir. Kullanılan platform, sensör verilerini gerçek zamanlı olarak okuyup işlemeye olanak sağlayacak ADC (Analog-Digital Converter), I2C, SPI gibi protokolleri destekler. Bu projede, sensörlerin sorunsuz bir şekilde bağlanabilmesi ve konfigürasyonlarının yapılabilmesi için ABOV A34G43x HAL kütüphanelerinin sunduğu hazır fonksiyonlar kullanılmıştır. Sensör verilerinin doğru bir şekilde alınması ve işlenmesi, sistemin genel başarımını doğrudan etkileyen kritik bir adımdır.

**MQTT Protokolü & AWS**  
MQTT (Message Queuing Telemetry Transport), IoT uygulamaları için geliştirilmiş, hafif yapısı ve düşük bant genişliği gereksinimi ile öne çıkan bir mesajlaşma protokolüdür. Bu protokol, sensör verilerinin bulut tabanlı platformlara hızlı ve güvenli bir şekilde iletilmesini sağlar. AWS IoT Core ile entegre edilen bir MQTT altyapısı, cihazdan buluta veri akışını kolaylaştırmakta ve bulut üzerinden alınan verilerin işlenip analiz edilmesine olanak tanımaktadır. Bu projede, AWS IoT Core servislerinin kullanımıyla MQTT protokolü üzerinden uç birimden alınan veriler güvenli bir şekilde AWS platformuna aktarılmış ve burada farklı analiz senaryolarına uygun iş akışları oluşturulmuştur.

**Kaynaklar & Okuma Önerileri** 

1- [What is MQTT? - AWS](https://aws.amazon.com/what-is/mqtt/)
