#include <SPI.h>
#include <LoRa.h>

#define LORA_SS    5  
#define LORA_RST    14   
#define LORA_DIO0   4  

#define LORA_SCK   18   
#define LORA_MISO  19   
#define LORA_MOSI  13   

#define LED_PIN    21

#define LORA_FREQ 433E6

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); 

  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI);

  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(LORA_FREQ)) {
    Serial.println("LoRa init failed on receiver!");
    while (true) {
      delay(1000);
    }
  }

  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(5);

  Serial.println("Receiver ready ✅ Listening...");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String incoming = "";

    while (LoRa.available()) {
      incoming += (char)LoRa.read();
    }

    Serial.print("Received: ");
    Serial.println(incoming);

    if (incoming == "BTN_PRESSED") {
      digitalWrite(LED_PIN, HIGH); 
      delay(1000);                 
      digitalWrite(LED_PIN, LOW);  
    }
  }
}

