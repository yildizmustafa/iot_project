#include <Firebase.h>
#include <FirebaseArduino.h>
#include <FirebaseError.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define firebase_url "proje-led.firebaseio.com"//firebase proje url
#define firebase_anahtar "LF4lVNAO0BmCzful9JMJ0yVaH4YSMUJbQrONpPi5"//veritabanı gizli anahtarı
#define LED D2 //led d2 pinine bağlı

String apiKey = "2X7CQM13P1LEVE7H";//thingspeak read keyi
const char* ssid = "Redmi";
const char* password = "q12we34rt56";

const char* server = "api.thingspeak.com";

WiFiClient client;
void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, 0);
  Serial.begin(115200);
  pinMode(A0, INPUT);

  delay(10);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Firebase.begin(firebase_url, firebase_anahtar);
  Firebase.setString("ledDurumu", "false");

}


void loop() {
  int temp = analogRead(A0);
  temp = map(temp, 0, 1023, 100, 0);//0-100 donusumu

  if (client.connect(server, 80)) { //   "184.106.153.149" veya api.thingspeak.com
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(temp);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    
//thingspeak e sensorden gelen veri aktarıldı
    Serial.print("Sensor verileri: ");
    Serial.print(temp);
    Serial.println("veriler thingspeak'e aktarıldı.");
  }
///////////////////////////////////////////firebase led 
  Firebase.set("sensor_verisi",temp);
  if (Firebase.getString("ledDurumu") == "true")
  {
    digitalWrite(LED, HIGH); 
  }
  else if(Firebase.getString("ledDurumu") == "false")
  {
    digitalWrite(LED, LOW); 
  }
  if (Firebase.failed()) {

Serial.println(Firebase.error());
return;
}
/////////////////////////////////////////////////



  client.stop();


  Serial.println("...");
  delay(3000);
}
