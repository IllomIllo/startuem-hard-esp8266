
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecureBearSSL.h>

#include <SPI.h>
#include <MFRC522.h>                        // https://github.com/miguelbalboa/rfid

#define SS_PIN D2                           // SDA - D2
#define RST_PIN D1                          // RST - D1
MFRC522 mfrc522(SS_PIN, RST_PIN);           // Создаём экземпляр класса MFRC522.

#define ON_Board_LED 2                      // Светодиод горит пока карта поднесена к считывателю

const char* ssid = "The GYM";
const char* password = "80Mb11183";
const String readerUID = "001";
const String readerPassword = "Sd23mfm!lsdl2323230000323";
const String readerEmail = "cardreader@mail.ru";
//Fingerprint (might need to be updated)
//const uint8_t fingerprint[20] = {0x7A, 0x19, 0x06, 0xB0, 0x69, 0x72, 0xA3, 0x77, 0xA4, 0x92, 0xFE, 0xA2, 0x29, 0x59, 0xD2, 0x87, 0x76, 0xF7, 0x77, 0xFE};
const char IRG_Root_X1 [] PROGMEM = R"CERT(
-----BEGIN CERTIFICATE-----
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
-----END CERTIFICATE-----
)CERT";

ESP8266WebServer server(80);                // Сервер на 80 порту

int readsuccess;
byte readcard[4];
char str[32] = "";
String StrUID;

X509List cert(IRG_Root_X1);

void setup() {
  Serial.begin(115200);                     // Скорость серийного монитора
  SPI.begin();                              // Инициализация шины SPI
  mfrc522.PCD_Init();                       // Инициализация считывателя MFRC522

  delay(500);

  WiFi.begin(ssid, password);               // Соединение с WiFi роутером
  Serial.println("Connecting to WiFi ..");

  pinMode(ON_Board_LED, OUTPUT);
  digitalWrite(ON_Board_LED, HIGH);         // Выключаем светодиод

   Serial.print("Подключение ");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
// Мигает при подключении к роутеру
    digitalWrite(ON_Board_LED, LOW);
    delay(250);
    digitalWrite(ON_Board_LED, HIGH);
    delay(250);
  }
  //--
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
  //--
  digitalWrite(ON_Board_LED, HIGH);         // Выключаем светодиод после соединения с роутером
// Выводим IP адрес при подключении к WIFI сети
  Serial.println("");
  Serial.print("Подключение к сети : ");
  Serial.println(ssid);
  Serial.print("IP адрес: ");
  Serial.println(WiFi.localIP());

  Serial.println("Приложите карту или брелок, чтобы увидеть UID");
  Serial.println("");
}

void loop() {
  readsuccess = getid();                    // Получаем ID карты в функции getid() и сохраняем в переменную

  if (readsuccess) {
    //
    /*std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setFingerprint(fingerprint);*/
    //
    WiFiClientSecure client;
    //
    digitalWrite(ON_Board_LED, LOW);
    client.setTrustAnchors(&cert);
    HTTPClient https;                        // Объявляем объект класса HttpClient

    String userUIDresultSend, postData, readerUIDresultSend;
    userUIDresultSend = StrUID;
    readerUIDresultSend = readerUID;         //?? передали нашу const str "номер считываетля" в переменную class string
    //postData = "{ \"userUID\": \"" + userUIDresultSend + "\" }";  // Переменная для хранения запроса "userUIDresult=UIDresultSend"

    postData = "{ \"userUID\": \"" + userUIDresultSend + "\" , \"readerUID\": \"" + readerUIDresultSend + "\" , \"email\": \"" + readerEmail + "\" , \"password\": \"" + readerPassword + "\"}";

    https.begin(client, "https://startuem-backend-production-b2ba.up.railway.app/cardReader/postUID");  // URL адрес к PHP скрипту
    https.addHeader("Content-Type", "application/json");              // Специальный заголовок типа содержимого

    int httpCode = https.POST(postData);       // Отправляем запрос

    Serial.println(userUIDresultSend);        // Печатаем UID
    Serial.println(httpCode);                 // Печатаем код возврата HTTP

    https.end();
    Serial.print("https end");                               // Закрываем соединение
    delay(1000);
    digitalWrite(ON_Board_LED, HIGH);
  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

// Процедура считывания и получения UID с карты или брелка
int getid() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return 0;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return 0;
  }


  Serial.print("UID ОТСКАНИРОВАННОЙ КАРТЫ : ");

  for (int i = 0; i < 4; i++) {
    readcard[i] = mfrc522.uid.uidByte[i];     // сохранение UID тега в считываемой карте
    array_to_string(readcard, 4, str);
    StrUID = str;
  }
  mfrc522.PICC_HaltA();
  return 1;
}

// Процедура изменения результата считывания UID массива в строку
void array_to_string(byte array[], unsigned int len, char buffer[]) {
  for (unsigned int i = 0; i < len; i++)
  {
    byte nib1 = (array[i] >> 4) & 0x0F;
    byte nib2 = (array[i] >> 0) & 0x0F;
    buffer[i * 2 + 0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
    buffer[i * 2 + 1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
  }
  buffer[len * 2] = '\0';
}
