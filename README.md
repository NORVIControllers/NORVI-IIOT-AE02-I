# NORVI IIOT-AE02-I – Hardware Test Example

## Overview

This example demonstrates how to run a full hardware test on the NORVI IIOT-AE02-I controller.

The purpose of this example is to show how to:

- Start the Wi-Fi access point and print the device MAC address
- Initialize the OLED display and scan the I2C bus
- Initialize the two ADS1115 analog-to-digital converters
- Read the status of the 8 digital inputs
- Read the six 4–20 mA analog inputs in mA
- Read the front-panel buttons
- Switch the 2 transistor outputs ON and OFF
- Send, receive and display data over RS-485

This example can be used as a starting point for developers building applications with NORVI controllers.

## Product Used

Product: NORVI IIOT-AE02-I (ESP32-WROOM-32, 8 digital inputs, 6 × 4–20 mA analog inputs, 2 transistor outputs, RS-485, OLED display)
Platform: Arduino IDE / PlatformIO (ESP32)

https://norvi.io

## Purpose of This Example

The example demonstrates how to:

- Start a Wi-Fi access point named after the device MAC address
- Initialize the SSD1306 OLED display over I2C (address 0x3C, SDA = GPIO 16, SCL = GPIO 17)
- Scan the I2C bus and print the address of every device found
- Initialize the two ADS1115 ADCs (I2C addresses 0x48 and 0x49) and read the 4–20 mA inputs
- Read the 8 digital inputs (GPIO 18, 39, 34, 35, 19, 21, 22, 23)
- Read the front-panel button analog value on GPIO 32
- Drive the transistor outputs (OUTPUT 1 = GPIO 26, OUTPUT 2 = GPIO 27)
- Use RS-485 (RX = GPIO 3, TX = GPIO 1, flow control = GPIO 4) to send text, and to receive text and show it on the OLED display

Naming used in the sketch and on the Serial Monitor:

- INPUT 1 to INPUT 8 are Digital Input 0 to Digital Input 7 in the datasheet
- OUTPUT 1 and OUTPUT 2 are Transistor Output 0 (T0) and Transistor Output 1 (T1) in the datasheet
- Current 0 to Current 5 are Analog Input 0 to Analog Input 5 (A0 to A5) in the datasheet

The program has two modes. It starts in the main test mode. Send `1` to switch to the RS-485 data mode and send `2` to return to the main test mode.

The program runs the following once at start-up, in this order:

- OLED display
- Wi-Fi access point
- I2C scan
- ADS1115 analog converters (0x48 and 0x49)

In the main test mode the program then continuously:

- Prints the state of INPUT 1 to INPUT 4 on one line and INPUT 5 to INPUT 8 on the next line (0 or 1)
- Prints the front-panel button analog value
- Prints the current of the six analog inputs in mA (Current 0 to Current 5)
- Switches OUTPUT 1 ON for 200 ms, then OUTPUT 1 OFF and OUTPUT 2 ON for 200 ms
- Switches OUTPUT 3 to OUTPUT 8 ON one after another at 100 ms intervals, then switches them OFF together
- Sets the RS-485 flow control pin HIGH and writes `RS485 01 SUCCESS` to Serial2
- Prints `receiver mode test 'serial print '1'` every 15 seconds

In the RS-485 data mode the program:

- Reads text received over RS-485 (9600 baud), prints it on the Serial Monitor and shows it on the OLED display under `RS485 Data:`
- Sends text typed in the Serial Monitor over RS-485

All results are shown on the Serial Monitor.

Follow the steps below to run the example:

### 1. Hardware Connections

- Power the controller with 24 V DC.
- Connect the USB cable to your computer.
- Connect digital input signals (18–32 V DC, sink or source) to INPUT 1 to INPUT 8.
- Connect 4–20 mA signals to the 6 analog inputs.
- Connect loads (or a multimeter) to the transistor outputs.
- For the RS-485 test, connect an RS-485 device or a USB-to-RS-485 converter to the RS-485 terminals.
- Ensure signals are within the supported input range.

### 2. Configure the Program

No additional user configuration is required for this example before upload.

### 3. Upload the Program

- Open the example program in Arduino IDE / PlatformIO
- Select the board **ESP32 Dev Module** and the correct port
- Upload the program to the NORVI controller

### 4. Observe the Serial Monitor

Open the Serial Monitor at **9600 baud**.

When the program runs successfully:

- The OLED display initializes
- The Wi-Fi access point starts and the MAC address, AP IP address and SSID are printed
- The I2C scan finds the OLED display (0x3C) and the two ADS1115 converters (0x48 and 0x49)
- The digital input states are printed continuously
- The front-panel button value is printed continuously
- The analog input currents are printed continuously
- The outputs switch ON and OFF in sequence

Example Output:

```

MAC Address: XX:XX:XX:XX:XX:XX
Setting up the Wi-Fi Access Point...
Access Point IP address: 192.168.4.1
Access Point SSID: XX:XX:XX:XX:XX:XX
Wi-Fi Access Point is active!

Scanning...
I2C device found at address 0x3C  !
I2C device found at address 0x48  !
I2C device found at address 0x49  !
Wire 1 Scanning complete


1111
1111

Push button  4095

Current 0: 0.00 mA
Current 1: 0.00 mA
Current 2: 0.00 mA
Current 3: 0.00 mA
Current 4: 0.00 mA
Current 5: 0.00 mA

----------------------
1111
1111
```

(Sample values only. The digits show INPUT 1–4 on the first line and INPUT 5–8 on the second line: 1 = OFF, 0 = ON.)

To test RS-485 reception, send `1` to switch to the RS-485 data mode. Text received over RS-485 is then printed on the Serial Monitor and shown on the OLED display. Send `2` to return to the main test mode.

### How to Validate the Example

To verify the example works correctly:

1. Confirm the OLED display turns on and no `SSD1306 allocation failed` message appears
2. Confirm the Wi-Fi access point appears on your phone or computer (SSID = device MAC address, password `12345678`)
3. Confirm the I2C scan lists 0x3C, 0x48 and 0x49, and that neither `Failed to initialize ADS 1.` nor `Failed to initialize ADS 2.` appears
4. Apply a valid signal to a digital input and observe the printed value change from 1 to 0
5. Apply a 4–20 mA signal to an analog input and observe the printed current change
6. Press each front-panel button and confirm the button value changes
7. Observe OUTPUT 1 and OUTPUT 2 switching (or check with a multimeter)
8. Send `1`, send text from an RS-485 device and confirm it appears on the Serial Monitor and on the OLED display
9. Send `2` and confirm the main test output returns

### Device Preparation / Configuration

Before running the example ensure:

- The controller is powered correctly
- Required signals are connected
- Any required configuration is completed

## Required Libraries

Install the following libraries before compiling.

- Wire
- WiFi
- Adafruit_GFX (installed with Adafruit_SSD1306)
- Adafruit_SSD1306
- Adafruit_ADS1X15
- SoftwareSerial (EspSoftwareSerial library for ESP32)

Installation:

1. Open Arduino IDE
2. Go to Library Manager
3. Search and install the required libraries and any dependencies you are prompted for (Wire and WiFi are included with the ESP32 core)

## Limitations

- This example is provided for demonstration purposes
- The OLED shows only the library's default splash screen in the main test mode; text is drawn on it only in the RS-485 data mode, when data is received
- If the OLED is not detected, the program stops at start-up; if an ADS1115 is not detected, a message is printed and the program continues
- The mode characters `1` and `2` are read once per loop, so in the main test mode (about 2.2 seconds per loop plus the ADC read time) a mode change can take a moment; any other character is ignored
- Characters typed in the Serial Monitor are also read by the mode selection at the start of each loop, so text typed for sending over RS-485 may arrive incomplete
- Serial2 is started only when `2` is received, with RX only (GPIO 3, 115200 baud) and the TX pin disabled; until then the `RS485 01 SUCCESS` text written to Serial2 in the main test mode is not sent
- The RS-485 pins (GPIO 1 and GPIO 3) are shared with the USB serial port
- The sketch also drives GPIO 14, 12, 13, 15, 2 and 33 (OUTPUT 3 to OUTPUT 8); the datasheet assigns no output terminal to these pins on this model
- OUTPUT 2 stays ON from the second step of the output sequence until the start of the next sequence
- The analog input currents are calculated with a fixed conversion factor (4.096 / 3269.826 per ADC count)

## Safety Notes

- Do not exceed the rated input voltage (digital inputs 32 V DC maximum, analog inputs 38 V DC maximum)
- Do not exceed the transistor output rating (100 mA, 40 V DC maximum)
- Ensure proper grounding
- Disconnect power before wiring
- Incorrect wiring may damage the controller

Test Date: 11/11/2026 
Verified By: Kaveesha

## Support

Documentation:
https://norvi.io/docs

User guide: https://norvi.io/docs/norvi-iiot-ae02-i-user-guide/

For additional support or inquiries, contact the NORVI support team (support@norvi.io).

Datasheet : https://norvi.io/docs/norvi-iiot-ae02-i-datasheet/

## License

This example is provided for development and educational purposes.
