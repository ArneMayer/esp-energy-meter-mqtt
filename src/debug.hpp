#pragma once
#include <core_esp8266_features.h>

#define DEBUG_PRINTS true

#if DEBUG_PRINTS == true

#define debug_print(s) Serial.print(s); Serial.flush();
#define debug_println(s) Serial.println(s); Serial.flush();

#else

#define debug_print(s) 
#define debug_println(s) 

#endif

void halt() {
    while(true) {
        delay(1000);
        yield();
    }
}