/*
 * NORVI IIOT AE02-I  FINAL
 */
 
#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_ADS1X15.h>
#include <SoftwareSerial.h>

#define ANALOG_PIN_0 32

#define I2C1_SDA 16
#define I2C1_SCL 17

#define INPUT1   18
#define INPUT2   39
#define INPUT3   34
#define INPUT4   35
#define INPUT5   19
#define INPUT6   21
#define INPUT7   22
#define INPUT8   23

#define OUTPUT1 26
#define OUTPUT2 27
#define OUTPUT3 14
#define OUTPUT4 12
#define OUTPUT5 13
#define OUTPUT6 15
#define OUTPUT7 2
#define OUTPUT8 33

#define RS485_RXD 3
#define RS485_FC 4

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

unsigned long previousMillis = 0;  
const long interval = 15000;      


Adafruit_ADS1115 ads1;
Adafruit_ADS1115 ads2;

const float mA_Factor = 4.096 / 3269.826 ; 

SoftwareSerial *rs485Serial = nullptr; 

int analog_value = 0;
bool mode = true; 

int readSwitch() {
  analog_value = analogRead(ANALOG_PIN_0);
  return analog_value; //Read analog
}

void setup() {
  Serial.begin(9600);

  pinMode(RS485_FC, OUTPUT);
  digitalWrite(RS485_FC, HIGH);

  pinMode(OUTPUT1, OUTPUT);
  pinMode(OUTPUT2, OUTPUT);
  pinMode(OUTPUT3, OUTPUT);
  pinMode(OUTPUT4, OUTPUT);
  pinMode(OUTPUT5, OUTPUT);
  pinMode(OUTPUT6, OUTPUT);
  pinMode(OUTPUT7, OUTPUT);
  pinMode(OUTPUT8, OUTPUT);


  pinMode(INPUT1, INPUT);
  pinMode(INPUT2, INPUT);
  pinMode(INPUT3, INPUT);
  pinMode(INPUT4, INPUT);
  pinMode(INPUT5, INPUT);
  pinMode(INPUT6, INPUT);
  pinMode(INPUT7, INPUT);
  pinMode(INPUT8, INPUT);

  Wire.begin(I2C1_SDA, I2C1_SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.display();

  WiFi.mode(WIFI_MODE_APSTA);
  delay(300);
  wifi_test();
  delay(100);
  I2C_SCAN_1();

  if (!ads1.begin(0x48)) {
    Serial.println("Failed to initialize ADS 1.");
  }
  ads1.setGain(GAIN_ONE);

  if (!ads2.begin(0x49)) {
    Serial.println("Failed to initialize ADS 2.");
  }
  ads2.setGain(GAIN_ONE);

  
}

void loop() {
  if (Serial.available()) {
    char input = Serial.read();
    if (input == '2') {
      mode = true;  // Switch to first mode
      Serial2.begin(115200, SERIAL_8N1, RS485_RXD, -1); // Only RX is defined, TX is disabled
      if (rs485Serial) {
        delete rs485Serial; // Delete SoftwareSerial object if it exists
        rs485Serial = nullptr;
      }
    } else if (input == '1') {
      mode = false; // Switch to second mode
      rs485Serial = new SoftwareSerial(RS485_RXD, 1); // Define both RX and TX for the second code
      rs485Serial->begin(9600);
    }
  }

  if (mode) {
    firstCode();
  } else {
    secondCode();
  }
}

void firstCode() {
 

  Serial.println("");
  Serial.print(digitalRead(INPUT1)); Serial.print(digitalRead(INPUT2)); Serial.print(digitalRead(INPUT3)); Serial.println(digitalRead(INPUT4));
  Serial.print(digitalRead(INPUT5)); Serial.print(digitalRead(INPUT6)); Serial.print(digitalRead(INPUT7)); Serial.println(digitalRead(INPUT8));
  Serial.println("");
  
 unsigned long currentMillis = millis();  

  if (currentMillis - previousMillis >= interval) {
    
    previousMillis = currentMillis;  
    Serial.println("receiver mode test 'serial print '1'");  
  }
 
  
  Serial.print("Push button  "); Serial.println(readSwitch());
  Serial.println("");
  
   for (int i = 0; i < 4; i++) {
    float current1 = ads1.readADC_SingleEnded(i) * mA_Factor;
    Serial.print("Current "); Serial.print(i); Serial.print(": "); Serial.print(current1); Serial.println(" mA");
  }
  
  for (int i = 0; i < 2; i++) {
    float current2 = ads2.readADC_SingleEnded(i) * mA_Factor;
    Serial.print("Current "); Serial.print(i+4); Serial.print(": "); Serial.print(current2); Serial.println(" mA");
      
  }
  Serial.println("");
  delay(100); 
  

  digitalWrite(OUTPUT1, HIGH);
  digitalWrite(OUTPUT2, LOW);
  delay(200);
  digitalWrite(OUTPUT1, LOW);
  digitalWrite(OUTPUT2, HIGH);
  delay(200);
  digitalWrite(OUTPUT3, HIGH);
  delay(100);
  digitalWrite(OUTPUT4, HIGH);
  delay(100);
  digitalWrite(OUTPUT5, HIGH);
  delay(100);
  digitalWrite(OUTPUT6, HIGH);
  delay(100);
  digitalWrite(OUTPUT7, HIGH);
  delay(100);
  digitalWrite(OUTPUT8, HIGH);
  delay(100);
  digitalWrite(OUTPUT3, LOW);
  digitalWrite(OUTPUT4, LOW);
  digitalWrite(OUTPUT5, LOW);
  digitalWrite(OUTPUT6, LOW);
  digitalWrite(OUTPUT7, LOW);
  digitalWrite(OUTPUT8, LOW);
  delay(100);

  digitalWrite(RS485_FC, HIGH); // Make FLOW CONTROL pin HIGH
  delay(500);
  Serial2.println(F("RS485 01 SUCCESS")); // Send RS485 SUCCESS serially
  delay(500);
 
 Serial.print("----------------------");
}

void secondCode() {
  if (rs485Serial && rs485Serial->available()) {
    String rs485Data = rs485Serial->readString();
    Serial.println(rs485Data);

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("RS485 Data:");
    display.setCursor(0, 10);
    display.print(rs485Data);
    display.display();
  }

  if (Serial.available()) {
    String userInput = Serial.readString();
    digitalWrite(RS485_FC, HIGH);
    rs485Serial->print(userInput);
    digitalWrite(RS485_FC, LOW);
    delay(100);
  }
}

void I2C_SCAN_1() {
  byte error, address;
  int deviceCount = 0;

  Serial.println("Scanning...");

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.print(address, HEX);
      Serial.println("  !");
      deviceCount++;
      delay(1);
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }

  if (deviceCount == 0) {
    Serial.println("No I2C devices found in Wire 1\n");
  } else {
    Serial.println("Wire 1 Scanning complete\n");
  }

  delay(1000);
}


void wifi_test(){
    Serial.println("");
String str_macAddress;
byte mac[6];

WiFi.macAddress(mac);

   str_macAddress = (String(mac[0] >> 4,  HEX) + String(mac[0] & 0x0F, HEX)) + (":") +
                   (String(mac[1] >> 4,  HEX) + String(mac[1] & 0x0F, HEX)) + (":") +
                   (String(mac[2] >> 4,  HEX) + String(mac[2] & 0x0F, HEX)) + (":") +
                   (String(mac[3] >> 4,  HEX) + String(mac[3] & 0x0F, HEX)) + (":") +
                   (String(mac[4] >> 4,  HEX) + String(mac[4] & 0x0F, HEX)) + (":") +
                   (String(mac[5] >> 4,  HEX) + String(mac[5] & 0x0F, HEX));
  str_macAddress.toUpperCase();
  
  Serial.println("MAC Address: " + str_macAddress);

  String ssid =  str_macAddress; 
  const char* password = "12345678";   

  Serial.println("Setting up the Wi-Fi Access Point...");
  WiFi.softAP(ssid.c_str(), password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("Access Point IP address: ");
  Serial.println(IP);

  Serial.print("Access Point SSID: ");
  Serial.println(ssid);

  Serial.println("Wi-Fi Access Point is active!");
  Serial.println("");
}
