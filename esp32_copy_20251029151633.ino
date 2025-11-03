// SENDER CODE - upload to ESP32 (العقدة اللي فيها الزر)

#include <SPI.h>
#include <LoRa.h>

// === LoRa pin mapping on YOUR ESP32 (Transmitter) ===
// غيّري الأرقام هنا لو أسلاكك مختلفة
#define LORA_SS    5   // NSS / CS pin from LoRa -> ESP32 GPIO10
#define LORA_RST    14   // RESET pin from LoRa -> ESP32 GPIO9
#define LORA_DIO0  26   // DIO0 pin from LoRa -> ESP32 GPIO25

#define LORA_SCK   18   // SCK  -> ESP32 GPIO18
#define LORA_MISO  19   // MISO -> ESP32 GPIO19
#define LORA_MOSI  23   // MOSI -> ESP32 GPIO13

// الزر
#define BUTTON_PIN 27   // زر بين هذا البن و GND

// نفس التردد في المستقبل
#define LORA_FREQ 433E6 // لو موديولك 868 MHz أو 915 MHz غيريها هنا وهناك

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Booting sender...");

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  // HIGH = مش مضغوط
  // LOW  = مضغوط (لأن الطرف الثاني للزر رايح GND)

  // تهيئة SPI مع البنّات اللي اخترناها
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI);

  // تعريف البنّات الخاصة بالموديول LoRa
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

  // بدء راديو LoRa على التردد
  if (!LoRa.begin(LORA_FREQ)) {
    Serial.println("LoRa init failed on sender!");
    while (true) {
      delay(1000);
    }
  }

  // إعدادات الاتصال (لازم تكون نفسها عند المستقبل)
  LoRa.setSpreadingFactor(12);        // 7..12
  LoRa.setSignalBandwidth(125E3);     // 125kHz
  LoRa.setCodingRate4(5);             // 5..8

  Serial.println("Sender ready ✅");
}

void loop() {
  static int lastState = HIGH;
  int currentState = digitalRead(BUTTON_PIN);

  // انتقال من HIGH -> LOW يعني الزر انضغط الآن
  if (currentState == LOW && lastState == HIGH) {
    Serial.println("Button pressed -> sending packet...");

    LoRa.beginPacket();
    LoRa.print("BTN_PRESSED");
    LoRa.endPacket();

    Serial.println("Packet sent ✔");
  }

  lastState = currentState;
}
