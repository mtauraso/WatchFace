
#include "esp32/config.h"
#include <Preferences.h>

#include "esp32/MemStats.h"


#include "watchglobals.h"
#include "clock.h"
#include "clock_2.h"
#include "test_pattern.h"


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
PCF8563_Class * rtc = nullptr;
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

int bl = 0;
void ch_bl(lv_task_t * ) {
  bl += 10;
  watch->bl->adjust(bl);
  Serial.print("Backlight");
  Serial.print(bl);
  Serial.print("\n");
}


void setup() {
  // put your setup code here, to run once:
  watchStart();
  printRamStats();
  //printNvsStats();
  //printFlashStats();

  watch->openBL();
  
  // 1/2 of max possible backlight
  watch->bl->adjust(255*1/3);

  //lv_scr_load(clock_2_create(NULL));
  //lv_scr_load(test_pattern_create(NULL, GAMMA_CHECKERBOARD));
  //lv_scr_load(test_pattern_create(NULL, CONTRAST_BARS));
  //lv_scr_load(color_val_test(nullptr, lv_color_make(31 << 3, 0, 31 << 3)));
  lv_scr_load(test_pattern_create(NULL, BLACK_LEVEL_SQUARES));
  //lv_task_create(ch_bl, 1000, LV_TASK_PRIO_MID, nullptr);
  //lv_task_create(clock_2_update, 50, LV_TASK_PRIO_MID, nullptr);
}


void loop() {
  // put your main code here, to run repeatedly:
  // This is the main Task in FreeRTOS... but WE CAN MAKE OTHERS!
  lv_task_handler();
}
