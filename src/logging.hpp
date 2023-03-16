#pragma once

#define DEBUG_PRINTS false

#if DEBUG_PRINTS == true

#define debug_print(s) Serial.print(s)
#define debug_println(s) Serial.println(s)

#else

#define debug_print(s) 
#define debug_println(s) 

#endif