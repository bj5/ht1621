#include <HT1621.h>

#define W6_PIN_CS 12
#define W6_PIN_WR 11
#define W6_PIN_DATA 10
#define W6_PIN_BACKLIGHT 9

#define W4_PIN_CS 8
#define W4_PIN_WR 7
#define W4_PIN_DATA 6
#define W4_PIN_BACKLIGHT 5

HT1621 lcd_w4;
HT1621 lcd_w6;

void setup() {
    // 6-digit display
    lcd_w6.begin(6, W6_PIN_CS, W6_PIN_WR, W6_PIN_DATA, W6_PIN_BACKLIGHT);
    lcd_w6.backlightOn();
    lcd_w6.print(123.456, 3);
    lcd_w6.batteryLevel(2);
    
    // 4-digit display
    lcd_w4.begin(4, W4_PIN_CS, W4_PIN_WR, W4_PIN_DATA, W4_PIN_BACKLIGHT);
    lcd_w4.backlightOn();
    lcd_w4.printTime(true, 12, 34);
}

void loop() {
}
