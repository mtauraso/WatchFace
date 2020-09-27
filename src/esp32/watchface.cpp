
#include "esp32/config.h"
#include <Preferences.h>

#include "esp32/MemStats.h"

#include "clock.h"
#include "watchglobals.h"


// TODO: Load Prefs from flash
#if 0
struct WatchFacePreferences {
  const char * wifi_ssid;
  const char * wifi_pw;
};

// TODO: Load Preferences from flash
void loadPreferences() {
  preferences.begin("WatchFace", false);

  const char default_wifi_ssid[] = "511-224th";
  const char default_wifi_pw[] = "dorothydog";
}
Preferences preferences;
#endif

// Goals
//   Make a watch face for the T-Watch 2020
//    X Print debug on serial prot
//    - Should display the time  HH:MM:SS
//    - Should update the time every second
//    - Should have a background image
//    - Do something useful on suspend?
//
//   Additional feature ideas
//    - Wifi (Hardcode SSID/PW, use for NTP, Display connect status)
//    - Battery Meter (Display battery %, Display charging/not)

TTGOClass * watch = nullptr;
//TFT_eSPI *tft = nullptr;


// Start watch hardware systems
void watchStart() {
  Serial.begin(115200);
  Serial.println("Hello World 2");

  watch = TTGOClass::getWatch();
  watch->begin();
  watch->lvgl_begin();

  // TODO, pull this off into initialization code on clock
  rtc = watch->rtc;
  //TODO: HACK This sets the clock to the moment the firmware was compiled
  //TODO: WIFI Fix by implementing wifi ntp sync
  rtc->check();
}


void setup() {
  // put your setup code here, to run once:
  watchStart();
  printRamStats();
  //printNvsStats();
  //printFlashStats();

  watch->openBL();
  watch->bl->adjust(100);

  lv_obj_t* clock_scr = clockScr();
  lv_scr_load(clock_scr);

  lv_task_create(update_clock, 50, LV_TASK_PRIO_MID, nullptr);
}

void loop() {
  // put your main code here, to run repeatedly:
  // This is the main Task in FreeRTOS... but WE CAN MAKE OTHERS!
  lv_task_handler();
}
