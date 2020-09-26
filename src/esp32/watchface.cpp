
#include "esp32/config.h"
#include <Preferences.h>

#include "esp32/MemStats.h"


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

LV_IMG_DECLARE(anime_girl);

TTGOClass * watch = nullptr;
//TFT_eSPI *tft = nullptr;

PCF8563_Class *rtc = nullptr;

// Start watch hardware systems
void watchStart() {
  Serial.begin(115200);
  Serial.println("Hello World");

  watch = TTGOClass::getWatch();
  watch->begin();
  watch->lvgl_begin();

  rtc = watch->rtc;
  //TODO: HACK This sets the clock to the moment the firmware was compiled
  //TODO: WIFI Fix by implementing wifi ntp sync
  rtc->check();
}

typedef struct {
  lv_obj_t *hour;
  lv_obj_t *minute;
  lv_obj_t *second;
} str_datetime_t;
static str_datetime_t time_labels;

// xcxc
int counter = 0;

lv_obj_t* clockScr() {
  lv_obj_t* clock_scr = lv_obj_create(NULL, NULL);

  // Background
  lv_obj_t* background = lv_img_create(clock_scr, NULL);
  lv_img_set_src(background, &anime_girl);
  lv_obj_align(background, NULL, LV_ALIGN_CENTER, 0, 0);

  //Style for numbers
  static lv_style_t style;
  lv_style_init(&style);
  lv_style_set_text_color(&style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
  lv_style_set_text_font(&style, LV_STATE_DEFAULT, &lv_font_montserrat_32);

  //Numbers
  time_labels.hour = lv_label_create(background, nullptr);
  lv_obj_add_style(time_labels.hour, LV_OBJ_PART_MAIN, &style);
  lv_label_set_text(time_labels.hour, "00");
  lv_obj_align(time_labels.hour, NULL, LV_ALIGN_CENTER, -50, 0);

  time_labels.minute = lv_label_create(background, nullptr);
  lv_obj_add_style(time_labels.minute, LV_OBJ_PART_MAIN, &style);
  lv_label_set_text(time_labels.minute, "00");
  lv_obj_align(time_labels.minute, NULL, LV_ALIGN_CENTER, 0, 0);

  time_labels.second = lv_label_create(background, nullptr);
  lv_obj_add_style(time_labels.second, LV_OBJ_PART_MAIN, &style);
  lv_label_set_text(time_labels.second, "00");
  lv_obj_align(time_labels.second, NULL, LV_ALIGN_CENTER, 50, 0);

  //lv_obj_t * label1 = lv_label_create(clock_scr, NULL);
  //lv_label_set_text(label1, LV_SYMBOL_OK);

  return clock_scr;
}

void update_clock(lv_task_t *t) {
  //lv_obj_t* clock_scr = clockScr();
  //lv_scr_load_anim(clock_scr, LV_SCR_LOAD_ANIM_MOVE_TOP, 990, 0, true);

  // xcxc use the RTC directly
  RTC_Date curr_datetime = rtc->getDateTime();

  lv_label_set_text_fmt(time_labels.hour, "%02u", curr_datetime.hour);
  lv_label_set_text_fmt(time_labels.minute, "%02u", curr_datetime.minute);
  lv_label_set_text_fmt(time_labels.second, "%02u", curr_datetime.second);

  //printRamStats();
}

void setup() {
  // put your setup code here, to run once:
  watchStart();
  printRamStats();
  //  printNvsStats();
  //  printFlashStats();

  watch->openBL();
  watch->bl->adjust(100);

  //tft = watch->tft;
  //tft->setTextFont(2);
  //tft->println("Hello World");

  lv_obj_t* clock_scr = clockScr();
  lv_scr_load(clock_scr);

  lv_task_create(update_clock, 50, LV_TASK_PRIO_MID, nullptr);
}

void loop() {
  // put your main code here, to run repeatedly:
  // This is the main Task in FreeRTOS... but WE CAN MAKE OTHERS!
  lv_task_handler();
}
