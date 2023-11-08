#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10
#define LED_PIN 7  // Conecta un LED al pin 7

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Crea un objeto MFRC522

void setup() {
  Serial.begin(9600);
  SPI.begin();        // Inicia la comunicación SPI
  mfrc522.PCD_Init();
  Serial.println(); // Inicializa el lector RFID
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);  // Apaga el LED al inicio
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Leer el UID de la tarjeta RFID
    Serial.print("UID de la tarjeta: ");
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i], HEX);
    }
    Serial.println();

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
      // Enciende el LED
      digitalWrite(LED_PIN, HIGH);
        // Puedes ajustar la duración que el LED permanece encendido

    
    } else {
      Serial.println("Tarjeta no válida");
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }
}

