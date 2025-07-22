#include "./monitor.h"
#include <assert.h>
#include <thread>
#include <chrono>
#include <iostream>

using std::cout, std::endl;
using std::flush;
using std::this_thread::sleep_for;
using std::chrono::seconds;

// Vital thresholds
constexpr float TEMP_LOW = 95.0f;
constexpr float TEMP_HIGH = 102.0f;
constexpr float PULSE_LOW = 60.0f;
constexpr float PULSE_HIGH = 100.0f;
constexpr float SPO2_MIN = 90.0f;

constexpr int BLINK_COUNT = 6;
constexpr int BLINK_INTERVAL_SEC = 1;

// Blinking warning indicator
void blinkWarning() {
    for (int i = 0; i < BLINK_COUNT; ++i) {
        cout << "\r* " << flush;
        sleep_for(seconds(BLINK_INTERVAL_SEC));
        cout << "\r *" << flush;
        sleep_for(seconds(BLINK_INTERVAL_SEC));
    }
    cout << endl;
}

// Alert printer + blinker
bool alertAndBlink(const char* message) {
    cout << message << endl;
    blinkWarning();
    return false;
}

// Generic range checker for vitals
bool isOutOfRange(float value, float min, float max) {
    return value < min || value > max;
}

// SPO2 is only lower-bounded
bool isBelowThreshold(float value, float min) {
    return value < min;
}

// Main vitals check
bool vitalsOk(float temperature, float pulseRate, float spo2) {
    if (isOutOfRange(temperature, TEMP_LOW, TEMP_HIGH)) {
        return alertAndBlink("Temperature is out of range!");
    }

    if (isOutOfRange(pulseRate, PULSE_LOW, PULSE_HIGH)) {
        return alertAndBlink("Pulse rate is out of range!");
    }

    if (isBelowThreshold(spo2, SPO2_MIN)) {
        return alertAndBlink("Oxygen saturation is too low!");
    }

    return true;
}
