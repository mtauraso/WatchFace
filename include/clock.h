#ifndef CLOCK_H
#define CLOCK_H

// for lv_obj_t and lv_task_t
#include <lvgl/lvgl.h>

lv_obj_t* clockScr();
void update_clock(lv_task_t* t);

#endif
