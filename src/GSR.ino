#include "Watchy_GSR.h"
#if __has_include("GSRWatchFaceBallsy.h")
#include "GSRWatchFaceBallsy.h"
#endif
#if __has_include("GSRWatchFaceLCD.h")
#include "GSRWatchFaceLCD.h"
#endif
#if __has_include("GSRWatchFaceQlock.h")
#include "GSRWatchFaceQlock.h"
#endif
#if __has_include("GSRWatchFaceBadForEye.h")
#include "GSRWatchFaceBadForEye.h"
#endif
#if __has_include("WeatherIcons.h")
#include "WeatherIcons.h"
#if __has_include("WatchyClassicsAddOn.h")
#include "WatchyClassicsAddOn.h"
#endif
#endif

RTC_DATA_ATTR uint8_t QlockStyleID;
RTC_DATA_ATTR uint8_t BadForEyeStyleID;

class OverrideGSR : public WatchyGSR {
  public:
    OverrideGSR() : WatchyGSR() {}

    void InsertAddWatchStyles() {
        QlockStyleID = AddWatchStyle("Qlock");
        BadForEyeStyleID = AddWatchStyle("BadForEye");
    };

    void InsertDrawWatchStyle(uint8_t StyleID) {
        #if __has_include("GSRWatchFaceQlock.h")
        if (StyleID == QlockStyleID) {
            drawQlockFace();
            return;
        }
        #endif
        #if __has_include("GSRWatchFaceBadForEye.h")
        if (StyleID == BadForEyeStyleID) {
            drawBadForEyeFace();
            return;
        }
        #endif
    };

  private:
    #if __has_include("GSRWatchFaceQlock.h")
    std::bitset<11> qbmap[10];

    void qResetBitmap() {
        qbmap[0] = 0b00100111111;
        qbmap[1] = 0b01000000011;
        qbmap[2] = 0b00000000001;
        qbmap[3] = 0b00001000100;
        qbmap[4] = 0b00001111111;
        for (int16_t i = 5; i < 9; i++) qbmap[i] = qbmap[i].set();
        qbmap[9] = 0b11111000000;
    }

    void qSetHours() {
        auto hr = WatchTime.Local.Hour % 12;
        if (hr == 0) hr = 12;
        if (WatchTime.Local.Minute > 35) { hr++; if (hr > 12) hr = 1; }
        switch (hr) {
            case 1:  qbmap[5] ^= 0b11100000000; break;
            case 2:  qbmap[6] ^= 0b00000000111; break;
            case 3:  qbmap[5] ^= 0b00000011111; break;
            case 4:  qbmap[6] ^= 0b11110000000; break;
            case 5:  qbmap[6] ^= 0b00001111000; break;
            case 6:  qbmap[5] ^= 0b00011100000; break;
            case 7:  qbmap[8] ^= 0b11111000000; break;
            case 8:  qbmap[7] ^= 0b11111000000; break;
            case 9:  qbmap[4] ^= 0b00000001111; break;
            case 10: qbmap[9] ^= 0b11100000000; break;
            case 11: qbmap[7] ^= 0b00000111111; break;
            case 12: qbmap[8] ^= 0b00000111111; break;
        }
    }

    void qSetMinutes() {
        auto m = WatchTime.Local.Minute;
        auto diff = m - m % 5;
        if (diff != 0) qbmap[9] |= 0b00000111111;
        if (diff >= 35) qbmap[4] |= 0b11110000000;
        switch (diff) {
            case 0:  qbmap[1].set(); qbmap[2].set(); qbmap[3].set(); qbmap[4] |= 0b11110000000; break;
            case 5:  qbmap[1].set(); qbmap[2] |= 0b11111100000; qbmap[3].set(); break;
            case 10: qbmap[1].set(); qbmap[2].set(); qbmap[3] |= 0b11110000011; break;
            case 15: qbmap[2].set(); qbmap[3].set(); qbmap[9] |= 0b00000111111; break;
            case 20: qbmap[1].set(); qbmap[2] |= 0b00000011110; qbmap[3].set(); break;
            case 25: qbmap[1].set(); qbmap[3].set(); break;
            case 30: qbmap[1].set(); qbmap[2].set(); qbmap[3] |= 0b00000111011; break;
            case 35: qbmap[1].set(); qbmap[3] |= 0b11110111000; break;
            case 40: qbmap[1].set(); qbmap[2] |= 0b00000011110; qbmap[3] |= 0b11110111000; break;
            case 45: qbmap[2].set(); qbmap[3] |= 0b11111111100; break;
            case 50: qbmap[1].set(); qbmap[2].set(); qbmap[3] |= 0b11110000000; break;
            case 55: qbmap[1].set(); qbmap[2] |= 0b11111100000; qbmap[3] |= 0b11110111000; break;
        }
        int rem = m % 5;
        if (rem == 0) display.drawBitmap(0, 0, epd_bitmap_chequerboard_qlock, 8, 8, GxEPD_WHITE);
        if (rem <= 1) display.drawBitmap(192, 0, epd_bitmap_chequerboard_qlock, 8, 8, GxEPD_WHITE);
        if (rem <= 2) display.drawBitmap(192, 192, epd_bitmap_chequerboard_qlock, 8, 8, GxEPD_WHITE);
        if (rem <= 3) display.drawBitmap(0, 192, epd_bitmap_chequerboard_qlock, 8, 8, GxEPD_WHITE);
    }

    void qDrawLetters() {
        uint16_t bitset;
        for (size_t y = 0; y < 10; y++) {
            bitset = (int)qbmap[y].to_ulong();
            size_t x = 0;
            while (bitset != 0) {
                if (bitset & 0x1) {
                    display.fillRect(13 + (10 - x) * 16, 11 + y * 18, 14, 16, GxEPD_BLACK);
                }
                bitset >>= 1;
                x++;
            }
        }
    }

    void drawQlockFace() {
        display.fillScreen(GxEPD_BLACK);
        display.drawBitmap(0, 0, epd_bitmap_qlock_bg, 200, 200, GxEPD_WHITE, GxEPD_BLACK);
        qResetBitmap();
        qSetHours();
        qSetMinutes();
        qDrawLetters();
    }
    #endif

    #if __has_include("GSRWatchFaceBadForEye.h")
    void drawBadForEyeFace() {
        const unsigned char *numbers[10] = {
            numbers0, numbers1, numbers2, numbers3, numbers4,
            numbers5, numbers6, numbers7, numbers8, numbers9
        };
        display.fillScreen(GxEPD_BLACK);
        display.drawBitmap(0, 0, window, 200, 200, GxEPD_WHITE);
        display.drawBitmap(50,  10,  numbers[WatchTime.Local.Hour / 10],   39, 80, GxEPD_BLACK);
        display.drawBitmap(110, 10,  numbers[WatchTime.Local.Hour % 10],   39, 80, GxEPD_BLACK);
        display.drawBitmap(50,  110, numbers[WatchTime.Local.Minute / 10], 39, 80, GxEPD_BLACK);
        display.drawBitmap(110, 110, numbers[WatchTime.Local.Minute % 10], 39, 80, GxEPD_BLACK);
    }
    #endif
};

// Do not edit anything below this, leave all of your code above.
OverrideGSR watchy;

void setup(){
  watchy.init();
}

void loop(){}
