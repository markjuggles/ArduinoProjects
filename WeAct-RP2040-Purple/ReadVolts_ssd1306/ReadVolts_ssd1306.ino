
/*
 * ReadVolts_ssd1306
 * 
 * Read the voltage on A0, A1, and A2.  
 * Display the results on an SSD1306 128x32 display.
 * 
 * WeAct RP2040 Pico
 * Pin 6  GPIO4 SDA0  with 10k pull-up
 * Pin 7  GPIO5 SCL0  with-10k pull-up
 *
 * https://www.buydisplay.com/
 * ER-OLEDM0.91-1W-I2C
 *
 * Board Manager - https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
 * Library - Adafruit SSD1306 
 *
 */

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int Channel[4];

void setup()
{
  Serial.begin(115200);

  analogReadResolution(12);   // Set the analog resolution to 12 bits for values of 0- 4095.
  Channel[0] = A0;
  Channel[1] = A1;
  Channel[2] = A2;

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(1000);
}

#define TEXT_HEIGHT  8    // Text Height in pixels for TextSize 1.

void loop()
{
  float volts;
  int   adc;
  int   ch;
  char  string[64];

  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(10, 0*TEXT_HEIGHT);
  sprintf(string, "Chan  Volts", ch, adc);
  display.println(string);
    
  for(ch=0;ch<3;ch++)
  {
    analogRead(Channel[ch]);
    adc = 0;
    adc += analogRead(Channel[ch]);
    adc += analogRead(Channel[ch]);
    adc += analogRead(Channel[ch]);
    adc += analogRead(Channel[ch]);
    adc = adc / 4;

    // The full scale reading is 4096 which corresponds to 3.3 volts.
    volts = (float) adc * 3.3 / 4095.0;
    
    display.setCursor(10, (ch+1)*TEXT_HEIGHT);
    sprintf(string, "  %d  %1.4f", ch, volts);
    display.println(string);
  }

  display.display();
  delay(1000);
}
