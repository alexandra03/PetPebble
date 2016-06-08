#include <pebble.h>
#include "pet_pebble_state.h"
#include "drawing.h"

// Layer representing the pet pebble
void draw_pet_pebble(Layer *layer, GContext *ctx) {
  PetPebbleState pet_state = current_pet_state();
  
  GPoint BODY_CENTER = GPoint(90, 120);
  int EYES_HEIGHT = 115;
  int EYE_LEFT_CENTER = 80;
  int EYE_RIGHT_CENTER = 100;
  uint16_t radius = 30;
  
  // Draw the pet pebble body
  graphics_context_set_stroke_color(ctx, GColorCobaltBlue);
  graphics_context_set_fill_color(ctx, GColorCyan);
  graphics_context_set_stroke_width(ctx, 5);
  graphics_draw_circle(ctx, BODY_CENTER, radius);
  graphics_fill_circle(ctx, BODY_CENTER, radius);
  
  // Draw the pet pebble eyes
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_context_set_stroke_width(ctx, 2);
  radius = 2;
  
  if (pet_state == SLEEPING) {
    graphics_draw_line(ctx,
      GPoint(EYE_LEFT_CENTER-5, EYES_HEIGHT),
      GPoint(EYE_LEFT_CENTER+5, EYES_HEIGHT));
    graphics_draw_line(ctx, 
      GPoint(EYE_RIGHT_CENTER-5, EYES_HEIGHT), 
      GPoint(EYE_RIGHT_CENTER+5, EYES_HEIGHT));
  } else {
    graphics_draw_circle(
      ctx, GPoint(EYE_LEFT_CENTER, EYES_HEIGHT),radius);
    graphics_draw_circle(
      ctx, GPoint(EYE_RIGHT_CENTER, EYES_HEIGHT), radius);    
  }
  
  // Draw the pet pebble mouth
  if (pet_state == SLEEPING) {
    graphics_draw_circle(ctx, 
      GPoint((EYE_LEFT_CENTER+EYE_RIGHT_CENTER)/2, EYES_HEIGHT+13), 5);
    // Draw "Zzzz" to show that the pet pebble is asleep
    graphics_draw_text(ctx, "Zzzz", 
      fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21),
      GRect(EYE_RIGHT_CENTER+20, EYES_HEIGHT-40, EYE_RIGHT_CENTER+60, EYES_HEIGHT-20),
      GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, NULL);
  } else {
    GRect rect_bounds = GRect(EYE_LEFT_CENTER, 
      EYES_HEIGHT+10, EYE_RIGHT_CENTER-EYE_LEFT_CENTER, 5);    
    int32_t angle_start = DEG_TO_TRIGANGLE(85);
    int32_t angle_end = DEG_TO_TRIGANGLE(275);
    graphics_draw_arc(ctx, rect_bounds,
      GOvalScaleModeFillCircle, angle_start, angle_end);
  }
}

// Layer representing the background
void canvas_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_stroke_color(ctx, GColorMidnightGreen);
  graphics_context_set_fill_color(ctx, GColorDarkGreen);  
  GRect rect_bounds = GRect(0, 120, 180, 180);
  graphics_draw_rect(ctx, rect_bounds);
  graphics_fill_rect(ctx, rect_bounds, 10, GCornersAll);
  APP_LOG(APP_LOG_LEVEL_INFO, conditions_buffer);
  
  Weather weather = current_weather();
  
  if (weather == CLOUDY) {

  }
}
