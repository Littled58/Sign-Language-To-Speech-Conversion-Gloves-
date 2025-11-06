#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial mySoftwareSerial(2, 3);
DFRobotDFPlayerMini myDFPlayer;
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int flexPin1 = A0;
const int flexPin2 = A1;
const int flexPin3 = A2;

// Increase threshold based on real values (adjust after Serial Monitor test)
int threshold = 200;     // Not 25 anymore → realistic
int hysteresis = 30;     // Stops flickering (threshold + hysteresis)

bool bent1 = false, bent2 = false, bent3 = false;
unsigned long lastPlayTime = 0;
int minDelay = 800;      // Prevents repeated speech within 0.8 sec

void setup() {
  Serial.begin(115200);
  mySoftwareSerial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.print("Sign to Speech");
  delay(1000);
  lcd.clear();

  if (!myDFPlayer.begin(mySoftwareSerial)) {
    lcd.print("DFPlayer Error!");
    while (1);
  }
  myDFPlayer.volume(30);
  lcd.print("DF Ready");
  delay(600);
  lcd.clear();
}

int smoothSensor(int pin) {
  long sum = 0;
  for (int i = 0; i < 10; i++) {
    sum += analogRead(pin);
    delay(5);
  }
  return sum / 10;
}

void loop() {
  int flexValue1 = smoothSensor(flexPin1);
  int flexValue2 = smoothSensor(flexPin2);
  int flexValue3 = smoothSensor(flexPin3);

  Serial.print("A0="); Serial.print(flexValue1);
  Serial.print(" | A1="); Serial.print(flexValue2);
  Serial.print(" | A2="); Serial.println(flexValue3);

  lcd.setCursor(0, 0);
  lcd.print("0:"); lcd.print(flexValue1);
  lcd.print(" 1:"); lcd.print(flexValue2);
  lcd.print(" 2:"); lcd.print(flexValue3);

  unsigned long currentTime = millis();

  if (flexValue1 > threshold && !bent1 && currentTime - lastPlayTime > minDelay) {
    bent1 = true;
    playVoice(1, "A0 -> 001");
  }
  if (flexValue1 < threshold - hysteresis) bent1 = false;

  if (flexValue2 > threshold && !bent2 && currentTime - lastPlayTime > minDelay) {
    bent2 = true;
    playVoice(2, "A1 -> 002");
  }
  if (flexValue2 < threshold - hysteresis) bent2 = false;

  if (flexValue3 > threshold && !bent3 && currentTime - lastPlayTime > minDelay) {
    bent3 = true;
    playVoice(3, "A2 -> 003");
  }
  if (flexValue3 < threshold - hysteresis) bent3 = false;

  delay(80);
}

void playVoice(int track, String msg) {
  lastPlayTime = millis();
  lcd.setCursor(0, 1);
  lcd.print(msg);
  myDFPlayer.play(track);
  delay(500);
  lcd.setCursor(0, 1);
  lcd.print("                ");
}
