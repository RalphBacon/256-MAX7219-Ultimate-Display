//   Wire DIN (data) to the MOSI pin 23
//   Wire CLK (clock) to the SCK pin 18
//   Wire LOAD to the /SS (slave select) pin 5
//   Wire nothing to MOSI pin 19

// Libraries required
#pragma once
#include "Arduino.h"
#include "defsHelper.h"
#include <MD_MAX72xx.h>
#include <MD_Parola.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES   8

// Pins for an ESP32 (you can have others)
#define CLK_PIN       18 // SCK
#define DATA_PIN      23 // MOSI
#define CS_PIN        5  // CS

// Arbitrary output pins
MD_Parola mx = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

// Emulate a single zone by allow Zone 0 to have all the modules. Important we don't use Zone 1 or 2.
bool mxSetSingleZone()
{
  mx.setZone(2, 0, 0);
  mx.setZone(1, 0, 0);
  return mx.setZone(0, 0, 7);
}

// Set up the three zones for the time(s)
bool mxSetZones()
{
  // Zone set up, counting devices from the RIGHT!!!
  /*
    ZONE 1-|-------- ZONE 0 -------|
        ___________________________
   and   4 |  3  |  2  |  1  |  0  |
   so on __|_____|_____|_____|_____|

  */
  if (!mx.setZone(0, 0, 2)) {
    log_e("Unable to set zone 0 (0-2)");
    return false;
  }

  if (!mx.setZone(1, 3, 5)) {
    log_e("Unable to set zone 1 (3-5)");
    return false;
  }

  if (!mx.setZone(2, 6, 7)) {
    log_e("Unable to set zone 2 (6-7)");
    return false;
  }

  // Set brightness of each zone
  mx.setIntensity(0, 1);
  mx.setIntensity(1, 1);
  mx.setIntensity(2, 1);

  // All done
  return true;
}

#include <vector>
static uint8_t msgNo = 0;
std::vector<std::string> msgs = {
    "Car Service due NEXT WEEK book now",
    "Tesco/Lidl Food shopping CMAS 2022",
    "Christmas Fayre Lincolnshire next year",
    "Uni Hospital Checkup 15:50 Wednesday",
};
