#include <Wire.h>

// UV
#include "Adafruit_VEML6070.h" 
Adafruit_VEML6070 uv = Adafruit_VEML6070();
/*--------------------------------------------------------------------------------------------*/
//PM2.5
#include <PMS.h>
#include <SoftwareSerial.h> 
SoftwareSerial Serial1(2, 3); // 將Arduino Pin2設定為RX, Pin3設定為TX
PMS pms(Serial1);
PMS::DATA data;
/*--------------------------------------------------------------------------------------------*/
// LCD
#include <LiquidCrystal_I2C.h> 
LiquidCrystal_I2C lcd(0x27, 16, 2);
/*--------------------------------------------------------------------------------------------*/
// LED
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        6 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 16 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels
/*--------------------------------------------------------------------------------------------*/
// 設定
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  /*--------------------------------------------------------------------------------------------*/
  // UV setting
  Serial.println("VEML6070 Test");
  uv.begin(VEML6070_1_T);  // pass in the integration time constant
  
  /*--------------------------------------------------------------------------------------------*/
  // LED setting
  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  
  /*--------------------------------------------------------------------------------------------*/
  // LCD setting
  lcd.init();  //LCD初始化
  lcd.backlight();  //開啟背光
  lcd.clear();  //清除先前畫面
}

// 主程式
void loop() {
  pixels.clear(); // Set all pixel colors to 'off'
  pixels.setPixelColor(0, pixels.Color(105,105,105)); // PM2.5 灰色代表
  pixels.setPixelColor(7, pixels.Color(105,105,105)); // PM2.5 灰色代表
  pixels.setPixelColor(8, pixels.Color(128,0,128)); // UV 紫色代表
  pixels.setPixelColor(15, pixels.Color(128,0,128)); // UV 紫色代表
  
  /*--------------------------------------------------------------------------------------------*/
  // UV 紫外線
  Serial.print("UV light level: "); 
  Serial.println(uv.readUV());
  
  lcd.clear();  //清除先前畫面
  //設定游標位置在第0列第0行
  lcd.setCursor(0,0);
  lcd.print("UV Light Level");
  
  //設定游標位置在第1列第0行
  lcd.setCursor(0,1);
  
  // UV light level: 0~2 - 綠色
  if(uv.readUV() >= 0 and uv.readUV() <= 2)
  {
    lcd.print(uv.readUV());
    lcd.print("   (Low)");
    for(int i=14; i>8; i--) { // For each pixel...
      pixels.setPixelColor(i, pixels.Color(0, 150, 0));
      pixels.show();   // Send the updated pixel colors to the hardware.
      delay(DELAYVAL); // Pause before next pass through loop
    }
  }
  // UV light level: 3~5 - 橘色
  else if(uv.readUV() >= 3 and uv.readUV() <= 5)
  {
    lcd.print(uv.readUV());
    lcd.print("   (Midium)");
    for(int i=14; i>8; i--) { // For each pixel...
      pixels.setPixelColor(i, pixels.Color(255,165,0));
      pixels.show();   // Send the updated pixel colors to the hardware.
      delay(DELAYVAL); // Pause before next pass through loop
    }
  }
  // UV light level: 6~ - 紅色
  else if(uv.readUV() >= 6)
  {
    lcd.print(uv.readUV());
    lcd.print("   (High)");
    for(int i=14; i>8; i--) { // For each pixel...
      pixels.setPixelColor(i, pixels.Color(255, 0, 0));
      pixels.show();   // Send the updated pixel colors to the hardware.
      delay(DELAYVAL); // Pause before next pass through loop
    }
  }
  
  delay(1000);
  
  /*--------------------------------------------------------------------------------------------*/
  // PM2.5
  Serial.print("PM 2.5 (ug/m3): ");
  Serial.println(data.PM_AE_UG_2_5);
  lcd.clear();  //清除先前畫面
  //設定游標位置在第0列第0行
  lcd.setCursor(0,0);
  lcd.print("PM 2.5 (ug/m3): ");
  //設定游標位置在第1列第0行
  lcd.setCursor(0,1);

  // PM2.5 0~35 低度風險 - 綠色
  if(data.PM_AE_UG_2_5 >= 0 and data.PM_AE_UG_2_5 <=35)
  {
    lcd.print(data.PM_AE_UG_2_5);
    lcd.print("   (Low)");
    for(int i=1; i<7; i++) { // For each pixel...
      pixels.setPixelColor(i, pixels.Color(0, 150, 0));
      pixels.show();   // Send the updated pixel colors to the hardware.
      delay(DELAYVAL); // Pause before next pass through loop
    }
  }
  // PM2.5 36~53 中度風險 - 橘色
  else if(data.PM_AE_UG_2_5 >= 36 and data.PM_AE_UG_2_5 <=53)
  {
    lcd.print(data.PM_AE_UG_2_5);
    lcd.print("   (Midium)");
    for(int i=1; i<7; i++) {
      pixels.setPixelColor(i, pixels.Color(255,165,0));
      pixels.show();   // Send the updated pixel colors to the hardware.
      delay(DELAYVAL); // Pause before next pass through loop
    }
  }
  // PM2.5 54~ 高度風險 - 紅色
  else if(data.PM_AE_UG_2_5 >=54)
  {
    lcd.print(data.PM_AE_UG_2_5);
    lcd.print("   (High)");
    for(int i=1; i<7; i++) { // For each pixel...
      pixels.setPixelColor(i, pixels.Color(255,0,0));
      pixels.show();   // Send the updated pixel colors to the hardware.
      delay(DELAYVAL); // Pause before next pass through loop
    }
  }
 
  /*--------------------------------------------------------------------------------------------*/
  // LED
  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  /*
  for(int i=0; i<8; i++) { // For each pixel...
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor(i, pixels.Color(0, 150, 0));

    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(DELAYVAL); // Pause before next pass through loop
  }
  for(int i=NUMPIXELS; i>7; i--) { // For each pixel...
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor(i, pixels.Color(255, 0, 0));

    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(DELAYVAL); // Pause before next pass through loop
  }
  */
  delay(1000);
}
