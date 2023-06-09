#include <ESP8266WiFi.h>
#include <MQ2.h>
#include <ThingSpeak.h>
 #include <Wire.h> 
  #include <LiquidCrystal_I2C.h>
#include <Servo.h>
  LiquidCrystal_I2C lcd(0x27, 16, 2);
MQ2 mq2(A0);
Servo s1;
WiFiClient client;

long myChannelNumber = 2181598;
const char myWriteAPIKey[] = "HS3GQD1RTXD53EKC";

void setup() {
  Serial.begin(9600);
  lcd.begin();//Defining 16 columns and 2 rows of lcd display
  lcd.backlight();
  WiFi.begin("Redmi Note 11T 5G", "99aAbB*#");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print("..");
    //lcd.print("Connecting to WIFI..");
  }
 // s1.attach(D4);
  Serial.println();
  //lcd.print("Connected");
  Serial.println("NodeMCU is connected!");
  Serial.println(WiFi.localIP());
  s1.attach(D4);
  mq2.begin();
  ThingSpeak.begin(client);
  s1.write(0);
  digitalWrite(D5, LOW);
  lcd.setCursor(0, 0);
  lcd.print("The IOT explorers");
  delay(5000);
}

void loop() {
  float* values = mq2.read(true); // Set it false if you don't want to print the values in the Serial
  float lpg = mq2.readLPG();
  float co = mq2.readCO();
  float smoke = mq2.readSmoke();
 

//delay(1000);
  Serial.println("LPG: " + String(lpg));
  Serial.println("CO: " + String(co));
  Serial.println("Smoke: " + String(smoke));
 // Serial.println("Methane: " + String(methane));
lcd.setCursor(0,0);
  lcd.print("LPG:");
  lcd.print(lpg);
  lcd.print(" CO:");
  lcd.print(co);
  lcd.setCursor(0,1);
  lcd.print("SMOKE:");
  lcd.print((smoke*100)/1000000);
  lcd.print(" ppm");
  if(smoke>=1||lpg>=1||co>=0)
{
      digitalWrite(D5, LOW);
      s1.write(90);
    }
  else
    {
      digitalWrite(D5, HIGH);
      s1.write(0);
    }
  ThingSpeak.writeField(myChannelNumber, 1, lpg, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 2, co, myWriteAPIKey);
  ThingSpeak.writeField(myChannelNumber, 3, smoke, myWriteAPIKey);
  //ThingSpeak.writeField(myChannelNumber, 4, methane, myWriteAPIKey);
  
  delay(500);
}
