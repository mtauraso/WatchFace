/**
* @file main
*
*/

/*********************
*      INCLUDES
*********************/
#include <stdlib.h>
#include <Windows.h>
#include <SDL.h>
#include "lvgl/lvgl.h"
#include "lv_drivers/display/monitor.h"
#include "lv_drivers/indev/mouse.h"
#include "lv_drivers/indev/keyboard.h"
#include "lv_examples/lv_examples.h"

#include "watchglobals.h"
#include "clock.h"
#include "clock_2.h"
#include "test_pattern.h"
#include "main_menu.h"

/*********************
*      DEFINES
*********************/

/**********************
*      TYPEDEFS
**********************/

/**********************
*  STATIC PROTOTYPES
**********************/
static void hal_init(void);
static int tick_thread(void *data);
static void adjust_gamma(void);

/**********************
*  STATIC VARIABLES
**********************/
static lv_indev_t * kb_indev;

static lv_disp_t* main_disp;
static lv_disp_buf_t disp_buf1;
static lv_color_t buf1_1[LV_HOR_RES_MAX * LV_VER_RES_MAX];

PCF8563_Class * rtc;

/**********************
*      MACROS
**********************/

/**********************
*   GLOBAL FUNCTIONS
**********************/


int main(int argc, char** argv)
{
    /*Initialize LittlevGL*/
    lv_set_monitor_zoom(2);
    lv_init();

    /*Initialize the HAL for LittlevGL*/
    hal_init();
    //adjust_gamma();

    rtc = new PCF8563_Class();
    rtc->check();

    /*
     * Demos, benchmarks, and tests.
     *
     * Uncomment any one (and only one) of the functions below to run that
     * item.
     */
    //lv_disp_set_default(debug_disp);
    lv_scr_load(main_menu_create(NULL));
    //lv_scr_load(clock_2_create(NULL));
    lv_task_create(clock_2_update, 50, LV_TASK_PRIO_MID, NULL);

    //lv_scr_load(test_pattern_create(NULL, GAMMA_CHECKERBOARD));
    //lv_scr_load(test_pattern_create(NULL, CONTRAST_BARS));
    //lv_scr_load(color_val_test(nullptr, lv_color_make(31 << 3, 0, 31 << 3)));
    //lv_scr_load(test_pattern_create(NULL, BLACK_LEVEL_SQUARES));


    while (1) {
        /* Periodically call the lv_task handler.
        * It could be done in a timer interrupt or an OS task too.*/
        lv_task_handler();
        Sleep(10);       /*Just to let the system breathe */
    }

    return 0;
}

/**********************
*   STATIC FUNCTIONS
**********************/

static void hal_input_init(lv_disp_t* disp) {
    lv_disp_t* old_default;
    old_default = lv_disp_get_default();
    lv_disp_set_default(disp);

    /* Add the mouse (or touchpad) as input device
    * Use the 'mouse' driver which reads the PC's mouse*/
    mouse_init();
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);          /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = mouse_read;         /*This function will be called periodically (by the library) to get the mouse position and state*/
    lv_indev_drv_register(&indev_drv);

    /* If the PC keyboard driver is enabled in`lv_drv_conf.h`
    * add this as an input device. It might be used in some examples. */
#if USE_KEYBOARD
    lv_indev_drv_t kb_drv;
    lv_indev_drv_init(&kb_drv);
    kb_drv.type = LV_INDEV_TYPE_KEYPAD;
    kb_drv.read_cb = keyboard_read;
    kb_indev = lv_indev_drv_register(&kb_drv);
#endif

    lv_disp_set_default(old_default);
}

/**
* Initialize the Hardware Abstraction Layer (HAL) for the Littlev graphics library
*/
static void hal_init(void)
{
    /* Add a display
    * Use the 'monitor' driver which creates window on PC's monitor to simulate a display*/
    monitor_init();

    // Initialize Main display
    lv_disp_buf_init(&disp_buf1, buf1_1, NULL, LV_HOR_RES_MAX * LV_VER_RES_MAX);

    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);            /*Basic initialization*/
    disp_drv.buffer = &disp_buf1;
    disp_drv.flush_cb = monitor_flush;
    main_disp = lv_disp_drv_register(&disp_drv);
    lv_disp_set_default(main_disp);

    hal_input_init(main_disp);

    /* Tick init.
    * You have to call 'lv_tick_inc()' in every milliseconds
    * Create an SDL thread to do this*/
    SDL_CreateThread(tick_thread, "tick", NULL);
}


/**
* A task to measure the elapsed time for LittlevGL
* @param data unused
* @return never return
*/
static int tick_thread(void* data)
{
    while (1) {
        lv_tick_inc(5);
        SDL_Delay(5);   /*Sleep for 1 millisecond*/
    }

    return 0;
}

//Haxonhax
// Mess with the gamma
// Reach inside lv_driver's internals...

// Forward declare this extern I want from lv_drivers/display/monitor.c
extern "C" {
    typedef struct {
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Texture* texture;
        volatile bool sdl_refr_qry;
#if MONITOR_DOUBLE_BUFFERED
        uint32_t* tft_fb_act;
#else
        uint32_t tft_fb[LV_HOR_RES_MAX * LV_VER_RES_MAX];
#endif
    } monitor_t;

    extern monitor_t monitor;
}

static void clamp_ramp(uint8_t min, uint8_t max, Uint16* ramp) {
    //input value of 0->min maps to min
    for (int i = 0; i < min; i++) {
        ramp[i] = min*256;
    };

    //input value of max->255 maps to max
    for (int i = max; i < 256; i++) {
        ramp[i] = max*256;
    }
}

static void adjust_gamma(void) {
    float gammaR = 1.0;// 0.7;
    float gammaG = 1.0;// 1.1;
    float gammaB = 1.0;// 1.4;s

    float scale = 1.0;

    static Uint16 gammaRampR[256];
    static Uint16 gammaRampG[256];
    static Uint16 gammaRampB[256];

    SDL_CalculateGammaRamp(gammaR*scale, gammaRampR);
    SDL_CalculateGammaRamp(gammaG*scale, gammaRampG);
    SDL_CalculateGammaRamp(gammaB*scale, gammaRampB);

    //clamp_ramp(0, 100, gammaRampR);
    //clamp_ramp(20, 255, gammaRampB);

    SDL_SetWindowGammaRamp(monitor.window, gammaRampR, gammaRampG, gammaRampB);
}




