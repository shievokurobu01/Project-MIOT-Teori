// M AKMAL SHIEVO BARUS (5200411446)
// Fakultas sains dan teknologi
// Informatika
// Universitas Teknologi Yogyakarta

//ALARM 
//project IoT menggunakan ESP32 dan sensor HC-SR04 Ultrasonic

//define variabel
#define trigerPin 12
#define echoPin 13
#define ledPin 14
#define speakerPin 2
#define pitch 262
#define CM_TO_INCH 0.393701
#define BUZZER_PIN 2//

long duration;
float distance;
float distanceInch;

//menghubungkan ke wifi
#include <WiFi.h>

const char* ssid = "Wokwi-GUEST";
const char* pass = "";

WiFiClient alfa;

//memakai library thingspeak
#include <ThingSpeak.h>

unsigned long ChannelID = 1995126;
const char * WriteAPI = "35QBNHEV8PRGNXWO";

void setup() {
  //setup untuk sensor
  Serial.begin(115200);
  pinMode(trigerPin, OUTPUT);
  pinMode(speakerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //setup untuk led
  pinMode(ledPin, OUTPUT);

  //setup untuk speaker
  pinMode(speakerPin, OUTPUT);
  

  if (distance>10){
    pinMode(echoPin, INPUT);
    pinMode(speakerPin, OUTPUT);
    tone(speakerPin, 1000);
  delay(1000);
  }

  //scan WiFi
  Serial.println("Scan WiFi...");
  int networks = WiFi.scanNetworks();
  if(networks == 0){
    Serial.println("No WiFi Networks!");
  }
  else{
    Serial.print(networks);
    Serial.println("Networks Found!");
    for(int i = 0; i < networks; i++)
      Serial.println(WiFi.SSID());
      delay(10);
  }

  //menyambungkan ke wifi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED){
    delay(100);
    Serial.println(".");
  }
  Serial.println("WiFi Connected!");
  Serial.println(WiFi.localIP());

  //memulai program thingspeak
  ThingSpeak.begin(alfa);

}

void loop() {
  
 //looping sensor (membuat sensor wave)
  digitalWrite(trigerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigerPin, LOW);

  //membuat durasi
  duration = pulseIn(echoPin, HIGH);

  //mengkalkulasi jarak
  distance = (duration/2) * 0.0343;
  if (distance < 20) {  // if the distance is less than 20 cm
    tone(BUZZER_PIN, 440, 1000);
    delay(1000); 
  }

  // Convert to inches
  distanceInch = distance * CM_TO_INCH;

  //concider max width of the door = 200 cm
 int x = ThingSpeak.writeField (ChannelID, 1, distance, WriteAPI);

  //mengambil data dari thingspeak
  if(distance>100){
    Serial.println("Aman boss");
    Serial.println("distance(cm): " + String(distance));    
  }

  else{
    Serial.println("Rumah Anda Ada Pencuri");
    Serial.println("Segeralah Datang Ke Rumah");
  }
}
