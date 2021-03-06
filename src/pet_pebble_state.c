#include <pebble.h>
#include "pet_pebble_state.h"

PetPebbleState current_pet_state() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  int hour = tick_time->tm_hour;
  
  if (hour >= 21 || hour < 7) {
    return SLEEPING;
  } else if (hour == 12 || hour == 19) {
    return EATING;
  }
  
  return REST; 
}

Weather current_weather() {
  if (strcmp(conditions_buffer, "Clouds") == 0) {
    return CLOUDY;
  } else if (strcmp(conditions_buffer, "Misty") == 0) {
    return MISTY;
  } else if (strcmp(conditions_buffer, "Sun") == 0) {
    return SUNNY;
  } else {
    return SUNNY;
  }
}