#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <ArduinoJson.h>
String TelegrammBOTtoken = "2029325713:AAEjw3qa3cgnC2lAju7VmEUdbnCljsO1yFQ";// https://api.telegram.org/bot165476586:AAH01HfdhytjhmDSHnpZuUoi6YwLzqaIpVPs/getMe
String Messange = "Сообщение !";
      WiFiClient client;
          HTTPClient https;
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);//
  WiFi.begin("vyacheslav", "1357246800");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
   Serial.println();
   Serial.println("UserName: " + UserName());
   Serial.println("chatid: " + String(chatid()));
   SendMessage(chatid(), UserName() + Messange); // Тут отправляем сообщение.  chatid() необходимо запомнить и вставлять сюда uint32_t вместо chatid() что значительно ускорит работу. UserName() тоже нужно заранее запоминать если будем обращаться по имени.
  ESP.deepSleep(0, RF_NO_CAL);
}

void loop() {
  

  }

String telegramPOST(String jsonarr) {
    
//  String token = TelegrammBOTtoken;
  if (https.begin(client, "https://api.telegram.org/bot" + TelegrammBOTtoken + "/")) { // HTTPS
    https.addHeader("Content-Type", "application/json");
    https.POST(jsonarr);
    String Answer = https.getString();
    //Serial.println(Answer);
    https.end();
    return Answer;
  }
   return "";
}

uint32_t chatid() {
  StaticJsonDocument<30> doc; // Создаем и наполняем json для последующей отправки на сервер
  doc["method"] = "getUpdates";
  doc["offset"] = -1; //Для проверки последнего сообщения
  DynamicJsonDocument Answer(1532);
  deserializeJson(Answer, telegramPOST(doc.as<String>()));                     // Парсим JSON-содержимое ответа сервера
  //   serializeJsonPretty(Answer, Serial);    Serial.println();
  return Answer["result"][0]["message"]["chat"]["id"];//id
}

String UserName() {
  StaticJsonDocument<30> doc; // Создаем и наполняем json для последующей отправки на сервер
  doc["method"] = "getUpdates";
  doc["offset"] = -1; //Для проверки последнего сообщения
  DynamicJsonDocument Answer(1532);
  deserializeJson(Answer, telegramPOST(doc.as<String>()));                     // Парсим JSON-содержимое ответа сервера
  //const char* first_name = Answer["result"][0]["message"]["from"]["first_name"];
  return Answer["result"][0]["message"]["from"]["first_name"];//id
}

void SendMessage(uint32_t chatid, String message) {
  StaticJsonDocument<100> doc; // Создаем и наполняем json для последующей отправки на сервер
  doc["method"] = "sendMessage";
  doc["chat_id"] = chatid;
  doc["text"] = message;
  DynamicJsonDocument Answer(1532);
  deserializeJson(Answer, telegramPOST(doc.as<String>()));                     // Парсим JSON-содержимое ответа сервера
  analogWrite(LED_BUILTIN, 0);
}
