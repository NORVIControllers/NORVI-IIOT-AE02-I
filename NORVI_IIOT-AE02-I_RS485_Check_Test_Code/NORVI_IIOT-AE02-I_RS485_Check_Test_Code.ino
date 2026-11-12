#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define FC 4
#define RXD 3
#define TXD 2

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);
  pinMode(FC, OUTPUT); 
 
  digitalWrite(FC, LOW); 
  
  Wire.begin(16, 17);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
}

void loop() { 
  Serial.println("RS485 01 SUCCESS");

  while (Serial.available()) {  // Check if data is available
    char c = Serial.read();     // Read data from RS485
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0, 12);
    display.print(c);
    delay(100);
    display.display();
  }
}
