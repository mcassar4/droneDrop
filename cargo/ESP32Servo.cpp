#include "ESP32Servo.h"
#include "esp32-hal-ledc.h"

#ifndef ARDUINO_H
#include "Arduino.h"
#endif

int Servo::ServoCount = 0;
int Servo::ChannelUsed[MAX_SERVOS] = {0};

Servo::Servo() : servoChannel(0), min(DEFAULT_uS_LOW), max(DEFAULT_uS_HIGH),
                 pinNumber(-1), timer_width(DEFAULT_TIMER_WIDTH), 
                 ticks(DEFAULT_PULSE_WIDTH_TICKS), timer_width_ticks(DEFAULT_TIMER_WIDTH_TICKS) {
    // Check for a free channel
    for (int i = 1; i <= MAX_SERVOS; i++) {
        if (ChannelUsed[i] == 0) {
            ChannelUsed[i] = 1;
            servoChannel = i;
            ServoCount++;
            break;
        }
    }
}

int Servo::attach(int pin) {
    return attach(pin, DEFAULT_uS_LOW, DEFAULT_uS_HIGH);
}

int Servo::attach(int pin, int min, int max) {
    if (servoChannel == 0 || servoChannel > MAX_SERVOS) {
        return 0; // No available channel or invalid channel
    }

    pinNumber = pin;
    this->min = min < MIN_PULSE_WIDTH ? MIN_PULSE_WIDTH : min;
    this->max = max > MAX_PULSE_WIDTH ? MAX_PULSE_WIDTH : max;

    setupPwmChannel();
    return servoChannel;
}

void Servo::detach() {
    if (attached()) {
        ledcDetachPin(pinNumber);
        ChannelUsed[servoChannel] = 0;
        pinNumber = -1;
        ServoCount--;
    }
}

void Servo::write(int value) {
    if (value < MIN_PULSE_WIDTH) {
        value = constrain(value, 0, 180);
        value = map(value, 0, 180, min, max);
    }
    writeMicroseconds(value);
}

void Servo::writeMicroseconds(int value) {
    if (attached()) {
        value = constrain(value, min, max);
        ticks = usToTicks(value);
        ledcWrite(servoChannel, ticks);
    }
}

int Servo::read() {
    return attached() ? map(ticks, min, max, 0, 180) : 0;
}

int Servo::readMicroseconds() {
    return attached() ? ticksToUs(ticks) : 0;
}

bool Servo::attached() {
    return servoChannel > 0 && servoChannel <= MAX_SERVOS && ChannelUsed[servoChannel];
}

void Servo::setTimerWidth(int value) {
    timer_width = constrain(value, MINIMUM_TIMER_WIDTH, MAXIMUM_TIMER_WIDTH);
    setupPwmChannel();
}

int Servo::readTimerWidth() {
    return timer_width;
}

int Servo::usToTicks(int usec) {
    return (int)(((long)usec * (1 << timer_width)) / REFRESH_USEC);
}

int Servo::ticksToUs(int ticks) {
    return (int)(((long)ticks * REFRESH_USEC) / (1 << timer_width));
}

void Servo::setupPwmChannel() {
    if (attached()) {
        ledcSetup(servoChannel, REFRESH_CPS, timer_width);
        ledcAttachPin(pinNumber, servoChannel);
    }
}
