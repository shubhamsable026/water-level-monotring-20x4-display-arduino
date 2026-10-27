
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <string.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

const byte LOW_PIN = 2;
const byte MEDIUM_PIN = 3;
const byte HIGH_PIN = 4;

unsigned long lastAnimation = 0;
bool blinkState = true;

void printCentered(byte row, const char* text) {
  lcd.setCursor(0, row);
  lcd.print("                    ");

  int len = strlen(text);
  int col = (20 - len) / 2;

  lcd.setCursor(col, row);
  lcd.print(text);
}

void drawBattery(byte level) {
  lcd.setCursor(0, 2);

  for (byte i = 0; i < 20; i++) {
    if (i < level * 2) {
      lcd.write((byte)255);
    } else {
      lcd.print(' ');
    }
  }
}

void setup() {
  pinMode(LOW_PIN, INPUT);
  pinMode(MEDIUM_PIN, INPUT);
  pinMode(HIGH_PIN, INPUT);

  lcd.init();
  lcd.backlight();
  lcd.clear();

  printCentered(0, "WATER LEVEL");
  printCentered(1, "SYSTEM STARTING");
  delay(1000);
  lcd.clear();
}

void loop() {
  // BC547 transistor ON = Arduino input LOW
  bool lowWet = digitalRead(LOW_PIN) == LOW;
  bool mediumWet = digitalRead(MEDIUM_PIN) == LOW;
  bool highWet = digitalRead(HIGH_PIN) == LOW;

  const char* status;
  const char* message;
  byte level;

  if (highWet) {
    status = "FULL";
    message = "TANK FULL";
    level = 10;
  }
  else if (mediumWet) {
    status = "MEDIUM";
    message = "HALF TANK";
    level = 5;
  }
  else if (lowWet) {
    status = "LOW";
    message = "WATER LOW!";
    level = 3;
  }
  else {
    status = "EMPTY";
    message = "NO WATER";
    level = 0;
  }

  // Blink only EMPTY and LOW
  if (millis() - lastAnimation >= 500) {
    lastAnimation = millis();
    blinkState = !blinkState;
  }

  printCentered(0, "WATER LEVEL");

  if (strcmp(status, "EMPTY") == 0) {
    printCentered(1, blinkState ? "!! EMPTY !!" : "");
  }
  else if (strcmp(status, "LOW") == 0) {
    printCentered(1, blinkState ? "! LOW !" : "");
  }
  else {
    printCentered(1, status);
  }

  // Full-width battery bar
  drawBattery(level);

  printCentered(3, message);

  delay(100);
}
