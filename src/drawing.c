#include <pebble.h>
#include "drawing.h"

// Layer representing the pet pebble
void draw_pet_pebble(Layer *layer, GContext *ctx) {
  // Draw the pet pebble body
  graphics_context_set_stroke_color(ctx, GColorCobaltBlue);
  graphics_context_set_fill_color(ctx, GColorCyan);
  graphics_context_set_stroke_width(ctx, 5);
  GPoint center = GPoint(90, 120);
  uint16_t radius = 30;
  graphics_draw_circle(ctx, center, radius);
  graphics_fill_circle(ctx, center, radius);
  
  // Draw the pet pebble eyes
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_context_set_stroke_width(ctx, 2);
  center = GPoint(80, 115);
  radius = 2;
  graphics_draw_circle(ctx, center, radius);
  center = GPoint(100, 115);
  graphics_draw_circle(ctx, center, radius);
  
  // Draw the pet pebble mouth
  GRect rect_bounds = GRect(80, 125, 20, 5);  
  int32_t angle_start = DEG_TO_TRIGANGLE(85);
  int32_t angle_end = DEG_TO_TRIGANGLE(275);
  graphics_draw_arc(ctx, rect_bounds, 
    GOvalScaleModeFillCircle, angle_start, angle_end);
}

// Layer representing the background
void canvas_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_stroke_color(ctx, GColorMidnightGreen);
  graphics_context_set_fill_color(ctx, GColorDarkGreen);  
  GRect rect_bounds = GRect(0, 120, 180, 180);
  graphics_draw_rect(ctx, rect_bounds);
  graphics_fill_rect(ctx, rect_bounds, 10, GCornersAll);  
}