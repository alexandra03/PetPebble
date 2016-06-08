#include <pebble.h>
#include "drawing.h"
#include "main.h"

#define KEY_TEMPERATURE 0
#define KEY_CONDITIONS 1

static Window *s_main_window;
static TextLayer *s_time_layer;
static Layer *s_canvas_layer;
static Layer *pet_pebble_layer;

char temperature_buffer[8];
char conditions_buffer[32];

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

// Get weather data, send to C files
static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Read tuples for data
  Tuple *temp_tuple = dict_find(iterator, KEY_TEMPERATURE);
  Tuple *conditions_tuple = dict_find(iterator, KEY_CONDITIONS);

  // If all data is available, use it
  if(temp_tuple && conditions_tuple) {
    snprintf(temperature_buffer, sizeof(temperature_buffer), "%dC", (int)temp_tuple->value->int32);
    snprintf(conditions_buffer, sizeof(conditions_buffer), "%s", conditions_tuple->value->cstring);
  }
  
  layer_mark_dirty(s_canvas_layer);
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
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
  
  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  // Open AppMessage
  const int inbox_size = 128;
  const int outbox_size = 128;
  app_message_open(inbox_size, outbox_size);  
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