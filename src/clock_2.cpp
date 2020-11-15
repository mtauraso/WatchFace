#include "watchglobals.h"
#include "background.h"
#include "clock_2.h"



// Labels that have H:M:S of time
typedef struct {
	lv_obj_t* hour;
	lv_obj_t* minute;
	lv_obj_t* second;
//	lv_obj_t* sec_digit;
} str_datetime_t;
static str_datetime_t time_labels;

lv_obj_t* clock_2_create(lv_obj_t* parent) {
	lv_obj_t* clock_scr = lv_obj_create(parent, NULL);
	lv_obj_set_size(clock_scr, LV_HOR_RES, LV_VER_RES);

	// Background
	lv_obj_t* background = background_create(clock_scr);

	// Bernadette light pink is lv_color_hsv_to_rgb(333, 10, 100);
	lv_color_t color = lv_color_hsv_to_rgb(333, 30, 100);

	//Style for numbers
	static lv_style_t style;
	lv_style_init(&style);
	lv_style_set_text_color(&style, LV_STATE_DEFAULT, color);
	lv_style_set_text_font(&style, LV_STATE_DEFAULT, &lv_font_montserrat_32);

	// Style of Gauge for seconds
	static lv_style_t gauge_style_main;
	lv_style_init(&gauge_style_main);
	lv_style_set_line_width(&gauge_style_main, LV_STATE_DEFAULT, 2);
	lv_style_set_line_color(&gauge_style_main, LV_STATE_DEFAULT, color);
	lv_style_set_scale_grad_color(&gauge_style_main, LV_STATE_DEFAULT, color);
	lv_style_set_scale_end_color(&gauge_style_main, LV_STATE_DEFAULT, color);
	lv_style_set_bg_opa(&gauge_style_main, LV_STATE_DEFAULT, LV_OPA_TRANSP);
	lv_style_set_border_opa(&gauge_style_main, LV_STATE_DEFAULT, LV_OPA_TRANSP);
	lv_style_set_text_opa(&gauge_style_main, LV_STATE_DEFAULT, LV_OPA_TRANSP);
	//lv_style_set_scale_border_width (&gauge_style_main, LV_STATE_DEFAULT, 30);
	

	// Style for Gauge Neelde
	static lv_style_t gauge_style_needle;
	lv_style_init(&gauge_style_needle);
	lv_style_set_line_opa(&gauge_style_needle, LV_STATE_DEFAULT, LV_OPA_TRANSP);
	lv_style_set_size(&gauge_style_needle, LV_STATE_DEFAULT, 0);

	// Style for Gauge major delineators
	static lv_style_t gauge_style_major;
	lv_style_init(&gauge_style_major);
	lv_style_set_line_width(&gauge_style_major, LV_STATE_DEFAULT, 3);
	lv_style_set_line_color(&gauge_style_major, LV_STATE_DEFAULT, color);
	lv_style_set_scale_end_color(&gauge_style_major, LV_STATE_DEFAULT, color);


	// Gague with seconds
	time_labels.second = lv_gauge_create(background, nullptr);
	lv_gauge_set_range(time_labels.second, 0, 60);
	lv_gauge_set_scale(time_labels.second, 360, 60, 2);
	lv_gauge_set_angle_offset(time_labels.second, 180);
	lv_obj_add_style(time_labels.second, LV_GAUGE_PART_MAIN, &gauge_style_main);
	lv_obj_add_style(time_labels.second, LV_GAUGE_PART_NEEDLE, &gauge_style_needle);
	lv_obj_add_style(time_labels.second, LV_GAUGE_PART_MAJOR, &gauge_style_major);

	lv_obj_set_size(time_labels.second, 170, 170);
	lv_obj_align(time_labels.second, NULL, LV_ALIGN_CENTER, 0, 0);

	//Numbers Hour
	time_labels.hour = lv_label_create(background, NULL);
	lv_obj_add_style(time_labels.hour, LV_OBJ_PART_MAIN, &style);
	lv_label_set_text(time_labels.hour, "00");
	lv_obj_align(time_labels.hour, NULL, LV_ALIGN_CENTER, 0, -25);


	// Numbers minute
	time_labels.minute = lv_label_create(background, NULL);
	lv_obj_add_style(time_labels.minute, LV_OBJ_PART_MAIN, &style);
	lv_label_set_text(time_labels.minute, "00");
	lv_obj_align(time_labels.minute, NULL, LV_ALIGN_CENTER, 0, 25);

	//If parent is a tileview set some stuff to be dragable via the tileview
	lv_obj_type_t parent_types;
	lv_obj_get_type(parent, &parent_types);
	if (parent_types.type[0] == "lv_tileview") {
		lv_tileview_add_element(parent, time_labels.second);
		lv_tileview_add_element(parent, time_labels.minute);
		lv_tileview_add_element(parent, time_labels.hour);
		lv_tileview_add_element(parent, background);
	}


//	time_labels.sec_digit = lv_label_create(background, NULL);
//	lv_obj_add_style(time_labels.sec_digit, LV_OBJ_PART_MAIN, &style);
//	lv_label_set_text(time_labels.sec_digit, "00");
//	lv_obj_align(time_labels.sec_digit, NULL, LV_ALIGN_CENTER, 0, 100);

	return clock_scr;
}

void clock_2_update(lv_task_t* t) {
	// Get time from the RTC directly
	RTC_Date curr_datetime = rtc->getDateTime();

	lv_gauge_set_value(time_labels.second, 0, curr_datetime.second);
	lv_gauge_set_angle_offset(time_labels.second, (180 + curr_datetime.second * (360 / 60)) % 360);
	lv_label_set_text_fmt(time_labels.hour, "%02u", curr_datetime.hour % 12);
	lv_obj_align(time_labels.hour, NULL, LV_ALIGN_CENTER, 0, -25);
	lv_label_set_text_fmt(time_labels.minute, "%02u", curr_datetime.minute);
	lv_obj_align(time_labels.minute, NULL, LV_ALIGN_CENTER, 0, 25);
//	lv_label_set_text_fmt(time_labels.sec_digit, "%02u", curr_datetime.second);
//	lv_obj_align(time_labels.sec_digit, NULL, LV_ALIGN_CENTER, 0, 100);

}