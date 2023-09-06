#include <FirebaseESP8266.h> //
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define WIFI_SSID "SUPAWADEE12_2.4G" // ชื่อWifiที่ต้องเชื่อมต่อ
#define WIFI_PASSWORD "0621796222" // รหัสWifi
#define FIREBASE_HOST "minipro-9f177-default-rtdb.firebaseio.com/" //Urlของfirebase
#define FIREBASE_KEY "CAD9U7DSeNHTNHAobA2Q7rdbF4g2JgL4PF7lwKcE" //Token Firebase
FirebaseData firebaseData; //ดึงข้อมูลในFirebase
const long offsetTime = 25200; //ตั้งเลขข้อมูล
WiFiUDP ntpUDP; //ให้เชื่อมต่อผ่านWifi
NTPClient timeClient(ntpUDP, "pool.ntp.org", offsetTime);//ให้ใช้ Client ใน Firebase
int hourNow, minuteNow, secondNow; // เพิ่มตัวแปลชั่วโมง นาที วินาที 

void setup() {
  Serial.begin(9600); // ให้โชว์ค่าใน Serial 9600
  pinMode(D5, OUTPUT); // ให้ขาD5 เป็นขาส่งออก
  pinMode(D6, OUTPUT); // ให้ขาD5 เป็นขาส่งออก
  connectWifi(); // เชื่อต่อWiFi 
  lcd.begin(); // ตั้งจอ LCD  
  lcd.backlight(); // ตั้งค่าพื้นหลังสีดำ
  Firebase.begin(FIREBASE_HOST, FIREBASE_KEY); // ให้เชื่อมต่อกัน Url และ Token
  timeClient.begin();
}

void loop() {
  // Do something
  timeClient.update();
  Firebase.getString(firebaseData, "/Plug/stateplug");
  String stateplug = firebaseData.stringData(); //สั่งใช้งานปลั๊ก1
  Firebase.getString(firebaseData, "/Plug/stateplug2");
  String stateplug2 = firebaseData.stringData(); //สั่งใช้งานปลั๊ก2
  Firebase.getString(firebaseData, "/Plug/hourtimeplug");
  String hourtimeplug = firebaseData.stringData(); // โชว์เวลา
  Firebase.getString(firebaseData, "/Plug/mintimeplug");
  String mintimeplug = firebaseData.stringData();// โชว์ชั่วโมง
    Firebase.getString(firebaseData, "/Plug/hourtimeplug2");
  String hourtimeplug2 = firebaseData.stringData();// โชว์นาที
  Firebase.getString(firebaseData, "/Plug/mintimeplug2");
  String mintimeplug2 = firebaseData.stringData(); // โชว์วินาที

  secondNow = timeClient.getSeconds();
  minuteNow = timeClient.getMinutes();
  hourNow = timeClient.getHours();

  lcd.setCursor(4, 0); //กำหนดชนิด lcd
  lcd.print(hourNow);  // แสดงชั่วโมง
  lcd.print(":"); 
  lcd.print(minuteNow);// แสดงนาที
  lcd.print(":");
  lcd.print(secondNow); //แสดงวินาที
  
  Serial.print(hourNow);
  Serial.print(":");
  Serial.print(minuteNow);
  Serial.print(":");
  Serial.println(secondNow);
  Serial.println(hourtimeplug);
  Serial.println(mintimeplug);
  if ((hourtimeplug.toInt() == hourNow)&&(mintimeplug.toInt() == minuteNow)){
    Firebase.setString(firebaseData, "/Plug/stateplug","true");
  }
  if ((hourtimeplug2.toInt() == hourNow)&&(mintimeplug2.toInt() == minuteNow)){
    Firebase.setString(firebaseData, "/Plug/stateplug2","true");
  }
  if (stateplug == "true") {
    digitalWrite(D5, LOW);
  } else {
    digitalWrite(D5, HIGH);
  }
  if (stateplug2 == "true") {
    digitalWrite(D6, LOW);
  } else {
    digitalWrite(D6, HIGH);
  }
}
// เมื่อให้เป็นค่าTureให้เป็นปิดค่าแต่ถ้านอกนั้นให้เป็นเปิด
void connectWifi() {
  Serial.begin(9600);
  Serial.println(WiFi.localIP());
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();  
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
}
//ไอ้ส่วนนี้เป็นการแสดงการเชื่อมต่อ Wi-Fi ในพอร์ตซีเรียลซ์