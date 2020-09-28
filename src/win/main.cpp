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
    lv_init();

    /*Initialize the HAL for LittlevGL*/
    hal_init();
    adjust_gamma();

    rtc = new PCF8563_Class();
    rtc->check();

    /*
     * Demos, benchmarks, and tests.
     *
     * Uncomment any one (and only one) of the functions below to run that
     * item.
     */

//	lv_scr_load(clock_2_create(NULL));
//	lv_task_create(clock_2_update, 50, LV_TASK_PRIO_MID, NULL);
    lv_scr_load(test_pattern_create(NULL));

    //lv_demo_widgets();
    //lv_demo_benchmark();
    //lv_demo_keypad_encoder();
    //lv_demo_printer();
    //lv_demo_stress();
    //lv_ex_get_started_1();
    //lv_ex_get_started_2();
    //lv_ex_get_started_3();

    //lv_ex_style_1();
    //lv_ex_style_2();
    //lv_ex_style_3();
    //lv_ex_style_4();
    //lv_ex_style_5();
    //lv_ex_style_6();
    //lv_ex_style_7();
    //lv_ex_style_8();
    //lv_ex_style_9();
    //lv_ex_style_10();
    //lv_ex_style_11();

    /*
     * There are many examples of individual widgets found under the
     * lv_examples/src/lv_ex_widgets directory.  Here are a few sample test
     * functions.  Look in that directory to find all the rest.
     */
    //lv_ex_arc_1();
    //lv_ex_cpicker_1();
    //lv_ex_gauge_1();
    //lv_ex_img_1();
    //lv_ex_tileview_1();

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


/**
* Initialize the Hardware Abstraction Layer (HAL) for the Littlev graphics library
*/
static void hal_init(void)
{
    /* Add a display
    * Use the 'monitor' driver which creates window on PC's monitor to simulate a display*/
    monitor_init();

    static lv_disp_buf_t disp_buf1;
    static lv_color_t buf1_1[LV_HOR_RES_MAX * LV_VER_RES_MAX];
    lv_disp_buf_init(&disp_buf1, buf1_1, NULL, LV_HOR_RES_MAX * LV_VER_RES_MAX);

    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);            /*Basic initialization*/
    disp_drv.buffer = &disp_buf1;
    disp_drv.flush_cb = monitor_flush;
    lv_disp_drv_register(&disp_drv);

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

    /* Tick init.
    * You have to call 'lv_tick_inc()' in every milliseconds
    * Create an SDL thread to do this*/
    SDL_CreateThread(tick_thread, "tick", NULL);
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
    float gammaB = 1.0;// 1.4;

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



/**
* A task to measure the elapsed time for LittlevGL
* @param data unused
* @return never return
*/
static int tick_thread(void *data)
{
    while (1) {
        lv_tick_inc(5);
        SDL_Delay(5);   /*Sleep for 1 millisecond*/
    }

    return 0;
}
