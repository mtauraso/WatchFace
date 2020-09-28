
#include "watchglobals.h"
#include "clock.h"
#include "background.h"

// Labels that have H:M:S of time
typedef struct {
  lv_obj_t *hour;
  lv_obj_t *minute;
  lv_obj_t *second;
} str_datetime_t;
static str_datetime_t time_labels;

lv_obj_t* clock_create(lv_obj_t* parent) {
  lv_obj_t* clock_scr = lv_obj_create(parent, NULL);

  // Put the background in
  lv_obj_t* background = background_create(clock_scr);

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

  return clock_scr;
}

void clock_update(lv_task_t *t) {
	// Get time from the the RTC directly
	RTC_Date curr_datetime = rtc->getDateTime();

	lv_label_set_text_fmt(time_labels.hour, "%02u", curr_datetime.hour);
	lv_label_set_text_fmt(time_labels.minute, "%02u", curr_datetime.minute);
	lv_label_set_text_fmt(time_labels.second, "%02u", curr_datetime.second);
}