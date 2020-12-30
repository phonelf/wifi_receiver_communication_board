#include <SoftwareSerial.h>
//35678
#include <ESP8266WiFi.h>
#include <Servo.h>

//設定wifi帳密(明碼)
#ifndef STASSID
//#define STASSID "iPhone"
//#define STAPSK "122334567vicki"
//#define STASSID "BenQ"
//#define STAPSK "00000000"
#define STASSID "ASUS"
#define STAPSK "79568d4ea14e"
#endif
#define FLCpin1 16
//d0
#define FLCpin2 5
//d1

#define PWMout0 4
//d2
#define PWMout1 14
//d5
#define PWMout2 12
//d6
#define PWMout3 15
//d8

//設定wifi帳密
const char *ssid = STASSID;
const char *password = STAPSK;

// Socket Server
//const char *host = "172.20.10.5";
const char *host = "192.168.43.1";
const uint16_t port = 9998;

String str1 = "a";
String str2 = "b";
String str3 = "c";
String str4 = "d";
int i = 0;
int pwmCount = 0;
char inport_c = '`';
uint8_t stat_alm = 0;
char stat001 = '`';
char buf[] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};

WiFiClient client;

Servo motor0;
Servo motor1;
Servo motor2;
Servo motor3;

void setup()
{
    Serial.begin(115200);
    pinMode(FLCpin1, OUTPUT); //flc
    pinMode(FLCpin2, OUTPUT); //flc
    digitalWrite(FLCpin1, HIGH);
    digitalWrite(FLCpin2, HIGH);
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
    //[B]
    digitalWrite(FLCpin1, LOW);
    digitalWrite(FLCpin2, LOW);

    motor0.attach(PWMout0);
    motor1.attach(PWMout1);
    motor2.attach(PWMout2);
    motor3.attach(PWMout3);
    client.println(millis());
}

void loop()
{
    if (client.available())
    {
        inport_c = client.read();
        Serial.print(inport_c);
        if (stat_alm == 1)
        {

            stat001 = inport_c;
            stat_alm = 2;
        }
        else if (stat_alm == 2)
        {

            if (stat001 == 'T' && inport_c == 'T')
            {
                digitalWrite(FLCpin1, HIGH);
                digitalWrite(FLCpin2, HIGH);
            }
            if (stat001 == 'S' && inport_c == 'B')
            {

                digitalWrite(FLCpin1, LOW);
                digitalWrite(FLCpin2, LOW);
            }

            if (stat001 == 'S' && inport_c == 'T')
            {

                digitalWrite(FLCpin1, LOW);
                digitalWrite(FLCpin2, HIGH);
            }

            if (stat001 == 'M' && inport_c == 'S')
            {

                digitalWrite(FLCpin1, HIGH);
                digitalWrite(FLCpin2, LOW);
            }
            stat_alm = 0;
        }
        if (inport_c >= 0 && inport_c <= 126)
        {
            pwmCount++;
            //Serial.println("!!");
        }
        buf[pwmCount] = inport_c;

        if (inport_c == '$')
        {

            stat_alm = 1;
            pwmCount = 0;
        }
        else
        {
        }

        if (inport_c == '~')
        {
            str1 = String(buf[9]) + String(buf[10]) + String(buf[11]) + String(buf[12]) + String(buf[13]);
            str2 = String(buf[14]) + String(buf[15]) + String(buf[16]) + String(buf[17]) + String(buf[18]);
            str3 = String(buf[19]) + String(buf[20]) + String(buf[21]) + String(buf[22]) + String(buf[23]);
            str4 = String(buf[24]) + String(buf[25]) + String(buf[26]) + String(buf[27]) + String(buf[28]);
            motor0.writeMicroseconds(str1.toInt());
            motor1.writeMicroseconds(str2.toInt());
            motor2.writeMicroseconds(str3.toInt());
            motor3.writeMicroseconds(str4.toInt());

            //Serial.println("!!");
            client.println(millis());
        }
        else
        {
        }
    }

    //client.println(str1 + "," + str2 + "," + str3 + "," + str4);
    //delay(100);
}
