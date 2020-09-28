#include "background.h"

lv_obj_t* background_create(lv_obj_t* parent) {
	lv_obj_t* background = lv_img_create(parent, NULL);
	lv_img_set_src(background, &anime_girl);
	lv_obj_align(background, NULL, LV_ALIGN_CENTER, 0, 0);
	return background;
}