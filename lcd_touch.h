#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <TouchScreen.h>

// wiring with UNO or Mega2560:
//--------------POWER Pins--------------------------------
//   5V  connects to DC 5V
//   GND connects to Ground
//   3V3 do not need to connect(NC)
//--------------LCD Display Pins--------------------------
//   LCD_RD   connects to Analog pin A0
//   LCD_WR   connects to Analog pin A1
//   LCD_RS   connects to Analog pin A2
//   LCD_CS   connects to Analog pin A3
//   LCD_RST  connects to Analog pin A4
//   LCD_D0   connects to digital pin 8
//   LCD_D1   connects to digital pin 9
//   LCD_D2   connects to digital pin 2
//   LCD_D3   connects to digital pin 3
//   LCD_D4   connects to digital pin 4
//   LCD_D5   connects to digital pin 5
//   LCD_D6   connects to digital pin 6
//   LCD_D7   connects to digital pin 7
//--------------SD-card fuction Pins ----------------------
//This Connection Only for UNO, Do not support Mega2560
//because they use different Hardware-SPI Pins
//SD_SS    connects to digital pin 10
//SD_DI    connects to digital pin 11
//SD_DO    connects to digital pin 12
//SD_SCK   connects to digital pin 13


#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin
#define LCD_CS A3   // Chip Select goes to Analog 3
#define LCD_CD A2  // Command/Data goes to Analog 2
#define LCD_WR A1  // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

//Param For 2.4"_9341_V3.0
#define TS_MINX 123
#define TS_MAXX 901

#define TS_MINY 82
#define TS_MAXY 904

#define MINPRESSURE 100
#define MAXPRESSURE 1000

#define FONT_SIZE_BASE_W 6
#define FONT_SIZE_BASE_H 8

#define CYAN              0x7FF
#define DARK_CYAN         0x002
#define BTN_DARK_CYAN     0x2AA
#define BTN_PRESS_CYAN    0x3BB

enum BUTTON
{
	SCAN_NEXT=1,
	SCAN_PREV,
	STEP_NEXT,
	STEP_PREV,
	VOL_PLUS,
	VOL_REDU,
	BAND_NEXT,
	BAND_PREV,
	POWER_ON,
	MUTE,
	BOOST,
	PHASE,
	RESET,
}BUTTON;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
MCUFRIEND_kbv tft;

class TOUCHLCD
{
  public:

#define W 320
#define H 240

    void Initialize()
    {
      tft.reset();
      tft.begin(tft.readID());
      tft.setRotation(1);
      tft.fillScreen(DARK_CYAN);
      tft.drawRect(0, 0, W, H, CYAN );
      tft.setTextColor(CYAN );
    }

    void Text(unsigned int font_size, unsigned int x, unsigned int y, char* text)
    {
      tft.setTextSize(font_size);
      tft.setCursor(x, y);
      tft.fillRect(x, y, FONT_SIZE_BASE_W * font_size * strlen(text), FONT_SIZE_BASE_H * font_size, DARK_CYAN );
      tft.print(text);
    }

    void centerText(unsigned int font_size, unsigned int y, char* text)
    {
      tft.fillRect(1, y, W - 2,   FONT_SIZE_BASE_H * font_size, DARK_CYAN );
      tft.setTextSize(font_size);
      tft.setCursor((W - FONT_SIZE_BASE_W * font_size * strlen(text)) / 2, y);
      tft.print(text);
    }
};

class Button
{
  public:
    unsigned int id = 0;
    unsigned int x = 0;
    unsigned int y = 0;
    unsigned int w = 0;
    unsigned int h = 0;
    char* label = "";
    bool toggle = false;

    Initialize(unsigned int _x, unsigned int _y, unsigned int _w, unsigned int _h, char* _label, unsigned int _id, bool _toggle)
    {
      id = _id; x = _x; y = _y; w = _w; h = _h; label = _label; toggle = _toggle;

      tft.drawRect(_x, _y, _w, _h,    CYAN );
      tft.fillRect(x + 1, y + 1, w - 2, h - 2, (toggle ?  BTN_PRESS_CYAN : BTN_DARK_CYAN));
      tft.setCursor(x + (w - 3 * FONT_SIZE_BASE_W * strlen(label)) / 2, y + (h - 3 * FONT_SIZE_BASE_H) / 2);
      tft.setTextSize(3);
      tft.print(label);
    }

    void Press()
    {
      Toggle();
      delay(100);
      Toggle();
    }

    void Toggle()
    {
      toggle = !toggle;
      tft.fillRect(x + 1, y + 1, w - 2, h - 2, toggle ? BTN_PRESS_CYAN : BTN_DARK_CYAN);
      tft.setCursor(x + (w - 3 * FONT_SIZE_BASE_W * strlen(label)) / 2, y + (h - 3 * FONT_SIZE_BASE_H) / 2);
      tft.setTextSize(3);
      tft.print(label);

    }

    int Pressed(unsigned int _x, unsigned int _y)
    {
      return (((_x > x) && (_x < (x + w))) && ((_y > y) && (_y < (y + h)))) ? id : 0;
    }
};
