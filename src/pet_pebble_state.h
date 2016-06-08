#pragma once

// Stores weather info sent from app.js
extern char temperature_buffer[8];
extern char conditions_buffer[32];

typedef enum {SLEEPING, EATING, FITNESS, WORKING, REST} PetPebbleState;

typedef enum {CLOUDY, SUNNY, RAINY, MISTY} Weather;

PetPebbleState current_pet_state();

Weather current_weather();