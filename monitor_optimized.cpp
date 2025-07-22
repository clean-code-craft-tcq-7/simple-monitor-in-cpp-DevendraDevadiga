#include "./monitor.h"
#include <assert.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <string>

using std::cout, std::endl;
using std::flush;
using std::this_thread::sleep_for;
using std::chrono::seconds;

// Constants
constexpr int BLINK_COUNT = 6;
constexpr int BLINK_INTERVAL_SEC = 1;

// Vital definition
struct Vital {
    const char* name;
    float value;
    float min;
    float max;
    bool onlyLowerBound;  // true = check only min, false = check min & max
};

// Blink warning pattern
void blinkWarning() {
    for (int i = 0; i < BLINK_COUNT; ++i) {
        cout << "\r* " << flush;
        sleep_for(seconds(BLINK_INTERVAL_SEC));
        cout << "\r *" << flush;
        sleep_for(seconds(BLINK_INTERVAL_SEC));
    }
    cout << endl;
}

// Show message and blink
int alertAndBlink(const char* message) {
    cout << message << endl;
    blinkWarning();
    return 0;
}

// Extracted logic to reduce CCN
bool isVitalOutOfRange(const Vital& v) {
    if (v.onlyLowerBound) {
        return v.value < v.min;
    }
    return v.value < v.min || v.value > v.max;
}

// Check and handle vital
int checkVital(const Vital& vital) {
    if (isVitalOutOfRange(vital)) {
        std::string msg = std::string(vital.name) + " is out of range!";
        return alertAndBlink(msg.c_str());
    }
    return 1;
}

// Master checker function
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
