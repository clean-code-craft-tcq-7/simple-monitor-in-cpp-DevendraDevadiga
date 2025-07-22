#include "./monitor.h"
#include <assert.h>
#include <thread>
#include <chrono>
#include <iostream>

using std::cout, std::flush, std::this_thread::sleep_for, std::chrono::seconds;

// Constants for vital thresholds
constexpr float TEMP_LOW = 95.0f;
constexpr float TEMP_HIGH = 102.0f;
constexpr float PULSE_LOW = 60.0f;
constexpr float PULSE_HIGH = 100.0f;
constexpr float SPO2_MIN = 90.0f;

constexpr int BLINK_COUNT = 6;
constexpr int BLINK_INTERVAL_SEC = 1;

void blinkWarning() {
    for (int i = 0; i < BLINK_COUNT; ++i) {
        cout << "\r* " << flush;
        sleep_for(seconds(BLINK_INTERVAL_SEC));
        cout << "\r *" << flush;
        sleep_for(seconds(BLINK_INTERVAL_SEC));
    }
}

int vitalsOk(float temperature, float pulseRate, float spo2) {
    if (temperature < TEMP_LOW || temperature > TEMP_HIGH) {
        cout << "Temperature is critical!\n";
        blinkWarning();
        return 0;
    }

    if (pulseRate < PULSE_LOW || pulseRate > PULSE_HIGH) {
        cout << "Pulse Rate is out of range!\n";
        blinkWarning();
        return 0;
    }

    if (spo2 < SPO2_MIN) {
        cout << "Oxygen Saturation out of range!\n";
        blinkWarning();
        return 0;
    }

    return 1;
}
