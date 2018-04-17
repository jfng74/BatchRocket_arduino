#include <Arduino.h>
#include <Wire.h>
#include <i2cdetect.h>
#include <SPI.h>
#include "Adafruit_MAX31855.h"
#include <mcp2515.h>



#include <LiquidCrystal_PCF8574.h>
#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif
uint8_t bell[8]  = {0x4,0xe,0xe,0xe,0x1f,0x0,0x4};
uint8_t note[8]  = {0x2,0x3,0x2,0xe,0x1e,0xc,0x0};
uint8_t clock[8] = {0x0,0xe,0x15,0x17,0x11,0xe,0x0};
uint8_t heart[8] = {0x0,0xa,0x1f,0x1f,0xe,0x4,0x0};
uint8_t duck[8]  = {0x0,0xc,0x1d,0xf,0xf,0x6,0x0};
uint8_t check[8] = {0x0,0x1,0x3,0x16,0x1c,0x8,0x0};
uint8_t cross[8] = {0x0,0x1b,0xe,0x4,0xe,0x1b,0x0};
uint8_t retarrow[8] = {	0x1,0x1,0x5,0x9,0x1f,0x8,0x4};

LiquidCrystal_PCF8574 lcd(0x3F);  // set the LCD address to 0x27 for a 16 chars and 2 line display


// BME280 0x76
//
//
//
// Example creating a thermocouple instance with software SPI on any three
// digital IO pins.
#define MAXDO   12
#define TC1_MAXCS   9
#define TC2_MAXCS   8
#define MAXCLK  13

// Initialize the Thermocouple

Adafruit_MAX31855 tc1(MAXCLK, TC1_MAXCS, MAXDO);
Adafruit_MAX31855 tc2(MAXCLK, TC2_MAXCS, MAXDO);
int show;
struct can_frame canMsg1;
struct can_frame canMsg2;
//MCP2515 mcp2515(10);


void setup() {
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
/*  pinMode(TC1_MAXCS, OUTPUT);
  digitalWrite(TC1_MAXCS, HIGH);
  pinMode(TC2_MAXCS, OUTPUT);
  digitalWrite(TC2_MAXCS, HIGH);
*/

  canMsg1.can_id  = 0x0F6;
  canMsg1.can_dlc = 8;
  canMsg1.data[0] = 0x8E;
  canMsg1.data[1] = 0x87;
  canMsg1.data[2] = 0x32;
  canMsg1.data[3] = 0xFA;
  canMsg1.data[4] = 0x26;
  canMsg1.data[5] = 0x8E;
  canMsg1.data[6] = 0xBE;
  canMsg1.data[7] = 0x86;

  canMsg2.can_id  = 0x036;
  canMsg2.can_dlc = 8;
  canMsg2.data[0] = 0x0E;
  canMsg2.data[1] = 0x00;
  canMsg2.data[2] = 0x00;
  canMsg2.data[3] = 0x08;
  canMsg2.data[4] = 0x01;
  canMsg2.data[5] = 0x00;
  canMsg2.data[6] = 0x00;
  canMsg2.data[7] = 0xA0;
/*
  SPI.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS,MCP_8MHZ);
  mcp2515.setNormalMode();
  SPI.end();
*/
  int error;

    // put your setup code here, to run once:
    //

  Serial.begin(9600);

  Wire.begin();
  Wire.beginTransmission(0x3F);
  error = Wire.endTransmission();
  Serial.print("Error: ");
  Serial.print(error);

  if (error == 0) {
    Serial.println(": LCD found.");

  } else {
    Serial.println(": LCD not found.");
  } // if

  lcd.begin(20, 4); // initialize the lcd
  show = 0;
//  tc1.begin();
//  tc2.begin();
  lcd.setBacklight(125);
  lcd.home(); lcd.clear();
  lcd.print("BatchRocket 1.0");
  delay(2000);
  digitalWrite(2, LOW);
}

void loop() {

/*  int status_tc1 = tc1.read();
  Serial.print("TC1 stat:\t");
  Serial.println(status_tc1);
*/

  double internal_tc1 = tc1.readInternal();
  double temp_tc1 = tc1.readCelsius();
  double internal_tc2 = tc2.readInternal();
  double temp_tc2 = tc2.readCelsius();
  uint8_t error = tc2.readError();

  Serial.print("TC1 : internal:\t");
  Serial.println(internal_tc1);
  Serial.print("TC1 : temperature:\t");
  Serial.println(temp_tc1);
  Serial.print("TC2 : internal:\t");
  Serial.println(internal_tc2);
  Serial.print("TC2 : temperature:\t");
  Serial.println(temp_tc2);
  Serial.print("TC2 : Error : ");
  Serial.println(error);

  lcd.home(); lcd.clear();
  lcd.print("tc1:");
  lcd.print(internal_tc1);
  lcd.print(":");
  lcd.print(temp_tc1);

  lcd.setCursor(0, 1);


  lcd.print("tc2:");
  lcd.print(internal_tc2);
  lcd.print(":");
  lcd.print(temp_tc2);
/*
  SPI.begin();
  mcp2515.sendMessage(&canMsg1);
  mcp2515.sendMessage(&canMsg2);
  SPI.end();
*/

  delay(1000);
}
