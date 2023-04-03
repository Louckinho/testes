#include <MFRC522.h>
#include <SPI.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Cria o objeto MFRC522

void setup() {
  Serial.begin(9600);   // Inicia a comunicação serial
  SPI.begin();          // Inicia a comunicação SPI
  mfrc522.PCD_Init();   // Inicia o leitor RFID
}

void loop() {
  // Verifica se há cartões presentes
   MFRC522::Uid uid = rfid.uid; // Define a variável "uid" com o UID do cartão
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Imprime o UID do cartão
    Serial.print("UID tag :");
    String conteudo = "";
    byte letra;
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(mfrc522.uid.uidByte[i], HEX);
      conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
      conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    Serial.println();
    Serial.print("MIFARE key: ");
    
    // Verifica o tipo de cartão MIFARE
    MFRC522::MIFARE_Key key;
    byte tipo = mfrc522.PICC_GetType(mfrc522.uid.sak);
    if (tipo == MFRC522::PICC_TYPE_MIFARE_MINI || tipo == MFRC522::PICC_TYPE_MIFARE_1K || tipo == MFRC522::PICC_TYPE_MIFARE_4K) {
      // Lê a chave de segurança do cartão MIFARE Classic
      for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF; // Preenche a chave com o valor padrão
      }
      Serial.println("MIFARE Classic card detected");
      Serial.print("The NUID tag is:");
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
      }
      Serial.println();
      Serial.print("Using key (for A) ");
      for (byte i = 0; i < 6; i++) {
        Serial.write(' ');
        Serial.print(key.keyByte[i] < 0x10 ? "0" : "");
        Serial.print(key.keyByte[i], HEX);
      }
    } else if (tipo == MFRC522::PICC_TYPE_MIFARE_UL) {
      Serial.println("MIFARE Ultralight card detected");
    } else {
      Serial.print("Card UID not supported");
    }
    
    // Exibe o conteúdo do cartão, caso seja possível
    if (mfrc522.PICC_DumpToSerial(&uid, &key))

 {
      Serial.println();
      Serial.println(conteudo);
    }
    Serial.println();
    delay(1000);
  }
}
