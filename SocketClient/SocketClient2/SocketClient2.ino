#define BLYNK_PRINT Serial

/* Fill-in your Template ID (only if using Blynk.Cloud) */
//#define BLYNK_TEMPLATE_ID   "YourTemplateID"

#define BLYNK_TEMPLATE_ID "TMPL9qevRIm4"
#define BLYNK_DEVICE_NAME "smartparking"
#define BLYNK_AUTH_TOKEN "S2eupAtlQDDU1sEVxr1KWFVYrE04IsEc"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
const byte RX = D6;
const byte TX = D5;
SoftwareSerial mySerial = SoftwareSerial(RX, TX);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "S2eupAtlQDDU1sEVxr1KWFVYrE04IsEc";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "P202";
char pass[] = "tuhomedia";

int servo;
String UIDReceived = "";
String UID = "";
String statusFromSV = "";
bool stringComplete = false;
String result = "";

void setup()
{
  // Debug console
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
  read_UID();
}

BLYNK_WRITE(V1) {
  servo = param.asInt();
  if(servo == 1) {
         statusFromSV = "1111111111";
  }
  else if (servo == 0) {
    statusFromSV = "0000000000";
  }
  sendStatus(statusFromSV);
}

void read_UID() {
  while (mySerial.available()) {
    char inChar = (char)mySerial.read();
    UIDReceived += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
  if (stringComplete) {
    Serial.print("UID nhận từ Uno: ");
    Serial.println(UIDReceived);

    //Mở đóng gói UID
    unpackUID(UIDReceived);
    UIDReceived = "";
    stringComplete = false;
  }
}
void unpackUID(String UIDReceived) {
  //Tìm vị trí kí tự
  int findStarChar, findOpenBracketChar = -1;
  findStarChar = UIDReceived.indexOf("*");
  findOpenBracketChar = UIDReceived.indexOf("(");
  //Cắt chuỗi
  if (findStarChar >= 0 && findOpenBracketChar >= 0) {
    UID =  UIDReceived.substring(findStarChar + 1, findOpenBracketChar);
    Serial.print("UID: ");
    Serial.println(UID);
  }
}
void sendStatus(String statusFromSV) {
  String statusSend = "";
  //Đóng gói UID
  statusSend = "*" + statusFromSV + "(";
  Serial.print(" Send to Uno: ");
  Serial.println(statusSend);
  Serial.flush();
  mySerial.println(statusSend);
  mySerial.flush();
}
