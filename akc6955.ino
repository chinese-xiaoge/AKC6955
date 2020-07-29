#define WITH_IFRA 1
#define WITH_TOUCHSCREEN 1
#define WITH_KEY 0

#if !(WITH_IFRA || WITH_TOUCHSCREEN || WITH_KEY)
#error "Select the control MODE (WITH_IFRA || WITH_TOUCHSCREEN || WITH_KEY) !"
#endif

#if  WITH_TOUCHSCREEN && WITH_KEY
#error "UNO : Conflict with PINS!"
#endif

#if WITH_KEY

#define key_scan_prev 2
#define key_step_prev 3
#define key_step_next 4
#define key_scan_next 5

#define key_vol_redu 6
#define key_vol_plus 7
#define key_band_prev 8
#define key_band_next 9

#define key_power 10
#define key_reset 11
#define key_mute 12
#define key_boost 13

#define led_fm A0
#define led_am A1
#define led_tuned A2
#define led_stereo A3

#endif

#if WITH_TOUCHSCREEN
#include "lcd_touch.h"

TOUCHLCD LCD;

Button btn_scan_prev;
Button btn_step_prev;
Button btn_step_next;
Button btn_scan_next;

Button btn_vol_redu;
Button btn_vol_plus ;
Button btn_band_prev;
Button btn_band_next;

Button btn_power;
Button btn_reset;
Button btn_mute ;
Button btn_boost;
#endif

#include "akc6955.h"
AKC6955_RADIO radio;

#if WITH_IFRA
#include <IRremote.h>

IRrecv irrecv(10);
#endif

#define radio_setReg(a) i2c_write(AKC6955,  a, radio.reg##a.value);
// #define radio_getReg(a) radio.reg##a.value = i2c_read(AKC6955, a);

void setup(void)
{

#if WITH_IFRA
  irrecv.enableIRIn();
#endif

#if WITH_TOUCHSCREEN
  LCD.Initialize();
  LCD.centerText(2, 10,  "Radio Le & Miao");

  btn_scan_prev.Initialize (5, 152, 50, 40, "<<", SCAN_PREV, false);
  btn_step_prev.Initialize (58, 152, 50, 40, "<", STEP_PREV, false);

  btn_power.Initialize (118, 152, 40, 40, "P", POWER_ON, true);
  btn_reset.Initialize (162, 152, 40, 40, "R", RESET, false);

  btn_step_next.Initialize (212, 152, 50, 40, ">", STEP_NEXT, false);
  btn_scan_next.Initialize (265, 152, 50, 40, ">>", SCAN_NEXT, false);

  // *************************************************************//
  btn_band_prev.Initialize (5, 195, 50, 40, "<=", BAND_PREV, false);
  btn_vol_redu.Initialize (58, 195, 50, 40, "-", VOL_REDU, false);

  btn_boost.Initialize (118, 195, 40, 40, "B", BOOST, true);
  btn_mute.Initialize (162, 195, 40, 40, "M", MUTE, false);

  btn_vol_plus.Initialize (212, 195, 50, 40, "+", VOL_PLUS, false);
  btn_band_next.Initialize (265, 195, 50, 40, "=>", BAND_NEXT, false);
#endif

#if WITH_KEY
  pinMode(key_scan_prev, INPUT);
  pinMode(key_scan_next, INPUT);
  pinMode(key_step_prev, INPUT);
  pinMode(key_step_next, INPUT);
  pinMode(key_band_next, INPUT);
  pinMode(key_vol_redu, INPUT);
  pinMode(key_vol_plus, INPUT);


  pinMode(led_fm, OUTPUT);
  pinMode(led_am, OUTPUT);
  //pinMode(led_tuned, OUTPUT);
  pinMode(led_stereo, OUTPUT);

#endif

  radio.Initialize();

}

void loop()
{

#if WITH_IFRA
  decode_results ifr_results;
#endif

#if WITH_TOUCHSCREEN
  unsigned int px = -1; unsigned int py = -1;
  TSPoint p = ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
#endif

  if (0
#if WITH_IFRA
      || irrecv.decode(&ifr_results)
#endif
#if WITH_TOUCHSCREEN
      || (((p.z > MINPRESSURE) && ( p.z < MAXPRESSURE)))
#endif
#if WITH_KEY
      || 1
#endif
     )
  {
#if WITH_IFRA
    irrecv.resume();
#endif

#if WITH_TOUCHSCREEN
    px = -1; py = -1;

    if ((p.z > MINPRESSURE) && ( p.z < MAXPRESSURE))
    {
      py = map(p.x, TS_MINX, TS_MAXX, tft.height(), 0);
      px = map(p.y, TS_MINY, TS_MAXY, 0, tft.width());
    }
#endif

// ****************************************************************************************************************** //

    if (0
#if WITH_IFRA
        || (ifr_results.value ==  0xFFB04F)// || (btn_phase.Toggle(px, py))
#endif
       )
    {
      radio.reg6.phase_inv = !radio.reg6.phase_inv ;
      radio_setReg(6);
    }

// ****************************************************************************************************************** //

    if (0
#if WITH_IFRA
        || (ifr_results.value == 0xFF22DD)
#endif
#if WITH_TOUCHSCREEN
        || (btn_reset.Pressed(px, py))
#endif
#if WITH_KEY
        || (digitalRead(key_reset) == HIGH)
#endif
       )
    {
#if WITH_TOUCHSCREEN
      btn_reset.Press();
#endif
      radio.switchBand(0);
    }

// ****************************************************************************************************************** //

    if (0
#if WITH_IFRA
        || (ifr_results.value == 0xFFA25D)
#endif
#if WITH_TOUCHSCREEN
        || (btn_power.Pressed(px, py))
#endif
#if WITH_KEY
        || (digitalRead(key_power) == HIGH)
#endif
       )
    {
#if WITH_TOUCHSCREEN
      btn_power.Toggle();
#endif
      radio.reg0.power_on = !radio.reg0.power_on;
      radio_setReg(0);
    }

// ****************************************************************************************************************** //

    if (0
#if WITH_IFRA
        || (ifr_results.value ==  0xFFE21D)
#endif
#if WITH_TOUCHSCREEN
        || (btn_mute.Pressed(px, py))
#endif
#if WITH_KEY
        || (digitalRead(key_mute) == HIGH)
#endif
       )
    {
#if WITH_TOUCHSCREEN
      btn_mute.Toggle();
#endif
      radio.reg0.mute = !radio.reg0.mute;
      radio_setReg(0);
    }

// ****************************************************************************************************************** //

    if (0
#if WITH_IFRA
        || (ifr_results.value ==  0xFFE01F)
#endif
#if WITH_TOUCHSCREEN
        || (btn_boost.Pressed(px, py))
#endif
       )
    {
#if WITH_TOUCHSCREEN
      btn_boost.Toggle();
#endif
      radio.reg7.bass_boost = !radio.reg7.bass_boost;
      radio_setReg(7);
    }

// ****************************************************************************************************************** //

    if (0
#if WITH_IFRA
        ||  (ifr_results.value == 0xFFC23D)
#endif
#if WITH_TOUCHSCREEN
        || (btn_scan_next.Pressed(px , py) )
#endif
#if WITH_KEY
        || (digitalRead(key_scan_next) == HIGH)
#endif
       )
    {
#if WITH_TOUCHSCREEN
      btn_scan_next.Press();
#endif
      radio.Seek(1);
    }

// ****************************************************************************************************************** //

    if (0
#if WITH_IFRA
        ||  (ifr_results.value == 0xFF02FD)
#endif
#if WITH_TOUCHSCREEN
        || (btn_scan_prev.Pressed(px , py) )
#endif
#if WITH_KEY
        || (digitalRead(key_scan_prev) == HIGH)
#endif
       )
    {
#if WITH_TOUCHSCREEN
      btn_scan_prev.Press();
#endif
      radio.Seek(0);
    }

// ****************************************************************************************************************** //

    if (0
#if WITH_IFRA
        ||  (ifr_results.value == 0xFFA857)
#endif
#if WITH_TOUCHSCREEN
        || (btn_vol_redu.Pressed(px , py))
#endif
#if WITH_KEY
        || (digitalRead(key_vol_redu) == HIGH)
#endif
       )
    {
#if WITH_TOUCHSCREEN
      btn_vol_redu.Press();
#endif
      if (radio.reg6.sound_volume >= 24)
      {
        radio.setVolume(--radio.reg6.sound_volume);
      }
    }

// ****************************************************************************************************************** //

    if (0
#if WITH_IFRA
        ||  (ifr_results.value == 0xFF906F)
#endif
#if WITH_TOUCHSCREEN
        || (btn_vol_plus.Pressed(px , py))
#endif
#if WITH_KEY
        || (digitalRead(key_vol_plus) == HIGH)
#endif
       )
    {
#if WITH_TOUCHSCREEN
      btn_vol_plus.Press();
#endif
      if (radio.reg6.sound_volume < 63)
      {
        radio.setVolume(++radio.reg6.sound_volume);
      }
    }

// ****************************************************************************************************************** //

    if (0
#if WITH_IFRA
        ||  (ifr_results.value == 0xFF629D)
#endif
#if WITH_TOUCHSCREEN
        || (btn_band_next.Pressed(px , py))
#endif
#if WITH_KEY
        || (digitalRead(key_band_next) == HIGH)
#endif
       )
    {
#if WITH_TOUCHSCREEN
      btn_band_next.Press();
#endif
      radio.band_cursor = (++ radio.band_cursor)  % BAND_COUNT ;
      radio.switchBand(radio.band_cursor);
    }

// ****************************************************************************************************************** //

    if (0
#if WITH_IFRA
        ||  (ifr_results.value == 0xFF9867)
#endif
#if WITH_TOUCHSCREEN
        || (btn_band_prev.Pressed(px , py))
#endif
#if WITH_KEY
        || (digitalRead(key_band_prev) == HIGH)
#endif
       )
    {
#if WITH_TOUCHSCREEN
      btn_band_prev.Press();
#endif
      radio.band_cursor = (radio.band_cursor + BAND_COUNT - 1) % BAND_COUNT;
      radio.switchBand( radio.band_cursor );
    }

// ****************************************************************************************************************** //

    if (0
#if WITH_IFRA
        ||  (ifr_results.value == 0xFF6897)
#endif
#if WITH_TOUCHSCREEN
        || ( btn_step_prev.Pressed(px, py))
#endif
#if WITH_KEY
        || (digitalRead(key_step_next) == HIGH)
#endif
       )
    {
#if WITH_TOUCHSCREEN
      btn_step_prev.Press();
#endif
      radio.Step(-1);
    }

// ****************************************************************************************************************** //

    if (0
#if WITH_IFRA
        ||  (ifr_results.value == 0xFF30CF)
#endif
#if WITH_TOUCHSCREEN
        || ( btn_step_next.Pressed(px, py))
#endif
#if WITH_KEY
        || (digitalRead(key_step_next) == HIGH)
#endif
       )
    {
#if WITH_TOUCHSCREEN
      btn_step_next.Press();
#endif
      radio.Step(1);
    }
  }
  delay(10);
  
}
