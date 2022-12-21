#pragma once
#include "Arduino.h"
#include "MD_Parola.h"

// Check for memory leaks whilst debugging
void showFreeMemory()
{
  log_d("Free heap: %d", ESP.getFreeHeap());
}

// All possible animiations
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0])) ///< Generic macro for obtaining number of elements of an array