/*******************************************************************************
Copyright 2016-2018 anxzhu (github.com/anxzhu)
Copyright 2018-2020 Valerio Nappi (github.com/5N44P) (changes)
Based on segment-lcd-with-ht1621 from anxzhu (2016-2018)
(https://github.com/anxzhu/segment-lcd-with-ht1621)

Partially rewritten and extended by Valerio Nappi (github.com/5N44P) in 2018

Rewritten again (github.com/kreddkrikk) with added support for 4-digit displays
and a focus on low power consumption.

This file is part of the HT1621 arduino library, and thus under the MIT license.
More info on the project and the license conditions on:

https://github.com/kreddkrikk/ht1621

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*******************************************************************************/

#include <Arduino.h>
#include "HT1621.h"

HT1621::HT1621() { }

void HT1621::begin(int width, int cs_p, int wr_p, int data_p, int backlight_p)
{
    pinMode(cs_p, OUTPUT);
    pinMode(wr_p, OUTPUT);
    pinMode(data_p, OUTPUT);
    pinMode(backlight_p, OUTPUT);
    _cs_p = cs_p;
    _wr_p = wr_p;
    _data_p = data_p;
    _backlight_p = backlight_p;
    _backlight_en = true;
    _lcd_width = width;
    config();
}

void HT1621::begin(int width, int cs_p, int wr_p, int data_p)
{
    pinMode(cs_p, OUTPUT);
    pinMode(wr_p, OUTPUT);
    pinMode(data_p, OUTPUT);
    _cs_p=cs_p;
    _wr_p=wr_p;
    _data_p=data_p;
    _backlight_en = false;
    _lcd_width = width;
    config();
}

void HT1621::wrDATA(uint8_t data, uint8_t cnt) {
    for (int i = 0; i < cnt; i++) {
        digitalWrite(_wr_p, LOW);
        delayMicroseconds(4);
        if (data & 0x80) {
            digitalWrite(_data_p, HIGH);
        }
        else {
            digitalWrite(_data_p, LOW);
        }
        digitalWrite(_wr_p, HIGH);
        delayMicroseconds(4);
        data <<= 1;
    }
}

void HT1621::wrCMD(uint8_t CMD) {
    digitalWrite(_cs_p, LOW);
    wrDATA(0x80, 4);
    wrDATA(CMD, 8);
    digitalWrite(_cs_p, HIGH);
}

void HT1621::wrCLR(uint8_t len) {
    uint8_t addr = 0;
    for (int i = 0; i < len; i++) {
        wrclrdata(addr, 0x00);
        addr = addr + 2;
    }
    wrlow();
}

void HT1621::wrclrdata(uint8_t addr, uint8_t sdata)
{
    addr <<= 2;
    digitalWrite(_cs_p, LOW);
    wrDATA(0xa0, 3);
    wrDATA(addr, 6);
    wrDATA(sdata, 8);
    digitalWrite(_cs_p, HIGH);
}

void HT1621::wrone(uint8_t addr, uint8_t sdata)
{
    addr <<= 2;
    digitalWrite(_cs_p, LOW);
    wrDATA(0xa0, 3);
    wrDATA(addr, 6);
    wrDATA(sdata, 8);
    digitalWrite(_cs_p, HIGH);
}

void HT1621::wrlow() {
    digitalWrite(_cs_p, LOW);
    digitalWrite(_wr_p, LOW);
    digitalWrite(_data_p, LOW);
}

void HT1621::config()
{
    if (_lcd_width == 4) {
        _max_num = W4_MAX_NUM;
        _min_num = W4_MIN_NUM;
        _max_temp = W4_MAX_TEMP;
        _min_temp = W4_MIN_TEMP;
        _ascii = &w4_ascii[0];
        _seg_p = W4_SEG_P;
        _ht_dot_rel = W4_HT_DOT_REL;
        _ht_bat_off = W4_HT_BAT_OFF;
        _ht_col_off = W4_HT_COL_OFF;
        _ht_seg_start = W4_HT_SEG_START;
    }
    else if (_lcd_width == 6) {
        _max_num = W6_MAX_NUM;
        _min_num = W6_MIN_NUM;
        _max_temp = W6_MAX_TEMP;
        _min_temp = W6_MIN_TEMP;
        _ascii = &w6_ascii[0];
        _seg_p = W6_SEG_P;
        _ht_dot_rel = W6_HT_DOT_REL;
        _ht_bat_off = W6_HT_BAT_OFF;
        _ht_col_off = W6_HT_COL_OFF;
        _ht_seg_start = W6_HT_SEG_START;
    }
    wrCMD(SYSDIS);
    wrCMD(BIAS);
    wrCMD(RC256);
    wrCMD(WDTDIS1);
    wrCMD(SYSEN);
    wrCMD(LCDON);
    wrlow();
    clear();
}

void HT1621::lcdOn()
{
    wrCMD(LCDON);
}

void HT1621::lcdOff()
{
    wrCMD(LCDOFF);
}

void HT1621::standBy() {
    pinMode(_cs_p, INPUT);
    pinMode(_wr_p, INPUT);
    pinMode(_data_p, INPUT);
    if (_backlight_en)
        pinMode(_backlight_p, INPUT);
}

void HT1621::active() {
    pinMode(_cs_p, OUTPUT);
    pinMode(_wr_p, OUTPUT);
    pinMode(_data_p, OUTPUT);
    if (_backlight_en)
        pinMode(_backlight_p, OUTPUT);
}

void HT1621::powerUp()
{
    active();
    wrCMD(SYSEN);
    wrCMD(LCDON);
}

void HT1621::powerDown()
{
    wrCMD(LCDOFF);
    wrCMD(SYSDIS);
    standBy();
}

void HT1621::clear() {
    wrCLR(16);
    wrlow();
}

void HT1621::backlightOn()
{
    if (_backlight_en)
        digitalWrite(_backlight_p, HIGH);
    delay(1);
}

void HT1621::backlightOff()
{
    if (_backlight_en)
        digitalWrite(_backlight_p, LOW);
    delay(1);
}

void HT1621::update() {
    // Buffer is backwards with respect to the LCD
    wrone(0, _ht_buf[5]);
    wrone(2, _ht_buf[4]);
    wrone(4, _ht_buf[3]);
    wrone(6, _ht_buf[2]);
    wrone(8, _ht_buf[1]);
    wrone(10, _ht_buf[0]);
    wrlow();
}

void HT1621::print(const char* str) {
    // Clear HT1621 buffer
    for (int i = 0; i < HT_SEG_END; i++)
        _ht_buf[i] = 0;

    // Setup HT1621 buffer
    int b = _ht_seg_start, s = 0;
    while (b < HT_SEG_END && str[s]) {
        if (str[s] == '.') {
            if (b >= HT_DOT_START)
                _ht_buf[b + _ht_dot_rel] |= _seg_p;
            else {
                printError(-ERR_INV_STR);
                return;
            }
            s++;
        }
        else if (str[s] == ':') {
            if (_lcd_width == 4 && s == 2)
                _ht_buf[_ht_col_off] |= _seg_p;
            else {
                printError(-ERR_INV_STR);
                return;
            }
            s++;
        }
        _ht_buf[b++] |= _ascii[str[s++] - 0x20];
    }

    // Done
    update();
}

void HT1621::print(double num, int precision)
{
    if (num > _max_num || num < _min_num) {
        printError(-ERR_INV_NUM);
        return;
    }
    char str[MAX_CHAR];
    int width = _lcd_width + 1; // +1 for decimal character
    if (precision > MAX_PRECISION)
        precision = MAX_PRECISION;
    if (precision == 0)
        width--;

    // Convert to right-aligned string
    dtostrf(num, width, precision, str);

    // Done
    print(str);
}

void HT1621::print(int num) {
    print((double)num, 0);
}

void HT1621::print(long num) {
    print((double)num, 0);
}

void HT1621::printTemp(double num, char unit, bool degree)
{
    if (num > _max_temp || num < _min_temp) {
        printError(-ERR_INV_TEMP);
        return;
    }
    char str[MAX_CHAR];
    int width = _lcd_width + 1;

    // Convert to right-aligned string
    dtostrf(num, width, MAX_PRECISION, str);

    // Write degree/unit. It's much simpler to just overwrite instead of 
    // adjusting the precision and width based on the decimal or degree.
    if (degree) {
        // Shift out anomalous decimal
        if (str[width - 3] == '.')
            width--;
        str[width - 2] = '*';
    }
    if (unit != '\0') {
        if (str[width - 2] == '.')
            width--;
        str[width - 1] = unit;
    }
    str[width] = '\0';

    // Done
    print(str);
}

void HT1621::printTime(bool pad, int num1, int num2)
{
    char str[MAX_CHAR];
    char fmt[MAX_CHAR];
    char sep = _ht_col_off != -1 ? ':' : '.';
    if (pad)
        snprintf(fmt, MAX_CHAR, "%%0%dd%c%%02d", _lcd_width - 2, sep);
    else
        snprintf(fmt, MAX_CHAR, "%%%dd%c%%02d", _lcd_width - 2, sep);
    snprintf(str, MAX_CHAR, fmt, num1, num2);

    // Done
    print(str);
}

void HT1621::printError(int err)
{
    if (err < -ERR_INV_CODE || err >= 0)
        err = -ERR_INV_CODE; // Bad error code

    // Setup HT1621 buffer
    char *errStr = "Error";
    char errChar = '0' + abs(err);
    for (int i = _ht_seg_start; i < HT_SEG_END; i++, errStr++)
        _ht_buf[i] = _ascii[*errStr - 0x20];
    _ht_buf[HT_SEG_END - 1] = _ascii[errChar - 0x20];

    // Done
    update();
}

void HT1621::batteryLevel(int level) {
    // Clear battery level bits
    _ht_buf[0] &= ~_seg_p;
    _ht_buf[1] &= ~_seg_p;
    _ht_buf[2] &= ~_seg_p;

    if (level >= 1)
        _ht_buf[_ht_bat_off + 2] |= _seg_p;
    if (level >= 2)
        _ht_buf[_ht_bat_off + 1] |= _seg_p;
    if (level >= 3)
        _ht_buf[_ht_bat_off] |= _seg_p;

    // Done
    update();
}
