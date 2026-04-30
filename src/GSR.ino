#include "Watchy_GSR.h"
#if __has_include("GSRWatchFaceBallsy.h")
#include "GSRWatchFaceBallsy.h"
#endif
#if __has_include("GSRWatchFaceLCD.h")
#include "GSRWatchFaceLCD.h"
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

RTC_DATA_ATTR uint8_t BadForEyeStyleID;

class OverrideGSR : public WatchyGSR {
  public:
    OverrideGSR() : WatchyGSR() {}

    void InsertAddWatchStyles() {
        BadForEyeStyleID = AddWatchStyle("BadForEye");
    };

    void InsertDrawWatchStyle(uint8_t StyleID) {
        if (StyleID == BadForEyeStyleID) {
            drawBadForEyeFace();
        }
    };

  private:
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
};

// Do not edit anything below this, leave all of your code above.
OverrideGSR watchy;

void setup(){
  watchy.init();
}

void loop(){}
