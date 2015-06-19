#include <pebble.h>
#include "mainwindow.h"
#include "../constantes.h"
#include "configwindow.h"
  
int m = 0;
int s = 0;
int mode = MODE_PAUSED;

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_bitham_42_medium_numbers;
static GBitmap *s_res_cog_btn;
static GBitmap *s_res_play_btn;
static GBitmap *s_res_reload_btn;
static GFont s_res_roboto_condensed_21;
static TextLayer *timer_layer;
static ActionBarLayer *s_actionbarlayer_1;
static TextLayer *counter_layer;

static void initialise_ui(void) {
  s_window = window_create();
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, true);
  #endif
  
  s_res_bitham_42_medium_numbers = fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS);
  s_res_cog_btn = gbitmap_create_with_resource(RESOURCE_ID_COG_BTN);
  s_res_play_btn = gbitmap_create_with_resource(RESOURCE_ID_PLAY_BTN);
  s_res_reload_btn = gbitmap_create_with_resource(RESOURCE_ID_RELOAD_BTN);
  s_res_roboto_condensed_21 = fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21);
  // timer_layer
  timer_layer = text_layer_create(GRect(-3, 46, 117, 61));
  text_layer_set_background_color(timer_layer, GColorClear);
  text_layer_set_text(timer_layer, "25:00");
  text_layer_set_text_alignment(timer_layer, GTextAlignmentCenter);
  text_layer_set_font(timer_layer, s_res_bitham_42_medium_numbers);
  layer_add_child(window_get_root_layer(s_window), (Layer *)timer_layer);
  
  // s_actionbarlayer_1
  s_actionbarlayer_1 = action_bar_layer_create();
  action_bar_layer_add_to_window(s_actionbarlayer_1, s_window);
  action_bar_layer_set_background_color(s_actionbarlayer_1, GColorWhite);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_UP, s_res_cog_btn);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_play_btn);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_DOWN, s_res_reload_btn);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_actionbarlayer_1);
  
  // counter_layer
  counter_layer = text_layer_create(GRect(2, 92, 118, 24));
  text_layer_set_background_color(counter_layer, GColorClear);
  text_layer_set_text(counter_layer, "Done");
  text_layer_set_text_alignment(counter_layer, GTextAlignmentCenter);
  text_layer_set_font(counter_layer, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)counter_layer);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(timer_layer);
  action_bar_layer_destroy(s_actionbarlayer_1);
  text_layer_destroy(counter_layer);
  gbitmap_destroy(s_res_cog_btn);
  gbitmap_destroy(s_res_play_btn);
  gbitmap_destroy(s_res_reload_btn);
}
// END AUTO-GENERATED UI CODE

static void configRunningUI(){
  window_set_background_color(s_window, GColorDarkCandyAppleRed);
  text_layer_set_text_color(timer_layer, GColorWhite);
  text_layer_set_text_color(counter_layer, GColorWhite);
}

static void configPauseUI(){
  window_set_background_color(s_window, GColorJaegerGreen);
  text_layer_set_text_color(timer_layer, GColorWhite);
  text_layer_set_text_color(counter_layer, GColorWhite);
}

static void configDefaultUI(){
  window_set_background_color(s_window, GColorCobaltBlue);
  text_layer_set_text_color(timer_layer, GColorWhite);
  text_layer_set_text_color(counter_layer, GColorWhite);
}

static void updateTimer(struct tm *tick_time, TimeUnits units_changed){
  if(s == 0 && m > 0){
    s = 59;
    m--;
  }else if(s == 0 && m == 0){
    if(mode == MODE_RUNNING_WORK){
      m = persist_read_int(CONFIG_PAUSE_TIME);
      configPauseUI();
      mode = MODE_RUNNING_PAUSE;
    }else{
      mode = MODE_RUNNING_WORK;
      m = persist_read_int(CONFIG_WORK_TIME);
      configRunningUI();
    }
    s = 0;
    
  }else{
    s--;
  }
  
  static char text[] = "";
  snprintf(text, 7, "%.2d:%.2d", m,s);
  text_layer_set_text(timer_layer, text);
  
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(mode == MODE_RUNNING_WORK || mode == MODE_RUNNING_PAUSE){
    configDefaultUI();
    tick_timer_service_unsubscribe();
    mode = MODE_PAUSED;
    static char text[] = "";
    int min = persist_read_int(CONFIG_WORK_TIME);
    snprintf(text, 7, "%d:00", min);
    text_layer_set_text(timer_layer, text);
  }else{
    m = persist_read_int(CONFIG_WORK_TIME);
    s = 0;
    mode = MODE_RUNNING_WORK;
    configRunningUI();
    tick_timer_service_subscribe(SECOND_UNIT, updateTimer);  
  }
  
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  show_configwindow();
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void handle_window_unload(Window* window) {
  destroy_ui();
}

static void handle_window_appear(Window* window){
  static char text[] = "";
  static char cycleText[] = "";
  int min = persist_read_int(CONFIG_WORK_TIME);
  int cycle = persist_read_int(CONFIG_P_COUNTER);
  snprintf(text, 8, "%.2d:00", min);
  snprintf(cycleText, 10, "Done: %d", cycle);
  text_layer_set_text(timer_layer, text);
  text_layer_set_text(counter_layer, cycleText);
}

void show_mainwindow(void) {
  initialise_ui();
  window_set_click_config_provider(s_window, click_config_provider);
  configDefaultUI();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
    .appear = handle_window_appear,
  });
  window_stack_push(s_window, true);
}

void hide_mainwindow(void) {
  window_stack_remove(s_window, true);
}
