#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ===== STUDENTS =====
struct Student {
  String uid;
  String name;
  bool isInside;
  int count;
};

Student students[] = {
  {"1 C4 8 5", "Berry", false, 0},
};

int totalStudents = sizeof(students) / sizeof(students[0]);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  lcd.init();
  lcd.backlight();

  lcd.print("Scan Card...");
}

void loop() {

  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  String uid = "";

  for (byte i = 0; i < rfid.uid.size; i++) {
    uid += String(rfid.uid.uidByte[i], HEX);
    uid += " ";
  }

  uid.toUpperCase();
  uid.trim();

  for (int i = 0; i < totalStudents; i++) {

    if (uid == students[i].uid) {

      students[i].count++;

      lcd.clear();
      lcd.print(students[i].name);

      if (!students[i].isInside) {
        students[i].isInside = true;

        lcd.setCursor(0, 1);
        lcd.print("Check-IN");

        Serial.println(students[i].name + ",IN," + String(students[i].count));

      } else {
        students[i].isInside = false;

        lcd.setCursor(0, 1);
        lcd.print("Check-OUT");

        Serial.println(students[i].name + ",OUT," + String(students[i].count));
      }

      delay(2000);
      lcd.clear();
      lcd.print("Scan Card...");
      return;
    }
  }

  // Unknown card
  lcd.clear();
  lcd.print("Access Denied");
  Serial.println("UNKNOWN,NA,0");

  delay(2000);
  lcd.clear();
  lcd.print("Scan Card...");
}