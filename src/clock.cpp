//#include "clock.h"

#include "watchglobals.h"

#include <lvgl/lvgl.h>

extern "C" {
	LV_IMG_DECLARE(anime_girl);
}

// Labels that have H:M:S of time
typedef struct {
  lv_obj_t *hour;
  lv_obj_t *minute;
  lv_obj_t *second;
} str_datetime_t;
static str_datetime_t time_labels;

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
  time_labels.hour = lv_label_create(background, NULL);
  lv_obj_add_style(time_labels.hour, LV_OBJ_PART_MAIN, &style);
  lv_label_set_text(time_labels.hour, "00");
  lv_obj_align(time_labels.hour, NULL, LV_ALIGN_CENTER, -50, 0);

  time_labels.minute = lv_label_create(background, NULL);
  lv_obj_add_style(time_labels.minute, LV_OBJ_PART_MAIN, &style);
  lv_label_set_text(time_labels.minute, "00");
  lv_obj_align(time_labels.minute, NULL, LV_ALIGN_CENTER, 0, 0);

  time_labels.second = lv_label_create(background, NULL);
  lv_obj_add_style(time_labels.second, LV_OBJ_PART_MAIN, &style);
  lv_label_set_text(time_labels.second, "00");
  lv_obj_align(time_labels.second, NULL, LV_ALIGN_CENTER, 50, 0);

  //lv_obj_t * label1 = lv_label_create(clock_scr, NULL);
  //lv_label_set_text(label1, LV_SYMBOL_OK);

  return clock_scr;
}

int counter = 0;

void update_clock(lv_task_t *t) {

	// Get time from the RTC directly

	RTC_Date curr_datetime = rtc->getDateTime();

	lv_label_set_text_fmt(time_labels.hour, "%02u", curr_datetime.hour);
	lv_label_set_text_fmt(time_labels.minute, "%02u", curr_datetime.minute);
	lv_label_set_text_fmt(time_labels.second, "%02u", curr_datetime.second);
}