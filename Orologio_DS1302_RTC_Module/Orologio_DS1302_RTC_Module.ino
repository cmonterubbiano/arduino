// Example sketch for interfacing with the DS1302 timekeeping chip.
//
// Copyright (c) 2009, Matt Sparks
// All rights reserved.
//
// http://quadpoint.org/projects/arduino-ds1302
#include <stdio.h>
#include <DS1302.h>

namespace {

// Set the appropriate digital I/O pin connections. These are the pin
// assignments for the Arduino as well for as the DS1302 chip. See the DS1302
// datasheet:
//
//   http://datasheets.maximintegrated.com/en/ds/DS1302.pdf
const int kCePin   = 5;  // Chip Enable CLK
const int kIoPin   = 6;  // Input/Output DA
const int kSclkPin = 7;  // Serial Clock RST

// Create a DS1302 object.
DS1302 rtc(kCePin, kIoPin, kSclkPin);

String dayAsString(const Time::Day day) {
  switch (day) {
    case Time::kSunday: return "Domenica";
    case Time::kMonday: return "Lunedi";
    case Time::kTuesday: return "Martedi";
    case Time::kWednesday: return "Mercoledi";
    case Time::kThursday: return "Giovedi";
    case Time::kFriday: return "Venerdi";
    case Time::kSaturday: return "Sabato";
  }
  return "(unknown day)";
}

void printTime() {
  // Get the current time and date from the chip.
  Time t = rtc.time();

  // Name the day of the week.
  const String day = dayAsString(t.day);

  // Format the time and date and insert into the temporary buffer.
  char buf[50];
  snprintf(buf, sizeof(buf), "%s %02d-%02d-%04d %02d:%02d:%02d",
           day.c_str(),
           t.date, t.mon, t.yr,
           t.hr, t.min, t.sec);

  // Print the formatted string to serial so we can see the time.
  Serial.println(buf);
}

}  // namespace

void setup() {
  Serial.begin(9600);

  // Initialize a new chip by turning off write protection and clearing the
  // clock halt flag. These methods needn't always be called. See the DS1302
  // datasheet for details.
  rtc.writeProtect(false);
  rtc.halt(false);
  // Dovrebbe esser fatto solo la prima volta poi ha la batteria
  // Make a new time object to set the date and time.
  // Sunday, September 22, 2013 at 01:38:50.
 // Time t(2013, 09, 22, 01, 38, 50, Time::kSunday);

  // Set the time and date on the chip.
 // rtc.time(t);
}

// Loop and print the time every second.
void loop() {
  printTime();
  delay(1000);
}
