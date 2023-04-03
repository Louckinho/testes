/*
 * ----------------------------------------------------------------------------
 * This is a MFRC522 library example; see https://github.com/miguelbalboa/rfid
 * for further details and other examples.
 * 
 * NOTE: The library file MFRC522.h has a lot of useful info. Please read it.
 * 
 * Released into the public domain.
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
#pragma GCC optimize("O3")

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

// Number of known default keys (hard-coded)
// NOTE: Synchronize the NR_KNOWN_KEYS define with the defaultKeys[] array
#define NR_KNOWN_KEYS   95
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
{ 0xAF, 0xE4, 0x44, 0xC4, 0xBC, 0xAA, } ,
{ 0xB8, 0x0C, 0xC6, 0xDE, 0x9A, 0x03, } ,
{ 0xA8, 0x33, 0xFE, 0x5A, 0x4B, 0x55, } ,
{ 0xB5, 0x33, 0xCC, 0xD5, 0xF6, 0xBF, } ,
{ 0xB7, 0x51, 0x3B, 0xFF, 0x58, 0x7C, } ,
{ 0xB6, 0xDF, 0x25, 0x35, 0x36, 0x54, } ,
{ 0x91, 0x28, 0xA4, 0xEF, 0x4C, 0x05, } ,
{ 0xA9, 0xD4, 0xB9, 0x33, 0xB0, 0x7A, } ,
{ 0xA0, 0x00, 0xD4, 0x2D, 0x24, 0x45, } ,
{ 0xAA, 0x5B, 0x6C, 0x7D, 0x88, 0xB4, } ,
{ 0xB5, 0xAD, 0xEF, 0xCA, 0x46, 0xC4, } ,
{ 0xBF, 0x3F, 0xE4, 0x76, 0x37, 0xEC, } ,
{ 0xB2, 0x90, 0x40, 0x1B, 0x0C, 0xAD, } ,
{ 0xAD, 0x11, 0x00, 0x6B, 0x06, 0x01, } ,
{ 0xA0, 0xA1, 0xA2, 0xA8, 0xA4, 0xA5, } ,
{ 0x0D, 0x60, 0x57, 0xE8, 0x13, 0x3B, } ,
{ 0xD3, 0xF3, 0xB9, 0x58, 0xB8, 0xA3, } ,
{ 0x6A, 0x68, 0xA7, 0xD8, 0x3E, 0x11, } ,
{ 0x7C, 0x46, 0x9F, 0xE8, 0x68, 0x55, } ,
{ 0xE4, 0x41, 0x0E, 0xF8, 0xED, 0x2D, } ,
{ 0x3E, 0x12, 0x05, 0x68, 0xA3, 0x5C, } ,
{ 0xCE, 0x99, 0xFB, 0xC8, 0xBD, 0x26, } ,
{ 0x21, 0x96, 0xFA, 0xD8, 0x11, 0x5B, } ,
{ 0x00, 0x9F, 0xB4, 0x2D, 0x98, 0xED, } ,
{ 0x00, 0x2E, 0x62, 0x6E, 0x28, 0x20, } ,
{ 0x03, 0x8B, 0x5F, 0x9B, 0x5A, 0x2A, } ,
{ 0x04, 0xDC, 0x35, 0x27, 0x76, 0x35, } ,
{ 0x0C, 0x42, 0x0A, 0x20, 0xE0, 0x56, } ,
{ 0x15, 0x2F, 0xD0, 0xC4, 0x20, 0xA7, } ,
{ 0x29, 0x6F, 0xC3, 0x17, 0xA5, 0x13, } ,
{ 0x29, 0xC3, 0x5F, 0xA0, 0x68, 0xFB, } ,
{ 0x31, 0xBE, 0xC3, 0xD9, 0xE5, 0x10, } ,
{ 0x46, 0x22, 0x25, 0xCD, 0x34, 0xCF, } ,
{ 0x4B, 0x7C, 0xB2, 0x53, 0x54, 0xD3, } ,
{ 0x55, 0x83, 0x69, 0x8D, 0xF0, 0x85, } ,
{ 0x57, 0x8A, 0x9A, 0xDA, 0x41, 0xE3, } ,
{ 0x6F, 0x95, 0x88, 0x7A, 0x4F, 0xD3, } ,
{ 0x76, 0x00, 0xE8, 0x89, 0xAD, 0xF9, } ,
{ 0x86, 0x12, 0x0E, 0x48, 0x8A, 0xBF, } ,
{ 0x88, 0x18, 0xA9, 0xC5, 0xD4, 0x06, } ,
{ 0x8C, 0x90, 0xC7, 0x0C, 0xFF, 0x4A, } ,
{ 0x8E, 0x65, 0xB3, 0xAF, 0x7D, 0x22, } ,
{ 0x97, 0x64, 0xFE, 0xC3, 0x15, 0x4A, } ,
{ 0x9B, 0xA2, 0x41, 0xDB, 0x3F, 0x56, } ,
{ 0xAD, 0x2B, 0xDC, 0x09, 0x70, 0x23, } ,
{ 0xB0, 0xA2, 0xAA, 0xF3, 0xA1, 0xBA, } ,
{ 0xB6, 0x9D, 0x40, 0xD1, 0xA4, 0x39, } ,
{ 0xC9, 0x56, 0xC3, 0xB8, 0x0D, 0xA3, } ,
{ 0xCA, 0x96, 0xA4, 0x87, 0xDE, 0x0B, } ,
{ 0xD0, 0xA4, 0x13, 0x1F, 0xB2, 0x90, } ,
{ 0xD2, 0x70, 0x58, 0xC6, 0xE2, 0xC7, } ,
{ 0xE1, 0x95, 0x04, 0xC3, 0x94, 0x61, } ,
{ 0xFA, 0x1F, 0xBB, 0x3F, 0x0F, 0x1F, } ,
{ 0xFF, 0x16, 0x01, 0x4F, 0xEF, 0xC7, } ,
{ 0x00, 0x01, 0x31, 0xB9, 0x3F, 0x28, } ,
  
};

/*
 * Initialize.
 */
void setup() {
    Serial.begin(9600);         // Initialize serial communications with the PC
    while (!Serial);            // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
    SPI.begin();                // Init SPI bus
    mfrc522.PCD_Init();         // Init MFRC522 card
    Serial.println(F("Try the most used default keys to print block 0 of a MIFARE PICC."));
}

/*
 * Helper routine to dump a byte array as hex values to Serial.
 */
void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}

/*
 * Try using the PICC (the tag/card) with the given key to access block 0.
 * On success, it will show the key details, and dump the block data on Serial.
 *
 * @return true when the given key worked, false otherwise.
 */
bool try_key(MFRC522::MIFARE_Key *key)
{
    bool result = false;
    byte buffer[18];
    byte block = 0;
    MFRC522::StatusCode status;

    // Serial.println(F("Authenticating using key A..."));
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        //Serial.print(F("PCD_Authenticate() failed: "));
         //Serial.println(mfrc522.GetStatusCodeName(status));
        //delay(1000);
        return false;
    }

    // Read block
    byte byteCount = sizeof(buffer);
    status = mfrc522.MIFARE_Read(block, buffer, &byteCount);
    if (status != MFRC522::STATUS_OK) {
        // Serial.print(F("MIFARE_Read() failed: "));
        // Serial.println(mfrc522.GetStatusCodeName(status));
    }
    else {
        // Successful read
        result = true;
        Serial.print(F("Success with key:"));
        dump_byte_array((*key).keyByte, MFRC522::MF_KEY_SIZE);
        Serial.println();
        // Dump block data
        Serial.print(F("Block ")); Serial.print(block); Serial.print(F(":"));
        dump_byte_array(buffer, 16);
        Serial.println();
    }
    Serial.println();

    mfrc522.PICC_HaltA();       // Halt PICC
    mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
    return result;
}

/*
 * Main loop.
 */
void loop() {
    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
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
        
        // http://arduino.stackexchange.com/a/14316
        if ( ! mfrc522.PICC_IsNewCardPresent())
            break;
        if ( ! mfrc522.PICC_ReadCardSerial())
            break;
    }
}
