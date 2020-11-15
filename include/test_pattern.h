#ifndef TEST_PATTERN_H
#define TEST_PATTERN_H

#include <lvgl/lvgl.h>

enum test_pattern {
    CONTRAST_BARS,
    BLACK_LEVEL_SQUARES,
    GAMMA_CHECKERBOARD,
    HSV_SWEEP
};

lv_obj_t* test_pattern_create(lv_obj_t* parent, test_pattern pattern);

lv_obj_t* contrast_bars_create(lv_obj_t* parent);
lv_obj_t* black_level_squares_create(lv_obj_t* parent); 
lv_obj_t* gamma_checkerboard_create(lv_obj_t* parent); 
lv_obj_t* hsv_sweep_create(lv_obj_t* parent);


lv_obj_t* color_val_test(lv_obj_t* parent, lv_color_t color);



#endif // background_h