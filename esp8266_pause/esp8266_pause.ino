#include <SoftwareSerial.h>
//35678
#include <ESP8266WiFi.h>

#include <bitset>

std::bitset<8> ToBits(unsigned char byte_)
{
    return std::bitset<8>(byte_);
}

//設定wifi帳密(明碼)
#ifndef STASSID
//#define STASSID "iPhone"
//#define STAPSK "122334567vicki"
#define STASSID "BenQ"
#define STAPSK "00000000"
#endif

//設定wifi帳密
const char *ssid = STASSID;
const char *password = STAPSK;

// Socket Server
//const char *host = "172.20.10.5";
const char *host = "192.168.43.1";
const uint16_t port = 9998;

WiFiClient client;

char inport_c = '`';

void setup()
{
    Serial.begin(115200);
    Serial.print("Start...");

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    delay(500);
    // Use WiFiClient class to create TCP connections

    if (!client.connect(host, port))
    {
        Serial.println("connection failed");
        Serial.println("wait 5 sec...");
        delay(5000);
        return;
    }

    //[c]

    pinMode(16, OUTPUT); //paluse
    digitalWrite(16, LOW);

    pinMode(5, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(0, OUTPUT);
    pinMode(2, OUTPUT);
    pinMode(14, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(13, OUTPUT);
    pinMode(15, OUTPUT);
}

void loop()
{
    if (client.available())
    {
        inport_c = client.read();
        Serial.print(inport_c);
        std::bitset<8> mtbest = ToBits(inport_c);
        digitalWrite(16, HIGH);
        digitalWrite(5, (mtbest[0]) ? HIGH : LOW);
        digitalWrite(4, (mtbest[1]) ? HIGH : LOW);
        digitalWrite(0, (mtbest[2]) ? HIGH : LOW);
        digitalWrite(2, (mtbest[3]) ? HIGH : LOW);
        digitalWrite(14, (mtbest[4]) ? HIGH : LOW);
        digitalWrite(12, (mtbest[5]) ? HIGH : LOW);
        digitalWrite(13, (mtbest[6]) ? HIGH : LOW);
        digitalWrite(15, (mtbest[7]) ? HIGH : LOW);
        digitalWrite(16, LOW);
    }

    client.println(String(millis()) + "," + String(inport_c));
}
