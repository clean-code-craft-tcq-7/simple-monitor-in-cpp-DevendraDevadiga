#include "./monitor.h"
#include <assert.h>
#include <thread>
#include <chrono>
#include <iostream>

using std::cout, std::endl;
using std::flush;
using std::this_thread::sleep_for;
using std::chrono::seconds;

constexpr int BLINK_COUNT = 6;
constexpr int BLINK_INTERVAL_SEC = 1;

struct Vital {
    const char* name;
    float value;
    float min;
    float max;
    bool onlyLowerBound;  // true if only min is checked
};

// Warning blink animation
void blinkWarning() {
    for (int i = 0; i < BLINK_COUNT; ++i) {
        cout << "\r* " << flush;
        sleep_for(seconds(BLINK_INTERVAL_SEC));
        cout << "\r *" << flush;
        sleep_for(seconds(BLINK_INTERVAL_SEC));
    }
    cout << endl;
}

// Print alert message + blinking
int alertAndBlink(const char* message) {
    cout << message << endl;
    blinkWarning();
    return 0;
}

// Check if a vital is in range
int checkVital(const Vital& vital) {
    bool outOfRange = vital.onlyLowerBound
                      ? (vital.value < vital.min)
                      : (vital.value < vital.min || vital.value > vital.max);
    if (outOfRange) {
        return alertAndBlink((std::string(vital.name) + " is out of range!").c_str());
    }
    return 1;
}

// Master function to verify all vitals
int vitalsOk(float temperature, float pulseRate, float spo2) {
    Vital vitals[] = {
        {"Temperature",        temperature, 95.0f, 102.0f, false},
        {"Pulse rate",         pulseRate,   60.0f, 100.0f, false},
        {"Oxygen saturation",  spo2,        90.0f, 0.0f,   true}
    };

    for (const auto& vital : vitals) {
        if (!checkVital(vital)) {
            return 0;
        }
    }
    return 1;
}
