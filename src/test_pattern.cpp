#include "test_pattern.h"
#include "watchglobals.h"

lv_obj_t* test_pattern_create(lv_obj_t* parent) {

	// Canvas data
	const int HEIGHT = 240;
	const int WIDTH = 240;
	const int bufsize = sizeof(lv_color_t) * WIDTH * HEIGHT;
	static lv_color_t * buf = (lv_color_t *) ps_malloc(bufsize);

	lv_obj_t * canvas = lv_canvas_create(parent, nullptr);

	lv_canvas_set_buffer(canvas, buf, WIDTH, HEIGHT, LV_IMG_CF_TRUE_COLOR);

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