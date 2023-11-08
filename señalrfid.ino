#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10

MFRC522 mfrc522(SS_PIN, RST_PIN);  

void setup() {
  Serial.begin(9600);
  SPI.begin();        
  mfrc522.PCD_Init(); 
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Leer el UID de la tarjeta RFID
    Serial.print("UID de la tarjeta: ");
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println();

    // Define el UID de la tarjeta permitida
    byte allowedUID[] = {0xA9, 0xA3, 0x1D, 0xB4}; 

    // Comprueba si la tarjeta es válida
    bool isCardValid = true;
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      if (mfrc522.uid.uidByte[i] != allowedUID[i]) {
        isCardValid = false;
        break;
      }
    }

    if (isCardValid) {
      // Envía una señal por el puerto serial
      Serial.println("Tarjeta válida detectada");
    } else {
      Serial.println("Tarjeta no válida");
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }
}
