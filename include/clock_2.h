#ifndef CLOCK_GUI_2_H
#define CLOCK_GUI_2_H

// for lv_obj_t and lv_task_t
#include <lvgl/lvgl.h>

lv_obj_t* clock_2_create(lv_obj_t* parent);
void clock_2_update(lv_task_t* t);

#endif // CLOCK_GUI_2_H
