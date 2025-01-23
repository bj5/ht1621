/*******************************************************************************
Copyright (c) 2016-2018 anxzhu (github.com/anxzhu)
Copyright (c) 2018 Valerio Nappi (github.com/5N44P) (changes)
Copyright (c) 2024 kreddkrikk (github.com/kreddkrikk) (changes)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*******************************************************************************/

#ifndef HT1621_H_
#define HT1621_H_      // Prevent double-include

#define BIAS     0x52  // LCD 1/3 bias, 4 commons
#define SYSDIS   0X00  // Turn off system oscillator and LCD bias generator
#define SYSEN    0X02  // Turn on system oscillator
#define LCDOFF   0X04  // Turn off LCD bias generator
#define LCDON    0X06  // Turn on LCD bias generator
#define XTAL     0x28  // System clock source, crystal oscillator
#define RC256    0X30  // System clock source, on-chip RC oscillator
#define TONEON   0X12  // Turn on tone outputs
#define TONEOFF  0X10  // Turn off tone outputs
#define WDTDIS1  0X0A  // Disable watchdog
#define HT_BUF_SIZE 12

#define HT_SEG_END 6   // Last offset of segments
#define HT_DOT_START 3 // Starting offset of decimals

/******************************************************************************
 * 4-digit display settings
 *
 * Often marked as LCM0220. These displays are shorter and only have 4 digits 
 * and a colon. The HT1621 is made for 6-digit displays and reserves 6 bytes 
 * for segment data at the start of the buffer. For 4-digit displays, segment 
 * data begins 2 bytes from the start of the buffer.
 *****************************************************************************/
#define W4_HT_SEG_START 2  // Starting offset of segments
#define W4_HT_DOT_REL -1   // Offset of decimal relative to previous digit
#define W4_HT_BAT_OFF -1   // Offset of battery symbol (not used here)
#define W4_HT_COL_OFF 5    // Offset of colon symbol

// Value limits
#define W4_MAX_NUM 9999
#define W4_MIN_NUM -999
#define W4_MAX_TEMP 999
#define W4_MIN_TEMP -99
/// AFEHBGCD  4-digit 7 segment LCD (58A) 
// ASCII-to-segment-bits table. Table starts at space (0x20)
static const char w4_ascii[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcc, 0x00, 0x00, 0x40, 0x00, 0x00, 
//  0-9
    0xeb, 0x0a, 0xad, 0x8f, 0x4e, 0xc7, 0xe7, 0x8a, 0xef, 0xcf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
//        A      B     C     D     E    F     G      H    I     J     K     L      M    N     O 
    0x00, 0xee, 0x67, 0xe1, 0x2f, 0xe5, 0xe4, 0xe3, 0x6e, 0x0a, 0x83, 0x00, 0x61, 0x00, 0xea, 0xeb, 
//  P      Q    R      S     T    U     V     W     X     Y      Z
    0xec, 0xce, 0x24, 0xc7, 0x65, 0x6b, 0x00, 0x00, 0x00, 0x4e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
//        a-z
    0x00, 0xee, 0x67, 0x25, 0x2f, 0xe5, 0xe4, 0x4f, 0xe6, 0x60, 0xa1, 0x00, 0x60, 0x00, 0x26, 0x27, 
    0xce, 0xce, 0x24, 0xc7, 0x65, 0x23, 0x00, 0x00, 0x00, 0x4e, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00
};

#define W4_SEG_P 0b00010000

/****************************************************************************** 
 * 6-digit display settings
 *
 * Often marked as PDC-6X1.
 *****************************************************************************/
#define W6_HT_SEG_START 0
#define W6_HT_DOT_REL 0
#define W6_HT_BAT_OFF 0
#define W6_HT_COL_OFF -1   // Not used

// Value limits
#define W6_MAX_NUM 999999
#define W6_MIN_NUM -99999
#define W6_MAX_TEMP 9999
#define W6_MIN_TEMP -999

static const char w6_ascii[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x00, 0x00, 0x02, 0x00, 0x00, 
    0x7d, 0x60, 0x3e, 0x7a, 0x63, 0x5b, 0x5f, 0x70, 0x7f, 0x7b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x77, 0x4f, 0x1d, 0x6e, 0x1f, 0x17, 0x5d, 0x67, 0x60, 0x68, 0x00, 0x0d, 0x00, 0x46, 0x4e, 
    0x37, 0x73, 0x06, 0x5b, 0x0f, 0x6d, 0x00, 0x00, 0x00, 0x6b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 
    0x00, 0x77, 0x4f, 0x1d, 0x6e, 0x1f, 0x17, 0x5d, 0x67, 0x60, 0x68, 0x00, 0x0d, 0x00, 0x46, 0x4e, 
    0x37, 0x73, 0x06, 0x5b, 0x0f, 0x6d, 0x00, 0x00, 0x00, 0x6b, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00
};

#define W6_SEG_P 0b10000000

#define MAX_CHAR 20        // Size of local string buffers
#define MAX_PRECISION 3    // Maximum precision for floating point numbers

// Error codes
enum {
    ERR_INV_NUM,    // Number out of bounds
    ERR_INV_PREC,   // Invalid precision
    ERR_INV_TEMP,   // Temperature out of bounds
    ERR_INV_CHAR,   // Non-displayable character
    ERR_INV_STR,    // Non-displayable string
    ERR_INV_UNIT,   // Invalid temperature unit
    ERR_INV_CODE,   // Invalid error code
};

class  HT1621
{
    public:
        HT1621();

        // Activate the display. Acceptable values for width are 6 for 6-digit
        // displays and 4 for 4-digit displays.
        void begin(int width, int cs_p, int wr_p, int data_p);

        // Activate the display with backlight control.
        void begin(int width, int cs_p, int wr_p, int data_p, int backlight_p);

        // Power down the display. This reduces power consumption to <0.1uA.
        void powerDown();

        // Bring the display out of power down status.
        void powerUp();

        // Turn off MCU pins, but keep the display active. This reduces power
        // consumption by about 200uA. The display cannot be updated while in 
        // standby mode until active() is called.
        void standBy();

        // Come out of standby mode to update the display.
        void active();

        // Clear the display.
        void clear();

        // Turn the display on.
        void lcdOn();

        // Turn the display off.
        void lcdOff();

        // Turn on the backlight.
        void backlightOn();

        // Turn off the backlight.
        void backlightOff();

        // Base function for all other print functions.
        void print(const char* str);

        // Print floating value, right-aligned.
        void print(double num, int precision);

        // Print 16-bit integer, right-aligned.
        void print(int num);

        // Print 32-bit integer, right-aligned.
        void print(long num);

        // Print the temperature, optionally including degree symbol and unit. 
        // Set unit to NULL to hide the unit symbol.
        void printTemp(double num, char unit, bool degree);

        // Print clock/timer number pair separated by colon or decimal, with 
        // the first number optionally padded by leading zeros.
        void printTime(bool pad, int num1, int num2);

        // Print an error code to the display.
        void printError(int err);

        // Set the battery indicator.
        void batteryLevel(int level);

    private:
        int _cs_p;
        int _wr_p;
        int _data_p;
        int _backlight_p;
        bool _backlight_en;
        char _ht_buf[HT_BUF_SIZE];
        int _lcd_width;
        long _max_num;
        long _min_num;
        long _max_temp;
        long _min_temp;
        char *_ascii;
        char _seg_p;
        int _ht_dot_rel;
        int _ht_bat_off;
        int _ht_col_off;
        int _ht_seg_start;

        // Buffer-writing functions
        void wrone(uint8_t addr, uint8_t sdata);
        void wrclrdata(uint8_t addr, uint8_t sdata);
        void wrCLR(uint8_t len);
        void wrDATA(uint8_t data, uint8_t cnt);
        void wrCMD(uint8_t CMD);
        //j5 20250123
		void HT1621::reverse_string(char* str);
        
        // Fixes issues with segments appearing too dark when off.
        void wrlow();

        // Configure the display type and hardware settings.
        void config();

        // Takes the buffer and puts it straight into the driver.
        void update();
};
#endif
