#include <SPI.h>
#include <LoRa.h>

// Pins for LoRa module on ESP32-S3 (Receiver node)
#define LORA_SS    5   // NSS / CS
#define LORA_RST    14   // RESET
#define LORA_DIO0   4   // DIO0 (interrupt)

// SPI pins for ESP32-S3 (we will map them manually)
#define LORA_SCK   18   // SCK
#define LORA_MISO  19   // MISO
#define LORA_MOSI  13   // MOSI

// LED pin (goes to LED via resistor on breadboard)
#define LED_PIN    21

// same frequency as sender!
#define LORA_FREQ 433E6

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // LED off at start

  // init SPI for LoRa
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI);

  // tell LoRa which control pins
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(LORA_FREQ)) {
    Serial.println("LoRa init failed on receiver!");
    while (true) {
      delay(1000);
    }
  }

  // يجب تكون نفس الإعدادات في المرسل لمدى مستقر
  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(5);

  Serial.println("Receiver ready ✅ Listening...");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // في باكِت وصلت
    String incoming = "";

    while (LoRa.available()) {
      incoming += (char)LoRa.read();
    }

    Serial.print("Received: ");
    Serial.println(incoming);

    // لو الرسالة من العقدة الأولى فيها طلب تشغيل
    if (incoming == "BTN_PRESSED") {
      digitalWrite(LED_PIN, HIGH); // ولع اللمبة
      delay(1000);                 // خليك شغّال 1 ثانية
      digitalWrite(LED_PIN, LOW);  // طفي
    }
  }
}

