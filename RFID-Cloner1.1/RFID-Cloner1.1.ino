/*
 * Copy the RFID card data into variables and then 
 * scan the second empty card to copy all the data
 * ----------------------------------------------------------------------------
 * Example sketch/program which will try the most used default keys listed in 
 * https://code.google.com/p/mfcuk/wiki/MifareClassicDefaultKeys to dump the
 * block 0 of a MIFARE RFID card using a RFID-RC522 reader.
 * 
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 *
 * More pin layouts for other boards can be found here: https://github.com/miguelbalboa/rfid#pin-layout
 *
 */

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

byte buffer[18];
byte block;
byte waarde[64][16];
MFRC522::StatusCode status;
    
MFRC522::MIFARE_Key key;

// Number of known default keys (hard-coded)
// NOTE: Synchronize the NR_KNOWN_KEYS define with the defaultKeys[] array
#define NR_KNOWN_KEYS   31
// Known keys, see: https://code.google.com/p/mfcuk/wiki/MifareClassicDefaultKeys
byte knownKeys[NR_KNOWN_KEYS][MFRC522::MF_KEY_SIZE] =  {
 { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }, // Chave padrão de fábrica
  { 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 }, // Chave de leitura pública
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // Chave nula
  { 0xB0, 0xA1, 0xB2, 0xC3, 0xD4, 0xE5 }, // Chave usada em sistemas de transporte público
  { 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6 }, // Chave comumente utilizada em sistemas de controle de acesso
  { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66 }, // Chave utilizada em muitos sistemas de controle de acesso
  { 0x61, 0x61, 0x61, 0x61, 0x61, 0x61 }, // Chave utilizada em sistemas de acesso a caixas eletrônicos
  { 0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7 }, // Chave comumente usada em sistemas de transporte
  { 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC }, // Chave usada em muitos sistemas de controle de acesso
  { 0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0 }, // Chave utilizada em muitos sistemas de controle de acesso
  { 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF }, // Chave utilizada em muitos sistemas de controle de acesso
  { 0x4D, 0x3A, 0x99, 0xC3, 0x51, 0xDD }, // Chave utilizada em sistemas de transporte público
  { 0x1A, 0x98, 0x2C, 0x7E, 0x45, 0x28 }, // Chave usada em sistemas de transporte público
  { 0x9E, 0x7F, 0x78, 0x97, 0xA6, 0x78 }, // Chave usada em sistemas de transporte público
  { 0xFF, 0x07, 0x80, 0x69, 0x99, 0x00 }, // Chave usada em muitos sistemas de controle de acesso
  { 0xA0, 0x47, 0x8C, 0xC3, 0x90, 0x91 }, // Chave utilizada em sistemas de transporte público
  { 0xa0, 0xb0, 0xc0, 0xd0, 0xe0, 0xf0 }, // Chave A: A0B0C0D0E0F0
  { 0x1a, 0x2b, 0x3c, 0x4d, 0x5e, 0x6f }, // Chave A: 1A2B3C4D5E6F
  { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff }, // Chave A: FFFFFFFFFFFFFF
  { 0xd3, 0xf7, 0xd3, 0xf7, 0xd3, 0xf7 }, // Chave B: D3F7D3F7D3F7
  { 0xa1, 0xb2, 0xc3, 0xd4, 0xe5, 0xf6 }, // Chave A: A1B2C3D4E5F6
  { 0xb0, 0x7a, 0xfa, 0x22, 0xe5, 0x0f }, // Chave A: B07AFA22E50F
  { 0x0f, 0xac, 0xbf, 0x02, 0x88, 0x76 }, // Chave B: 0FACBF028876
  { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66 }, // Chave A: 112233445566
  { 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC }, // Chave B: 123456789ABC
  { 0x69, 0x6E, 0x66, 0x6F, 0x43, 0x6F }, // Chave A: 696E666F436F
  { 0x99, 0x88, 0x77, 0x66, 0x55, 0x44 }, // Chave B: 998877665544
  { 0xC9, 0xC8, 0x53, 0x50, 0x75, 0x89 }, // Chave A: C9C853507589
  { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 }, // Chave B: 000102030405
  { 0x10, 0x20, 0x30, 0x40, 0x50, 0x60 }, // Chave A: 102030405060
  { 0x21, 0x43, 0x65, 0x87, 0xA9, 0xCB }, // Chave A: 214365879ACB
};

char choice;
/*
 * Initialize.
 */
void setup() {
    Serial.begin(9600);         // Initialize serial communications with the PC
    while (!Serial);            // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
    SPI.begin();                // Init SPI bus
    mfrc522.PCD_Init();         // Init MFRC522 card
    Serial.println(F("Try the most used default keys to print block 0 to 63 of a MIFARE PICC."));
    Serial.println("1.Read card \n2.Write to card \n3.Copy the data.");

    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }
}



 //Via seriele monitor de bytes uitlezen in hexadecimaal
 
void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}
//Via seriele monitor de bytes uitlezen in ASCI

void dump_byte_array1(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.write(buffer[i]);
  }
}

/*
 * Try using the PICC (the tag/card) with the given key to access block 0 to 63.
 * On success, it will show the key details, and dump the block data on Serial.
 *
 * @return true when the given key worked, false otherwise.
 */
 
bool try_key(MFRC522::MIFARE_Key *key)
{
    bool result = false;
    
    for(byte block = 0; block < 64; block++){
      
    // Serial.println(F("Authenticating using key A..."));
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return false;
    }

    // Read block
    byte byteCount = sizeof(buffer);
    status = mfrc522.MIFARE_Read(block, buffer, &byteCount);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Read() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
    }
    else {
        // Successful read
        result = true;
        Serial.print(F("Success with key:"));
        dump_byte_array((*key).keyByte, MFRC522::MF_KEY_SIZE);
        Serial.println();
        
        // Dump block data
        Serial.print(F("Block ")); Serial.print(block); Serial.print(F(":"));
        dump_byte_array1(buffer, 16); //omzetten van hex naar ASCI
        Serial.println();
        
        for (int p = 0; p < 16; p++) //De 16 bits uit de block uitlezen
        {
          waarde [block][p] = buffer[p];
          Serial.print(waarde[block][p]);
          Serial.print(" ");
        }
        
        }
    }
    Serial.println();
    
    Serial.println("1.Read card \n2.Write to card \n3.Copy the data.");

    mfrc522.PICC_HaltA();       // Halt PICC
    mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
    return result;
    
    start();
}

/*
 * Main loop.
 */
void loop() {
  start();
    
}

void start(){
  choice = Serial.read();
  
  if(choice == '1')
  {
    Serial.println("Read the card");
    keuze1();
      
    }
    else if(choice == '2')
    {
      Serial.println("See what is in the variables");
      keuze2();
    }
    else if(choice == '3')
    {
      Serial.println("Copying the data on to the new card");
      keuze3();
    }
}

void keuze2(){ //Test waardes in blokken
  
  for(block = 4; block <= 62; block++){
    if(block == 7 || block == 11 || block == 15 || block == 19 || block == 23 || block == 27 || block == 31 || block == 35 || block == 39 || block == 43 || block == 47 || block == 51 || block == 55 || block == 59){
      block ++;
    }
  
  Serial.print(F("Writing data into block ")); 
  Serial.print(block);
  Serial.println("\n");
  
    for(int j = 0; j < 16; j++){
      Serial.print(waarde[block][j]);
      Serial.print(" ");
    }
    Serial.println("\n");
    
  }
  
  Serial.println("1.Read card \n2.Write to card \n3.Copy the data.");
  start();
}

void keuze3(){ //Copy the data in the new card
Serial.println("Insert new card...");
  // Look for new cards
    if ( ! mfrc522.PICC_IsNewCardPresent())
        return;

    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial())
        return;

    // Show some details of the PICC (that is: the tag/card)
    Serial.print(F("Card UID:"));
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println();
    Serial.print(F("PICC type: "));
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    Serial.println(mfrc522.PICC_GetTypeName(piccType));
    
    // Try the known default keys
    MFRC522::MIFARE_Key key;
    for (byte k = 0; k < NR_KNOWN_KEYS; k++) {
        // Copy the known key into the MIFARE_Key structure
        for (byte i = 0; i < MFRC522::MF_KEY_SIZE; i++) {
            key.keyByte[i] = knownKeys[k][i];
        }
    }
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }

  for(int i = 4; i <= 62; i++){ //De blocken 4 tot 62 kopieren, behalve al deze onderstaande blocken (omdat deze de authenticatie blokken zijn)
    if(i == 7 || i == 11 || i == 15 || i == 19 || i == 23 || i == 27 || i == 31 || i == 35 || i == 39 || i == 43 || i == 47 || i == 51 || i == 55 || i == 59){
      i++;
    }
    block = i;
    
      // Authenticate using key A
    Serial.println(F("Authenticating using key A..."));
    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }
    
    // Authenticate using key B
    Serial.println(F("Authenticating again using key B..."));
    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, block, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }
    
    // Write data to the block
    Serial.print(F("Writing data into block ")); 
    Serial.print(block);
    Serial.println("\n");
          
    dump_byte_array(waarde[block], 16); 
    
          
     status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(block, waarde[block], 16);
      if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Write() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
      }
    
        
     Serial.println("\n");
     
  }
  mfrc522.PICC_HaltA();       // Halt PICC
  mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
  
  Serial.println("1.Read card \n2.Write to card \n3.Copy the data.");
  start();
}

void keuze1(){ //Read card
  Serial.println("Insert card...");
  // Look for new cards
    if ( ! mfrc522.PICC_IsNewCardPresent())
        return;

    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial())
        return;

    // Show some details of the PICC (that is: the tag/card)
    Serial.print(F("Card UID:"));
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println();
    Serial.print(F("PICC type: "));
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    Serial.println(mfrc522.PICC_GetTypeName(piccType));
    
    // Try the known default keys
    MFRC522::MIFARE_Key key;
    for (byte k = 0; k < NR_KNOWN_KEYS; k++) {
        // Copy the known key into the MIFARE_Key structure
        for (byte i = 0; i < MFRC522::MF_KEY_SIZE; i++) {
            key.keyByte[i] = knownKeys[k][i];
        }
        // Try the key
        if (try_key(&key)) {
            // Found and reported on the key and block,
            // no need to try other keys for this PICC
            break;
        }
    }
}
