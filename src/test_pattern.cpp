#include "test_pattern.h"
#include "watchglobals.h"

void draw_bar(lv_obj_t* canvas, int bar_height, int bar_offset, bool r, bool g, bool b) {
	const int bar_gutter_left = 11;
	const int num_sections = 31; //( [1..1F] for all non-zero 5 bit color vals)
	const int section_width = 7;
	const int bar_width = num_sections * section_width;  // This is 217. This + bar_gutter_left < WIDTH
	// for each pixel of the bar
	for (int i = 0; i < bar_width; i++) {
		// What section are we in?
		int section_num = (i / section_width) + 1;
		for (int j = bar_offset; j < bar_height + bar_offset; j++) {
			uint8_t red = r ? section_num << 3: 0x0;
			uint8_t green = g ? section_num << 3: 0x0;
			uint8_t blue = b ? section_num << 3: 0x0;
			lv_canvas_set_px(canvas, i + bar_gutter_left, j, lv_color_make(red, green, blue));
		}
	}
}

void draw_square(lv_obj_t* canvas, int x, int y, int size, lv_color_t color) {

	for (int i = x; i < x + size; i++) {
		for (int j = y; j < y + size; j++) {
			lv_canvas_set_px(canvas, i, j, color);
		}
	}
	
}

void draw_checkboard(lv_obj_t* canvas, int x, int y, int width, int height, lv_color_t color, lv_color_t color2) {
	for (int i = x; i < x + width; i++) {
		for (int j = y; j < y + height; j++) {
			if (j % 2 == (( i % 2 ) + 1) % 2) {
			//if (j % 2 == 0) {
				lv_canvas_set_px(canvas, i, j, color);
			} else {
				lv_canvas_set_px(canvas, i, j, color2);
			}
		}
	}
}

lv_obj_t* contrast_bars_create(lv_obj_t* parent) {
	// Canvas data
	const int HEIGHT = 240;
	const int WIDTH = 240;
	const int bufsize = sizeof(lv_color_t) * WIDTH * HEIGHT;
	static lv_color_t* buf = (lv_color_t*)ps_malloc(bufsize);

	lv_obj_t* canvas = lv_canvas_create(parent, nullptr);

	lv_canvas_set_buffer(canvas, buf, WIDTH, HEIGHT, LV_IMG_CF_TRUE_COLOR);
	lv_canvas_fill_bg(canvas, lv_color_make(0, 0, 0), LV_OPA_COVER);

	// Contrast bars
	const int bar_height = 16;
	const int bar_sep = 16;
	draw_bar(canvas, bar_height, bar_sep, false, false, true);
	draw_bar(canvas, bar_height, bar_sep * 2 + bar_height, false, true, false);
	draw_bar(canvas, bar_height, bar_sep * 3 + bar_height * 2, false, true, true);
	draw_bar(canvas, bar_height, bar_sep * 4 + bar_height * 3, true, false, false);
	draw_bar(canvas, bar_height, bar_sep * 5 + bar_height * 4, true, false, true);
	draw_bar(canvas, bar_height, bar_sep * 6 + bar_height * 5, true, true, false);
	draw_bar(canvas, bar_height, bar_sep * 7 + bar_height * 6, true, true, true);
	
	return canvas;
}

lv_obj_t* black_level_squares_create(lv_obj_t* parent) {
	// Canvas data
	const int HEIGHT = 240;
	const int WIDTH = 240;
	const int bufsize = sizeof(lv_color_t) * WIDTH * HEIGHT;
	static lv_color_t* buf = (lv_color_t*)ps_malloc(bufsize);

	lv_obj_t* canvas = lv_canvas_create(parent, nullptr);

	lv_canvas_set_buffer(canvas, buf, WIDTH, HEIGHT, LV_IMG_CF_TRUE_COLOR);
	lv_canvas_fill_bg(canvas, lv_color_make(0, 0, 0), LV_OPA_COVER);

	// Black level squares
	const int square_sep = 8;
	const int square_size = 32;
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			int square_num = j + 6 * i;
			lv_color_t color = lv_color_make((square_num + 1) * 8, (square_num + 1) * 8, (square_num + 1) * 8);
			draw_square(canvas,
				square_sep * j + square_sep / 2 + square_size * j,
				square_sep * i + square_sep / 2 + square_size * i,
				square_size, color);
		}
	}
	return canvas;
}

lv_obj_t* gamma_checkerboard_create(lv_obj_t* parent) {
	// Canvas data
	const int HEIGHT = 240;
	const int WIDTH = 240;
	const int bufsize = sizeof(lv_color_t) * WIDTH * HEIGHT;
	static lv_color_t* buf = (lv_color_t*)ps_malloc(bufsize);

	lv_obj_t* canvas = lv_canvas_create(parent, nullptr);

	lv_canvas_set_buffer(canvas, buf, WIDTH, HEIGHT, LV_IMG_CF_TRUE_COLOR);
	lv_canvas_fill_bg(canvas, lv_color_make(0, 0, 0), LV_OPA_COVER);

	for (int i = 0; i < 3; i++) {
		int color_val_full = 253; // one off of full brightness on my test pattern..?
		int color_val_26 = 195; // Should equal brightness to 50% checkerboard at gamma of 2.6
		int color_val_22 = 186; // Should equal brightness to 50% checkerboard at gamma of 2.2
		int color_val_18 = 173; // Should equal brightness to 50% checkerboard at gamma of 1.8
		lv_color_t color_full = lv_color_make(color_val_full * (i == 0),
			color_val_full * (i == 1),
			color_val_full * (i == 2));
		lv_color_t color_26 = lv_color_make(color_val_26 * (i == 0),
			color_val_26 * (i == 1),
			color_val_26 * (i == 2));
		lv_color_t color_22 = lv_color_make(color_val_22 * (i == 0),
			color_val_22 * (i == 1),
			color_val_22 * (i == 2));
		lv_color_t color_18 = lv_color_make(color_val_18 * (i == 0),
			color_val_18 * (i == 1),
			color_val_18 * (i == 2));
		draw_checkboard(canvas, i * 80, 0, 80, 240, color_full, lv_color_make(0, 0, 0));
		draw_square(canvas, i * 80 + 20, 30, 20, color_26);
		draw_square(canvas, i * 80 + 40, 30, 20, color_26);
		draw_square(canvas, i * 80 + 20, 80 + 30, 20, color_22);
		draw_square(canvas, i * 80 + 40, 80 + 30, 20, color_22);
		draw_square(canvas, i * 80 + 20, 160 + 30, 20, color_18);
		draw_square(canvas, i * 80 + 40, 160 + 30, 20, color_18);
	}

	return canvas;
}

lv_obj_t* hsv_sweep_create(lv_obj_t* parent) {
	// Canvas data
	const int HEIGHT = 240;
	const int WIDTH = 240;
	const int bufsize = sizeof(lv_color_t) * WIDTH * HEIGHT;
	static lv_color_t* buf = (lv_color_t*)ps_malloc(bufsize);

	lv_obj_t* canvas = lv_canvas_create(parent, nullptr);

	lv_canvas_set_buffer(canvas, buf, WIDTH, HEIGHT, LV_IMG_CF_TRUE_COLOR);
	lv_canvas_fill_bg(canvas, lv_color_make(0, 0, 0), LV_OPA_COVER);

	// HSV traversal
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			float i_clamp = float(i) / (WIDTH - 1);
			float j_clamp = float(j) / (HEIGHT - 1);

			uint16_t hue = i_clamp * 359; // [0..359]
			uint8_t saturation = 100; // [0..100]
			uint8_t value = j_clamp * 100; // [0..100]

			lv_canvas_set_px(canvas, i, j, lv_color_hsv_to_rgb(hue, saturation, value));
		}
	}

	return canvas;
}

lv_obj_t* test_pattern_create(lv_obj_t* parent, test_pattern pattern) {
	switch (pattern) {
		case CONTRAST_BARS:
			return contrast_bars_create(parent);
		case BLACK_LEVEL_SQUARES:
			return black_level_squares_create(parent);
		case GAMMA_CHECKERBOARD:
			return gamma_checkerboard_create(parent);
		case HSV_SWEEP:
			return hsv_sweep_create(parent);
	}
}

lv_obj_t* color_val_test(lv_obj_t* parent, lv_color_t color)
{
	// Canvas data
	const int HEIGHT = 240;
	const int WIDTH = 240;
	const int bufsize = sizeof(lv_color_t) * WIDTH * HEIGHT;
	static lv_color_t * buf = (lv_color_t *) ps_malloc(bufsize);

	lv_obj_t * canvas = lv_canvas_create(parent, nullptr);

	lv_canvas_set_buffer(canvas, buf, WIDTH, HEIGHT, LV_IMG_CF_TRUE_COLOR);
	lv_canvas_fill_bg(canvas, lv_color_make(0, 0, 0), LV_OPA_COVER);
	
	draw_square(canvas, 100,100,50, color);

	static lv_style_t label_style;

	lv_style_init(&label_style);
	lv_style_set_text_color(&label_style, LV_STATE_DEFAULT, lv_color_make(255,255,255));

	lv_obj_t * color_label_b = lv_label_create(canvas, nullptr);
	lv_obj_add_style(color_label_b, LV_LABEL_PART_MAIN, &label_style);
#ifdef WATCH_SIM
	lv_label_set_text_fmt(color_label_b, "R:%b G:%b B:%b", color.ch.red, color.ch.green, color.ch.blue);
#else
	lv_label_set_text_fmt(color_label_b, "R:%b G:%b B:%b", color.ch.red, (color.ch.green_h<<3) | color.ch.green_l, color.ch.blue);
#endif


	lv_obj_align(color_label_b, NULL, LV_ALIGN_CENTER, 0, 50);

	lv_obj_t * color_label_d = lv_label_create(canvas, nullptr);
	lv_obj_add_style(color_label_d, LV_LABEL_PART_MAIN, &label_style);
#ifdef WATCH_SIM
	lv_label_set_text_fmt(color_label_d, "R:%d G:%d B:%d", color.ch.red, color.ch.green, color.ch.blue);
#else
	lv_label_set_text_fmt(color_label_d, "R:%d G:%d B:%d", color.ch.red, (color.ch.green_h << 3) | color.ch.green_l, color.ch.blue);
#endif
	lv_obj_align(color_label_d, NULL, LV_ALIGN_CENTER, 0, 80);

	lv_obj_t * color_label_h = lv_label_create(canvas, nullptr);
	lv_obj_add_style(color_label_h, LV_LABEL_PART_MAIN, &label_style);
#ifdef WATCH_SIM
	lv_label_set_text_fmt(color_label_h, "R:%x G:%x B:%x", color.ch.red, color.ch.green, color.ch.blue);
#else
	lv_label_set_text_fmt(color_label_h, "R:%x G:%x B:%x", color.ch.red, (color.ch.green_h << 3) | color.ch.green_l, color.ch.blue);
#endif
	lv_obj_align(color_label_h, NULL, LV_ALIGN_CENTER, 0, 110);

	return canvas;
}