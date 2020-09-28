#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <lvgl/lvgl.h>

extern "C" {
	LV_IMG_DECLARE(anime_girl);
}

lv_obj_t* background_create(lv_obj_t* parent);

#endif // background_h