#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
#define LED 13
const int OUT_PIN = A2;
const int IN_PIN = A0;
const float IN_STRAY_CAP_TO_GND = 24.48;
const float IN_CAP_TO_GND  = IN_STRAY_CAP_TO_GND;
const float R_PULLUP = 30.8;  // the resistance in the port
const int MAX_ADC_VALUE = 1023;
const float sensitivity = 5.8573; // richtingscoefficient
const float startvalue = 113.119; // stationaryvalue
int counter = 0;

void setup() {
  pinMode(OUT_PIN, OUTPUT);
  pinMode(IN_PIN, OUTPUT);
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  lcd.begin(16, 2);

}

void loop() {
  pinMode(IN_PIN, INPUT);
  digitalWrite(OUT_PIN, HIGH);
  int val = analogRead(IN_PIN);
  digitalWrite(OUT_PIN, LOW);
  digitalWrite(LED, LOW);
  
  
  if (val < 1000) {
    pinMode(IN_PIN, OUTPUT);

    float capacitance = (float)val * IN_CAP_TO_GND / (float)(MAX_ADC_VALUE - val);
    float acceleration = (capacitance - startvalue) / sensitivity;

    if (capacitance > 30.0) {
      digitalWrite(LED, HIGH);
    }
    Serial.print(F("Capacitance Value;"));
    Serial.print(capacitance, 3); 
    Serial.println(); 
    Serial.print(F(" pF ("));
    Serial.print(val);
    Serial.println(F(") "));

    Serial.print(F("acceleration;"));
    Serial.print(acceleration, 2);
    Serial.println();
    if ((counter % 7) == 0) {
      lcd.print(capacitance,3);
      lcd.print("pF");
      lcd.setCursor(0, 0);
    }
    

  }


  else {
    pinMode(IN_PIN, OUTPUT);
    delay(1);
    pinMode(OUT_PIN, INPUT_PULLUP);
    unsigned long u1 = micros();
    unsigned long t;
    int digVal;

    do {
      digVal = digitalRead(OUT_PIN);
      unsigned long u2 = micros();
      t = u2 > u1 ? u2 - u1 : u1 - u2;
    } while ((digVal < 1) && (t < 400000L));

    pinMode(OUT_PIN, INPUT);
    val = analogRead(OUT_PIN);
    digitalWrite(IN_PIN, HIGH);
    int dischargeTime = (int)(t / 1000L) * 5;
    delay(dischargeTime);
    pinMode(OUT_PIN, OUTPUT);
    digitalWrite(OUT_PIN, LOW);
    digitalWrite(IN_PIN, LOW);

    float capacitance = -(float)t / R_PULLUP / log(1.0 - (float)val / (float)MAX_ADC_VALUE);
    

    Serial.print(F("Capacitance Value = "));

    
    if (capacitance > 1000.0) {

      Serial.print(capacitance / 1000.0, 2);
      Serial.print(F(" uF"));
      lcd.print(capacitance,3);
     
      lcd.print("uF");
      lcd.setCursor(0, 0);

    }
    else {
      Serial.print(capacitance, 2);
      Serial.print(F(" nF"));
      lcd.print(capacitance,3);
      lcd.print("nF");
      lcd.setCursor(0, 0);
    }

    Serial.print(F(" ("));
    Serial.print(digVal == 1 ? F("Normal") : F("HighVal"));
    Serial.print(F(", t= "));
    Serial.print(t);
    Serial.print(F(" us, ADC= "));
    Serial.print(val);
    Serial.println(F(")"));
    lcd.print(capacitance,3);
    lcd.print("nF");
    lcd.setCursor(0, 0);

  }
  counter += 1;
  while (millis() % 100 != 0);

}
