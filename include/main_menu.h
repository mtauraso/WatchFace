#ifndef MAIN_MENU_H
#define MAIN_MENU_H

// for lv_obj_t and lv_task_t
#include <lvgl/lvgl.h>

lv_obj_t* main_menu_create(lv_obj_t* parent);
void main_menu_update(lv_task_t* t);

#endif // MAIN_MENU_H