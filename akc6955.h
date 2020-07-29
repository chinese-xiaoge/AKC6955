#include <Wire.h>
#define AKC6955 0x10

enum TUNER_MODE
{
  AM = 0,
  FM,
} TUNER_MODE;

enum BAND_VALUE
{
  FM1 = 0b000,
  MW2 = 0b00010,

  SW1 = 0b00101,
  SW2 = 0b00110,
  SW3 = 0b00111,
  SW4 = 0b01000,
  SW5 = 0b01001,
  SW6 = 0b01010,
  SW7 = 0b01011,
  SW8 = 0b01100,
  SW9 = 0b01101,
  SW10 = 0b01110,
  SW11 = 0b01111,

  LW = 0b00000,
  TV1 = 0b101,
  TV2 = 0b110,
  //MW1 = 0b00001
};

enum BAND
{
  _FM1 = 0,
  _MW2,

  _SW1,
  _SW2,
  _SW3,
  _SW4,
  _SW5,
  _SW6,
  _SW7,
  _SW8,
  _SW9,
  _SW10,
  _SW11,

  _LW,
  _TV1,
  _TV2,
  //_MW1,

}BAND;

union reg0
{
  unsigned char value;
  struct
  {
    unsigned char reserved:            2;
    unsigned char mute:             1;
    unsigned char seek_direction:       1; // 0 下 1 上
    unsigned char go_seek:            1; // 0->1 开始搜索 STC=1 停止搜索
    unsigned char tune:             1; // 0->1 到指定的频率
    unsigned char fm_mode:            1;
    unsigned char power_on:           1;
  };
};

union reg1
{
  unsigned char value;
  struct
  {
    unsigned char fm_band:            3; // 可以设为000 FM 187~108, 搜台间隔有space定
    unsigned char am_band:            5; // 可以设为00010 MW2，0.522~1.62, 9K搜台
  };
} ;

union reg2
{
  unsigned char value;
  struct
  {
    unsigned char chanel_freq_high:     5; // 信道号的高5位 FM mode: Channel Freq.=25kHz*CHAN + 30MHz   AM mode, 5K信道号模式时:Channel Freq.= (mode3k?3:5) kHz*CHAN
    unsigned char mode3k:             1; // MCU, MW2 工作时，送入的信道号一定要保证是 3 的倍数。否则电台会乱掉
    unsigned char ref_32kmode:        1; // 1—参考时钟为32.768K / 0 1—参考时钟为12M
    unsigned char reserved:           1;
  };
};

union reg3
{
  unsigned char value;
  struct
  {
    unsigned char chanel_freq_low:      8;  // 信道号的低 8 位
  };
};

union reg4
{
  unsigned char value;
  struct
  {
    unsigned char user_chanel_start:    8; // 自定义信道号起始 //chan=32*usr_chan_start
  };
};

union reg5
{
  unsigned char value;
  struct
  {
    unsigned char user_chanel_end:      8; // 自定义信道号终止 chan=32*usr_chan_stop
  };
};

union reg6
{
  unsigned char value;
  struct
  {
    unsigned char phase_inv:          1; // 0 同相 / 1反相
    unsigned char line_in:            1; // 0 OFF / 1 ON
    unsigned char sound_volume:       6; // <24:mute，24~63共40级音量控制，每级1.5dB pd_adc_vol=1 时用该寄存器音量
  };
};


union reg7
{
  unsigned char value;
  struct
  {
    unsigned char fm_band_width:        2; // 00 150K 01 200K 10 50K 11 100K
    unsigned char stereo:             2; // “00”自动立体声,门限有Stereo_th控制 “10”只要有导频就强制立体声 “x1”强制单声道解调
    unsigned char bass_boost:         1;
    unsigned char de:               1; // 去加重模式选择 0— 75 μ s (USA) 1— 50 μ s (China / Europe)
    unsigned char reserved0:            1;
    unsigned char reserved1:            1;
  };
};


union reg8
{
  unsigned char value;
  struct
  {
    unsigned char stereo_threshold:       2; // FM开始立体声解调的CNR门限值
    unsigned char fd_threshold:         2; // 芯片内判台和点灯用的频偏门限
    unsigned char am_cnr_threshold:       2; // AM模式时，芯片判台和点灯的载噪比门限
    unsigned char fm_cnr_threshold:       2; // FM模式时，芯片判台和点灯的载噪比门限
  };
};

union reg9
{
  unsigned char value;
  struct
  {
    unsigned char lv_en:            1; //在低压供电时，是否进入低压工作模式
    unsigned char reserved0:          1;
    unsigned char oscillator_type:      1; //Oscillator source selection
    unsigned char i2c_volume:         1;
    unsigned char reserved1:          4;
  };
};

union reg11
{
  unsigned char value;
  struct
  {
    unsigned char reserved0:          4;
    unsigned char fm_seek_skip:       2; // FM 搜台步进 00—25kHz 01—50kHz 10—100kHz 11—200kHz, 针对 TV1 和 TV2 该档为 100K
    unsigned char reserved1:          2;
  };
};

union reg12
{
  unsigned char value;
  struct
  {
    unsigned char reserved0:          5;
    unsigned char pd_rx:              1; // 1--模拟与射频通道关闭
    unsigned char reserved1:          1;
    unsigned char pd_adc:           1; // 0-- 信号通道 ADC 打开
  };
};

union reg13
{
  unsigned char value;
  struct
  {
    unsigned char reserved0:          2;
    unsigned char predefined_volume:    2; // 输出音量调整整值：00： 0dB 01： 3.5dB 10： 7dB 11： 10.5dB
    unsigned char reserved1:          2;
    unsigned char st_led:           1; // 0—tund管脚为调谐灯 1—FM 且非wtmode时，tund管脚为立体声解调指示灯，其余为调谐灯
    unsigned char reserved2:          1;
  };
};

// *************************************** 以下为只读 ********************************************************//
union reg20
{
  unsigned char value;
  struct
  {
    unsigned char read_chanel_high:     5;
    unsigned char tuned:              1;
    unsigned char finished:           1;
    unsigned char stereo:           1;
  };
};

union reg21
{
  unsigned char value;
  struct
  {
    unsigned char read_chanel_low:    8;
  };
};

union reg22
{
  unsigned char value;
  struct
  {
    unsigned char cnr_am:           7; // AM 制式时信号的载噪比，单位 dB
    unsigned char mode3k:           1;
  };
};

union reg23
{
  unsigned char value;
  struct
  {
    unsigned char cnr_fm:               7; // FM 制式时信号的载噪比，单位 dB
    unsigned char stereo_demode:        1; // 只有 FM 立体声解调时（立体声比例大于 30%）才显示 1
  };
};

union reg24
{
  unsigned char value;
  struct
  {
    unsigned char lvmod:              1; // 低电压最大音量限制模式指示位
    unsigned char reserved:           1;
    unsigned char pgalevel_if:        3; // 射频功率控制环增益等级, 等级越大，增益约高
    unsigned char pgalevel_rf:        3; // 射频功率控制环增益等级, 等级越大，增益约高
  };
};

union reg25
{
  unsigned char value;
  struct
  {
    unsigned char vbat:           6; // 电源电压指示：vcc_bat(V)=1.8+0.05*vbat
    unsigned char reserved:       2;
  };
};

union reg26
{
  unsigned char value;
  struct
  {
    unsigned char fd_num:           8; // 频偏指示，补码格式，大于 127 时，减 256 即可变为正常数值，注意此处 FM 以 1KHz 单位，AM 以100Hz 为单位
  };
};

union reg27
{
  unsigned char value;
  struct
  {
    unsigned char rssi:           7; // 可利用 rssi、pgalevel_rf、pgalevel_if 计算天线口信号电平 FM/SW: Pin(dBuV) = 103 - rssi - 6*pgalevel_rf - 6*pgalevel_if MW/LW: Pin(dBuV) = 123 - rssi - 6*pgalevel_rf - 6*pgalevel_if
    unsigned char reserved:       1;
  };
};

//*********************************************************************************************************************//
void i2c_write( unsigned char DEVICE_ID, unsigned char reg, unsigned char val)
{
  Wire.beginTransmission(DEVICE_ID);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission();
}

unsigned char i2c_read( unsigned char DEVICE_ID, unsigned char reg)
{
  Wire.beginTransmission(DEVICE_ID);
  Wire.write(reg);
  Wire.endTransmission();
  delay(5);
  Wire.requestFrom(DEVICE_ID, sizeof(reg));
  delay(10);
  return Wire.read();
}

//*********************************************************************************************************************//
#define setReg(a) i2c_write(AKC6955, a, reg##a.value);
#define getReg(a) reg##a.value = i2c_read(AKC6955, a);

class AKC6955_RADIO
{

  public:

    union reg0 reg0;
    union reg1 reg1;
    union reg2 reg2;
    union reg3 reg3;
    union reg4 reg4;
    union reg5 reg5;
    union reg6 reg6;
    union reg7 reg7;
    union reg8 reg8;
    union reg9 reg9;

    union reg11 reg11;
    union reg12 reg12;
    union reg13 reg13;


    //************************************************************************************/
    union reg20 reg20;
    union reg21 reg21;
    union reg22 reg22;
    union reg23 reg23;
    union reg24 reg24;
    union reg27 reg27;

#define BAND_COUNT 16

    const unsigned char band_pool[BAND_COUNT] = {FM1, MW2, SW1, SW2, SW3, SW4, SW5, SW6, SW7, SW8, SW9, SW10, SW11, LW, TV1, TV2};
#if WITH_TOUCHSCREEN
    const char* band_lables[BAND_COUNT] = {"FM", "MW", "SW1", "SW2", "SW3", "SW4", "SW5", "SW6", "SW7", "SW8", "SW9", "SW10", "SW11", "LW", "TV1", "TV2"};
#endif    
    const float band_freqs_begin[BAND_COUNT] = {87, 522, 3200, 4700, 5700, 6800, 9200, 11400, 13500, 15000, 17400, 18900, 21400, 150, 56.25, 174.75};
    const float band_freqs_end[BAND_COUNT] = {108, 1620, 4100, 5600, 6400, 7600, 10000, 12200, 14300, 15900, 17900, 19700, 21900, 285, 91.75, 222.25};
    unsigned char band_cursor = 0;

    float current_frequence = 0;

    void setVolume(int volume = 48)
    {
      reg6.sound_volume = volume;
      setReg(6);
#if WITH_TOUCHSCREEN
      printVolume();
#endif
    }

    void Step(int _direction)
    {
      reg0.tune = 0;
      setReg(0);
      setFrequence(reg0.fm_mode, current_frequence + _direction * ( (reg0.fm_mode == FM) ? 0.1 : ((band_cursor == _MW2) ? 9 : ((band_cursor == _LW) ? 3 : 5))), band_freqs_begin[band_cursor], band_freqs_end[band_cursor]);
    }

    void setFrequence(int band_mode, float frequence)
    {
      unsigned int akc6955_chanel = 0;
      current_frequence = frequence;

      reg0.fm_mode = band_mode;

      if (reg0.fm_mode == FM)
      {
#if WITH_KEY
        digitalWrite(led_fm, HIGH);
        digitalWrite(led_am, LOW);
#endif
        akc6955_chanel = (frequence - 30) * 40;
      }
      else
      {
#if WITH_KEY
        digitalWrite(led_fm, LOW);
        digitalWrite(led_am, HIGH);
#endif
        reg2.mode3k = ((band_cursor == _LW) || (band_cursor == _MW2));
        akc6955_chanel = frequence / (reg2.mode3k ? 3 : 5);
      }

      reg2.chanel_freq_high = (akc6955_chanel >>  8) | 0b01100000;
      reg3.chanel_freq_low	= akc6955_chanel & 0b11111111;

      setReg(2);
      setReg(3);

      reg0.tune = 1;
      setReg(0);

      delay(50);

      getReg(20);
#if WITH_KEY
      digitalWrite(led_tuned, reg20.tuned ? HIGH : LOW);
      digitalWrite(led_stereo, reg20.stereo ? HIGH : LOW);
#endif


#if WITH_TOUCHSCREEN
      printFrequence();
#endif

    }

    void setFrequence(int band_mode, float frequence, float minfreq, float maxfreq)
    {
      if (frequence > maxfreq)
      {
        frequence = minfreq;
      }

      if (frequence < minfreq)
      {
        frequence = maxfreq;
      }
      setFrequence(band_mode, frequence);
    }

    float Seek(int seek_direction)
    {
      reg0.mute = 1;
      reg0.tune = 0;
      reg0.seek_direction = seek_direction;

      reg0.go_seek = 0; setReg(0);
      reg0.go_seek = 1; setReg(0);

      while (1)
      {

        getReg(20);
        getReg(21);

        current_frequence  = (reg0.fm_mode == FM) ? (( (reg20.read_chanel_high * 256 + reg21.read_chanel_low) * 2.5 + 3000 ) / 100 ) : (reg20.read_chanel_high * 256 + reg21.read_chanel_low) * (reg2.mode3k ? 3 : 5) ;

#if WITH_TOUCHSCREEN
        printFrequence();
#endif

        if (reg20.finished /* || reg20.tuned */)
        {
#if WITH_KEY
          digitalWrite(led_tuned, reg20.tuned ? HIGH : LOW);
          digitalWrite(led_stereo, reg20.stereo ? HIGH : LOW);
#endif

          reg0.mute = 0;
          setReg(0);
          return current_frequence ;
        }
      }
    }
#if WITH_TOUCHSCREEN
    void printFrequence()
    {
      LCD.centerText(4, 40,
                     String(String(
                              (((reg0.fm_mode == FM ) || (band_cursor == _MW2) || (band_cursor == _LW)) ? current_frequence : current_frequence / 1000),

                              (((band_cursor == _MW2) || (band_cursor == _LW)) ? 0 : ((band_cursor == _FM1) ? 1 : (((band_cursor == _TV1) || (band_cursor == _TV2)) ? 2 : 3)))
                            ) +
                            String(((band_cursor == _MW2) || (band_cursor == _LW)) ? "KHz" : "MHz")).c_str());

      if (reg0.fm_mode == FM)
      {
        getReg(23);
      }
      else
      {
        getReg(22);
      }

      getReg(24);
      getReg(27);

      LCD.Text(1, 20, 128, ((reg0.fm_mode == FM) && (reg20.stereo)) ? "Stereo" : " Mono ");
      LCD.Text(1, 80, 128, String("Signal : " + String( abs((((band_cursor == _MW2) || (band_cursor == _LW)) ? 123 : 103) - reg27.rssi - 6 * reg24.pgalevel_rf - 6 * reg24.pgalevel_if)) + String("  SNR : " + String((reg0.fm_mode == FM) ?  reg23.cnr_fm : reg22.cnr_am)) + "  ").c_str());

    }

    void printVolume()
    {
      LCD.Text(1, 235, 128, String("Volume : " + String(String(reg6.sound_volume - 23)) + " ").c_str());
    }
#endif

    void Initialize()
    {
      Wire.begin(); // AKC6955

      reg0.mute = 0;
      reg0.seek_direction = 1;
      reg0.go_seek = 0;
      reg0.tune = 0;
      reg0.fm_mode = FM;
      reg0.power_on = 1;
      setReg(0);

      reg1.fm_band = FM1;
      reg1.am_band = MW2;
      setReg(1);

      reg2.mode3k = 1;
      reg2.ref_32kmode = 1;
      setReg(2);

      reg6.phase_inv = 0;
      reg6.line_in = 0;
      reg6.sound_volume = 48;
      setReg(6);

      reg7.fm_band_width = 0b11;
      reg7.stereo = 0b10;
      reg7.bass_boost = 1;
      reg7.de = 1;
      reg7.reserved0 = 0;
      reg7.reserved1 = 0;
      setReg(7);

      reg8.stereo_threshold = 0b00;
      reg8.fd_threshold = 0b00;
      reg8.am_cnr_threshold = 0b00;
      reg8.fm_cnr_threshold = 0b01;
      setReg(8);

      reg9.lv_en = 1;
      reg9.reserved0 = 0;
      reg9.oscillator_type = 1;
      reg9.i2c_volume = 1;
      setReg(9);

      reg11.fm_seek_skip = 0b10;
      reg11.reserved1 = 0b11;
      setReg(11);

      reg13.predefined_volume = 0b11;
      reg13.st_led = 0;
      setReg(13);

#if WITH_TOUCHSCREEN
      printVolume();
#endif

      switchBand(_FM1);
    }

    void switchBand(unsigned char _band_cursor)
    {
      band_cursor = _band_cursor;

      if ((band_cursor == _FM1) || (band_cursor == _TV1) || (band_cursor == _TV2)) // FM
      {
        reg1.fm_band = band_pool[band_cursor];
        setReg(1);
        reg0.fm_mode = FM;
        reg0.tune = 0;
        setReg(0);
      }
      else
      {
        reg1.am_band = band_pool[band_cursor];
        setReg(1);
        reg2.mode3k = ((band_cursor == _MW2) || (band_cursor == _LW));
        setReg(2);
        reg0.fm_mode = AM;
        reg0.tune = 0;
        setReg(0);
      }
#if WITH_TOUCHSCREEN
      LCD.centerText(3,  90, band_lables[band_cursor]);
#endif
      setFrequence(reg0.fm_mode, band_freqs_begin[band_cursor]);

    }
};
