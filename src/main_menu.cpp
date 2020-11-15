#include "main_menu.h"
#include "clock.h"
#include "clock_2.h"
#include "test_pattern.h"
#include "watchglobals.h"


struct menu_item {
	lv_obj_t* (*item_func)(lv_obj_t*);
	lv_obj_t* (*screen_func)(lv_obj_t*);
};

static menu_item menu_items[] = {
	// XCXC clocks with backgrounds can't be part of this tileview.
	// Somehow the graphics are shoved up in a corner.
	{&clock_2_create, NULL},
	//{&clock_create, NULL},
	{&contrast_bars_create, NULL},
	{&black_level_squares_create, NULL},
	{&gamma_checkerboard_create, NULL},
	{&hsv_sweep_create, NULL},
};

static lv_point_t valid_pos[] = {
	{0,0},
	{0,1},
	{0,2},
	{0,3},
	{0,4}
};

lv_obj_t* main_menu_create(lv_obj_t* parent) {

	int num_items = sizeof(menu_items) / sizeof(menu_item);

	lv_obj_t* tileview = lv_tileview_create(parent, NULL);
	lv_tileview_set_edge_flash(tileview, true);
	lv_page_set_scrollbar_mode(tileview, LV_SCRLBAR_MODE_OFF);

	for (int i = 0; i < num_items; i++) {
		menu_item item_data = menu_items[i];
		lv_obj_t* item = item_data.item_func(tileview);
		lv_obj_set_pos(item, valid_pos[i].x * LV_HOR_RES, valid_pos[i].y * LV_VER_RES);
		lv_tileview_add_element(tileview, item);
	}

	lv_tileview_set_valid_positions(tileview, valid_pos, num_items);
	return tileview;
}

void main_menu_update(lv_task_t* t) {
	return;
}