
#include <SPI.h>
#include <LoRa.h>

#define LORA_SS    5   // NSS / CS pin from LoRa -> ESP32 GPIO10
#define LORA_RST    14   // RESET pin from LoRa -> ESP32 GPIO9
#define LORA_DIO0  26   // DIO0 pin from LoRa -> ESP32 GPIO25

#define LORA_SCK   18   // SCK  -> ESP32 GPIO18
#define LORA_MISO  19   // MISO -> ESP32 GPIO19
#define LORA_MOSI  23   // MOSI -> ESP32 GPIO13

#define BUTTON_PIN 27   // زر بين هذا البن و GND

#define LORA_FREQ 433E6 // لو موديولك 868 MHz أو 915 MHz غيريها هنا وهناك

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Booting sender...");

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI);

  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(LORA_FREQ)) {
    Serial.println("LoRa init failed on sender!");
    while (true) {
      delay(1000);
    }
  }

  LoRa.setSpreadingFactor(12);       
  LoRa.setSignalBandwidth(125E3);    
  LoRa.setCodingRate4(5);            

  Serial.println("Sender ready ✅");
}

void loop() {
  static int lastState = HIGH;
  int currentState = digitalRead(BUTTON_PIN);

  if (currentState == LOW && lastState == HIGH) {
    Serial.println("Button pressed -> sending packet...");

    LoRa.beginPacket();
    LoRa.print("BTN_PRESSED");
    LoRa.endPacket();

    Serial.println("Packet sent ✔");
  }

  lastState = currentState;
}
