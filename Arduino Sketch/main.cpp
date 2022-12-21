#include <Arduino.h>

// Some font file I found in the examples - 7-segment LCD numbers
#include <D:\Documents\PlatformIO\Projects\ESP32_WORLD_CLOCK_MAX7219 YouTube\.pio\libdeps\esp32dev\MD_MAX72XX\examples\MD_MAX72xx_PushWheel\Font_Data.h>

// Local includes
#include "MAX7219Helper.h"
#include "defsHelper.h"

// Forward declaration
void animatedNumber();

// ------------------------------
void setup()
// ------------------------------
{
  Serial.begin(115200);
  log_v("Setup started");

  // Initialise MAX7219 display (declare max zones, but set single zone, initially)
  mx.begin(3);
  if (!mxSetSingleZone()) {
    log_e("Unable to set single zone");
  }

  // Intensity 0 -15, 1 is plenty bright enough for testing
  mx.setIntensity(1);

  // How to set "debugging" messages on the screen for the required length of time (2000)
  mx.displayText("Go!", PA_LEFT, 0, 2000, PA_PRINT, PA_NO_EFFECT);
  while (!mx.getZoneStatus(0)) { mx.displayAnimate(); }

  // Clear the specified (or all) zones
  //delay(1000);
  mx.displayClear();

  // Set up message (single zone, all LED modules) Zone 0 display
  mx.displayZoneText(0, "Just One Zone", PA_LEFT, 25, 1000, PA_SCROLL_LEFT, PA_SCROLL_RIGHT);
}

// ------------------------------
// Main loop
// ------------------------------
void loop()
{
  // We want to do this a number of times
  for (auto counter = 0; counter < 2; counter++) {
    while (1) {

      // Has this animation for ZONE 0 finished?
      if (mx.displayAnimate() && mx.getZoneStatus(0)) {

        // Reset the animation, do it again
        mx.displayReset(0);
        break;
      }
    }
  }

  // Change to 3 zones: [L]2 modules, [C]3 modules and [R]3 modules
  if (!mxSetZones()) {
    log_e("Unable to set 3 zones");
  }

  // Display something in each zone with different entry/exit animation
  mx.displayZoneText(0, "Zero", PA_CENTER, 20, 1000, PA_SCROLL_LEFT, PA_SCROLL_RIGHT);

  mx.displayZoneText(1, "One", PA_CENTER, 25, 2000, PA_SCROLL_UP, PA_SCROLL_DOWN);

  mx.displayZoneText(2, "Two", PA_CENTER, 15, 2500, PA_SCROLL_RIGHT, PA_SCROLL_LEFT);

  // Continue the animations until ALL zones are complete
  while (!mx.getZoneStatus(0) || !mx.getZoneStatus(1) || !mx.getZoneStatus(2)) {
    mx.displayAnimate();
  }

  // Clear the display and display a static number in centre zone that is dynamically updated
  mx.displayClear();
  animatedNumber();

  // Revert to single zone that spans all 8 modules  |---- 8 modules ----|
  if (!mxSetSingleZone()) {
    log_e("Unable to set single zone");
  } else {
    log_v("Single zone set");
  }

  mx.displayClear();
  log_v("Display cleared");

  // Display something that spans entire zone
  mx.displayZoneText(0, "- All Zero -", PA_CENTER, 25, 1000, PA_SCROLL_LEFT, PA_SCROLL_RIGHT);

  // Ensure no memory leaks
  showFreeMemory();
}

// ------------------------------
// Update a number (psuedo code)
// ------------------------------
void animatedNumber()
{
  // Create initallised buffer
  char buffer[6] = {'\0'};
  snprintf(buffer, 5, "%04d", 2436);

  // Change to non-proportional font
  mx.setFont(numeric7Seg);

  // Display the "number" in the centre zone, does not exit, stays displayed
  mx.displayZoneText(1, buffer, PA_CENTER, 25, 1000, PA_SCROLL_UP, PA_NO_EFFECT);
  while (1) {
    if (mx.displayAnimate() && mx.getZoneStatus(1))
      break;
  }

  // Increment the "number" in the centre zone for a while, just appears and no exit effect
  for (auto cnt = 2436; cnt < 2436 + 10; cnt++) {
    snprintf(buffer, 5, "%04d", cnt);
    mx.displayZoneText(1, buffer, PA_CENTER, 0, 500, PA_NO_EFFECT, PA_NO_EFFECT);
    while (1) {
      if (mx.displayAnimate() && mx.getZoneStatus(1))
        break;
    }
  }

  // Revert font to standard
  mx.setFont(nullptr);
}