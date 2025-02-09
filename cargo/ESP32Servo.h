#ifndef ESP32_Servo_h
#define ESP32_Servo_h

#define ESP32_Servo_VERSION 1 // Library version

// Default pulse width values for servos
#define DEFAULT_uS_LOW 400
#define DEFAULT_uS_HIGH 2500

// PWM timer settings
#define DEFAULT_TIMER_WIDTH 10
#define DEFAULT_TIMER_WIDTH_TICKS 1024
#define MINIMUM_TIMER_WIDTH 10
#define MAXIMUM_TIMER_WIDTH 20

// Pulse width constraints
#define MIN_PULSE_WIDTH 500 // Minimum pulse sent to a servo in microseconds
#define MAX_PULSE_WIDTH 2500 // Maximum pulse sent to a servo in microseconds
#define DEFAULT_PULSE_WIDTH 1500 // Default pulse width when servo is attached
#define DEFAULT_PULSE_WIDTH_TICKS 4825
#define REFRESH_CPS 50 // Refresh rate in Hz
#define REFRESH_USEC 20000 // Refresh period in microseconds

#define MAX_SERVOS 16 // Maximum number of PWM channels on ESP32

class Servo {
public:
    Servo();
    int attach(int pin); // Attaches a servo to a pin
    int attach(int pin, int min, int max); // Attaches a servo with custom pulse width range
    void detach(); // Detaches the servo
    void write(int value); // Sets the servo angle or pulse width in microseconds
    void writeMicroseconds(int value); // Sets the servo pulse width in microseconds
    int read(); // Returns the last set servo angle
    int readMicroseconds(); // Returns the last set servo pulse width in microseconds
    bool attached(); // Checks if the servo is attached

    // ESP32 specific functions
    void setTimerWidth(int value); // Sets the PWM timer width
    int readTimerWidth(); // Returns the current PWM timer width

private:
    int usToTicks(int usec); // Converts microseconds to ticks
    int ticksToUs(int ticks); // Converts ticks to microseconds
    void setupPwmChannel(); // Sets up the PWM channel

    static int ServoCount; // Total number of attached servos
    static int ChannelUsed[MAX_SERVOS]; // Tracks whether a channel is in service

    int servoChannel; // Channel number for this servo
    int min; // Minimum pulse width for this servo
    int max; // Maximum pulse width for this servo
    int pinNumber; // GPIO pin assigned to this channel
    int timer_width; // PWM timer width
    int ticks; // Current pulse width in ticks
    int timer_width_ticks; // Number of ticks at rollover
};

#endif // ESP32_Servo_h
