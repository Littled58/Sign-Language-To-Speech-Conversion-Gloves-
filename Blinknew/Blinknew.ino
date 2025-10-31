#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial mySoftwareSerial(2, 3); 
DFRobotDFPlayerMini myDFPlayer;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Flex pins
const int flexPin1 = A0;
const int flexPin2 = A1;
const int flexPin3 = A2;

// Adjust threshold based your readings
int threshold = 25;

bool bent1 = false, bent2 = false, bent3 = false;

void setup() {
  Serial.begin(115200);
  mySoftwareSerial.begin(9600);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Sign to Speech");
  delay(1000);
  lcd.clear();

  if (!myDFPlayer.begin(mySoftwareSerial)) {
    lcd.print("DFPlayer Error!");
    while (1);
  }

  lcd.print("DF Ready");
  delay(600);
  lcd.clear();

  myDFPlayer.volume(25);
}

void loop() {

  int flexValue1 = analogRead(flexPin1);
  int flexValue2 = analogRead(flexPin2);
  int flexValue3 = analogRead(flexPin3);

  // Debug to Serial
  Serial.print("A0="); Serial.print(flexValue1);
  Serial.print(" | A1="); Serial.print(flexValue2);
  Serial.print(" | A2="); Serial.println(flexValue3);

  // Show live values on LCD (first line only)
  lcd.setCursor(0, 0);
  lcd.print("0:"); lcd.print(flexValue1);
  lcd.print(" 1:"); lcd.print(flexValue2);
  lcd.print(" 2:"); lcd.print(flexValue3);

  // Sensor 1 (A0)
  if (flexValue1 > threshold && !bent1) {
    bent1 = true;
    playVoice(1, "A0 -> 001");
  }
  if (flexValue1 <= threshold) bent1 = false;

  // Sensor 2 (A1)
  if (flexValue2 > threshold && !bent2) {
    bent2 = true;
    playVoice(2, "A1 -> 002");
  }
  if (flexValue2 <= threshold) bent2 = false;

  // Sensor 3 (A2)
  if (flexValue3 > threshold && !bent3) {
    bent3 = true;
    playVoice(3, "A2 -> 003");
  }
  if (flexValue3 <= threshold) bent3 = false;

  delay(100);
}

void playVoice(int track, String msg) {
  lcd.setCursor(0,1);
  lcd.print(msg);
  myDFPlayer.play(track);
  delay(1000);         // Small delay only
  lcd.setCursor(0,1);
  lcd.print("                 "); // clear line
}
