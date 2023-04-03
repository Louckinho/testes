#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define LED_READ 6
#define LED_ACCESS 7
#define LED_SUCCESS 8

MFRC522 mfrc522(SS_PIN, RST_PIN); // instância do leitor RFID

byte keys[][6] = {
  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},  // Chave padrão de fábrica
  {0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5},  // Chave genérica 1
  {0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5},  // Chave genérica 2
  {0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5},  // Chave genérica 3
  {0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7},  // Chave MIFARE MAD 1K
  {0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0},  // Chave MIFARE MAD 4K
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Chave nula
  {0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00},  // Chave de teste
  {0x4D, 0x3A, 0x99, 0xC3, 0x51, 0xDD},  // Chave NFC Forum
  {0x1A, 0x98, 0x2C, 0x7E, 0x45, 0x9A}   // Chave NFC Forum
};

void setup() {
  pinMode(LED_READ, OUTPUT);
  pinMode(LED_ACCESS, OUTPUT);
  pinMode(LED_SUCCESS, OUTPUT);
  Serial.begin(9600); // Inicializa a comunicação serial
  SPI.begin();        // Inicializa a interface SPI
  mfrc522.PCD_Init();  // Inicializa o leitor RFID
  Serial.println("Aproxime um cartao RFID...");
  Serial.println();
}

void loop() {
  digitalWrite(LED_READ, HIGH); // acende o led de leitura
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    digitalWrite(LED_READ, LOW); // apaga o led de leitura
    return;
  }
  digitalWrite(LED_READ, LOW); // apaga o led de leitura
  digitalWrite(LED_ACCESS, HIGH); // acende o led de acesso
  Serial.println("Cartao detectado!");

  // Tenta as chaves comuns
  for (int i = 0; i < 10; i++) {
    Serial.print("Tentando chave ");
