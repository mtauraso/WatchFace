#ifndef CLOCK_H
#define CLOCK_H

// for lv_obj_t and lv_task_t
#include <lvgl/lvgl.h>

lv_obj_t* clock_create(lv_obj_t * parent);
void clock_update(lv_task_t* t);

#endif
