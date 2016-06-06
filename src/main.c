#include <pebble.h>
#include "drawing.h"
#include "main.h"

static Window *s_main_window;
static TextLayer *s_time_layer;
static Layer *s_canvas_layer;
static Layer *pet_pebble_layer;

// Watchface started: set up graphics
static void main_window_load(Window *window) {
  //  Get info about the window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  // Create the TextLayer
  s_time_layer = text_layer_create(
    GRect(0, 10, bounds.size.w, 50));  
  
  // Improve the layout
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_font(s_time_layer, 
    fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  
  // Add background as a child layer to Window
  s_canvas_layer = layer_create(bounds);
  layer_set_update_proc(s_canvas_layer, canvas_update_proc);
  layer_add_child(window_layer, s_canvas_layer);
  
  // Add pet pebble as a child layer to Window
  pet_pebble_layer = layer_create(bounds);
  layer_set_update_proc(pet_pebble_layer, draw_pet_pebble);
  layer_add_child(window_layer, pet_pebble_layer);
}

// Teardown: remove all layers
static void main_window_unload(Window *window) {
  text_layer_destroy(s_time_layer);
  layer_destroy(pet_pebble_layer);
  layer_destroy(s_canvas_layer);
}

// Updates the time displayed
static void update_time() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                          "%H:%M" : "%I:%M", tick_time);
  text_layer_set_text(s_time_layer, s_buffer);
}

// Gets called on the minute
static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
  layer_mark_dirty(s_canvas_layer);  
  layer_mark_dirty(pet_pebble_layer);
}

// Initialize the watchface
static void init() {
  s_main_window = window_create();
  
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  window_set_background_color(s_main_window, GColorChromeYellow);
  window_stack_push(s_main_window, true);
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  update_time();
}

// Deinitialize the watchface
static void deinit() {
 window_destroy(s_main_window); 
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}