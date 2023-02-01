  
//-----------------------------------Define Pin--------------------------------------------
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "max6675.h"

#define buttonOk 8
#define buttonUp 12
#define buttonDown 13
#define Alarm 27

int menu = 0; //
int thermoDO = 4;
int thermoDO2 = 15;
int thermoCS = 5;
int thermoCLK = 3; //
int dummyReset = 0;
int dummyBack = 0;

char lcdBuff[16];
char strTemp[6];
char lcdBuff2[16];
char strTemp2[6];

#define loadSub 70
#define pressed LOW
#define loadSys 100

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
MAX6675 thermocouple2(thermoCLK, thermoCS, thermoDO2);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  //  ---------------------------------Set Up Button---------------------------------------
  pinMode(buttonOk, INPUT_PULLUP);
  pinMode(buttonUp, INPUT_PULLUP);
  pinMode(buttonDown, INPUT_PULLUP);

  pinMode(Alarm, OUTPUT);
  //  ---------------------------------LCD Loading-----------------------------------------
  lcd.init();
  lcd.backlight();
  print(0, 0, "System Starting");
  print(0, 1, "Loading .");
  delay(loadSys);
  print(0, 1, "Loading ..");
  delay(loadSys);
  print(0, 1, "Loading ...");
  delay(700);
  lcd.clear();
  //  ---------------------------------Established Sensor----------------------------------
  print(0, 0, "Established OK!");
  delay(500);
  lcd.clear();
}
void loop() {
  if (digitalRead(buttonOk) == pressed) {
    menuMonitor();
  } else if (digitalRead(buttonDown) == pressed) {
    delay(900);
    muteAlarm();
    // dispRelay();
  } else if (digitalRead(buttonUp) == pressed) {
    delay(900);
    onAlarm();
  }
  tempRead();
  print(0, 0, "M/B Tack Convey.");
  print(0, 1, ">Start  Monitor<");
}
//  ---------------------------------Established Sensor----------------------------------
// Function For Read Value Sensor
void tempRead() {
  static const unsigned long rateInterval = 700;
  static unsigned long oldInterval = 0;
  float readC;
  float readCC;

  if (millis() - oldInterval >= rateInterval) {
    readC = thermocouple.readCelsius();
    readCC = thermocouple2.readCelsius();
    oldInterval += rateInterval;
  }

  else if (readC >= 30.00) {
    alertAlarm1();
  } else if (readCC >= 30.00) {
    alertAlarm2();
  } else if (readC >= 30.00 && readCC >= 30.00) {
    alertAlarmAll();
  }
}
//  ---------------------------------Established Sensor----------------------------------
// Function For LCD Print
void print(uint8_t col, uint8_t row, char* msg) {
  lcd.setCursor(col, row);
  lcd.print(msg);
}
//  ---------------------------------Menu Monitoring----------------------------------
// Funtion Display Menu Home
void menuMonitor() {
  lcd.clear();
  while (true) {
    dummyBack = 30;
    static const unsigned long rateInterval = 700;
    static unsigned long oldInterval = 0;
    float readC;
    float readCC;

    if (millis() - oldInterval >= rateInterval) {
      readC = thermocouple.readCelsius();
      readCC = thermocouple2.readCelsius();
      oldInterval += rateInterval;
    } else if (digitalRead(buttonDown) == pressed) {
      dummyBack = 0;
      break;
    } else if (digitalRead(buttonUp) == pressed) {
      menu++;
      delay(500);
      if (menu > 3) menu = 1;
    } else if (readC >= 30.00) {
      alertAlarm1();
    } else if (readCC >= 30.00) {
      alertAlarm2();
    } else if (readC >= 30.00 && readCC >= 30.00) {
      alertAlarmAll();
    }
    print(0, 0, " Select Monitor ");
    switch (menu) {
      case 1:
        dtostrf(readC, 5, 2, strTemp);
        sprintf(lcdBuff, "Motor 1: %s c", strTemp);
        print(0, 1, lcdBuff);
        break;
      case 2:
        dtostrf(readCC, 5, 2, strTemp2);
        sprintf(lcdBuff2, "Motor 2: %s c", strTemp2);
        print(0, 1, lcdBuff2);
        break;
      case 3: print(0, 1, "  Reset  Alarm  "); break;
    }
  }
  lcd.clear();
}
//  ---------------------------------Alarm Alert Display----------------------------------
// Function For Alert Alarm
void alertAlarm1() {
  lcd.clear();
  while (true) {
    if (dummyReset == 20 && digitalRead(Alarm) == HIGH) {
      lcd.clear();
      print(0, 0, "High Temperature");
      print(0, 1, "Motor 1  <Muted>");
      delay(2000);
      lcd.clear();
      break;
    } else if (digitalRead(buttonDown) == pressed && dummyReset == 0) {
      digitalWrite(Alarm, LOW);
      break;
    }
    digitalWrite(Alarm, HIGH);
    print(0, 0, "High Temperature");
    print(0, 1, "Motor 1  <Reset>");
  }
  lcd.clear();
}
void alertAlarm2() {
  lcd.clear();
  while (true) {
    if (dummyReset == 20 && digitalRead(Alarm) == HIGH) {
      lcd.clear();
      print(0, 0, "High Temperature");
      print(0, 1, "Motor 2  <Muted>");
      delay(2000);
      lcd.clear();
      break;
    } else if (digitalRead(buttonDown) == pressed && dummyReset == 0) {
      digitalWrite(Alarm, LOW);
      break;
    }
    digitalWrite(Alarm, HIGH);
    print(0, 0, "High Temperature");
    print(0, 1, "Motor 2  <Reset>");
  }
  lcd.clear();
}
void alertAlarmAll() {
  lcd.clear();
  while (true) {
    if (dummyReset == 20 && digitalRead(Alarm) == HIGH) {
      lcd.clear();
      print(0, 0, "High Temperature");
      print(0, 1, "All Mtr  <Muted>");
      delay(2000);
      lcd.clear();
      break;
    } else if (digitalRead(buttonDown) == pressed && dummyReset == 0) {
      digitalWrite(Alarm, LOW);
      break;
    }
    digitalWrite(Alarm, HIGH);
    print(0, 0, "High Temperature");
    print(0, 1, "All Mtr  <Reset>");
  }
  lcd.clear();
}
//  ---------------------------------Selected Items----------------------------------
// Function Select From Menu
void inSelect() {
  lcd.clear();
  while (true) {
    if (digitalRead(buttonDown) == pressed) {
      break;
    }
    print(0, 0, "Sub Menu Done");
  }
}
//  ---------------------------------Mute Alarm----------------------------------
// Function For Mute Alarm
void muteAlarm() {
  lcd.clear();
  while (true) {
    digitalWrite(Alarm, HIGH);
    print(0, 0, "Alarm Muted . . .");
    dummyReset = 20;
    delay(2000);
    break;
  }
  lcd.clear();
}
//  ---------------------------------Turn On Alarm----------------------------------
// Function For Turn On Alarm
void onAlarm() {
  lcd.clear();
  while (true) {
    digitalWrite(Alarm, LOW);
    print(0, 0, "Alarm On . . .");
    dummyReset = 0;
    delay(2000);
    break;
  }
  lcd.clear();
}
