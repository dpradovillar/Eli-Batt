#ifndef __SAMPLE_COLLECTOR_H__
#define __SAMPLE_COLLECTOR_H__

#include "Arduino.h"

/**
 * Defines an interface to  define a method to be executed whenever the SampleClock determines a
 * period is elapsed.
 * <p/>
 * Simply make a subclass of SampleCallback and redefine the method to perform a specific action.
 * <p/>
 * This class can be instantiated and it will behave as a NoOpSampleCallback, as it provides an
 * empty method definition.
 */
class SampleCallback {
public:
    /**
    * Method that gets called when the period of time has elapsed (for the given frequency).
    */
    virtual void eventDetected(uint32_t current_usecs);
};

/**
 * Conveniently outputs via Serial interface, the timing callback. Make sure to enable Serial
 * communications and to call Serial.begin() before using this callback.
 */
class SerialCallback : public SampleCallback {
public:
    void eventDetected(uint32_t current_usecs);
};

/**
 * Convenient class, to make a LED blink at regular intervals;
 */
class LedBlinkCallback : public SampleCallback {
private:
    int m_pin;
    int m_state;

public:
    /**
     * Empty constructor, make sure to call the setup method.
     */
    LedBlinkCallback();
    
    /**
     * Initializes this object, make sure to call this method once, in the main setup method.
     */
    void setup(int pin);

    void eventDetected(uint32_t current_usecs);
};

/**
 * Keeps track of frequency and current time, to schedule the execution of actions at regular
 * intervals.
 * <p/>
 * This class is intended to be used with sensors, which may be queried at specific sampling rates.
 * For example, an analog temperature sensor may be read at 100Hz. So specifying a clock with a
 * value of 100Hz and a callback, will make the program execute the action every 10ms.
 * <p/>
 * This class was designed having in mind an external clock of 16MHz. For other clocks it may not
 * give reliable results but for non time-sensitive applications (50ms or 100ms of resolution) it
 * should work ok.
 */
class SampleClock {
private:
    /** Time, in microseconds, which must be elapsed between consecutive action calls. */
    uint32_t m_period;

    /** Time of the last action triggerd by this object. */
    uint32_t m_lastloop;

    /** Object that will get its eventDetected() method called when the period time has elapsed. */
    SampleCallback *m_callback;

public:
    /**
     * Convenient empty constructor to define global variables.
     */
    SampleClock();

    /**
     * Initializes a SampleClock with a given frequency (speed rate at which an action will be
     * executed) and a callback, that defines a custom action to be executed.
     * <p/>
     * For fs (the sampling frequency), make sure to chose any of the following frequencies:
     *         1 Hz
     *         2 Hz
     *         4 Hz
     *         5 Hz
     *         8 Hz
     *        10 Hz
     *        16 Hz
     *        20 Hz
     *        25 Hz
     *        40 Hz
     *        50 Hz
     *        80 Hz
     *       100 Hz
     *       125 Hz
     *       200 Hz
     *       250 Hz
     *       400 Hz
     *       500 Hz
     *       625 Hz
     *      1000 Hz
     *      1250 Hz
     *      2000 Hz
     *      2500 Hz
     *      3125 Hz
     *      5000 Hz
     *      6250 Hz
     *     10000 Hz
     *     12500 Hz
     *     15625 Hz
     *     25000 Hz
     *     31250 Hz
     *     50000 Hz
     *     62500 Hz
     *    125000 Hz
     *    250000 Hz
     * Chosing a different frequency will imply an erratic behaviour, as the internal clock can't
     * handle other timing resolutions.
     * <p/>
     * For frequencies like 0.1Hz, 0.2Hz or any other fractionary value, please refer to the method
     * setPeriod(uint32_t).
     */
    void setup(uint32_t fs, SampleCallback *callback);

    /**
    * Sets the period to be elapsed between successive callback action calls.
    * <p/>
    * This method should not be called, as setting the frequency at the constructor is enough to
    * calculate automatically the correct period. Calling this method will override the chosen
    * frequency.
    * <p/>
    * This method allows to set a finer period, useful for fractionary frequencies (i.e. for periods
    * longer than 1s). The period value should be divisible by 4, and for values smaller than
    * 1000000 (1M uSecs), it should divide 1000000, due to hardware restrictions and a clock running
    * at 16MHz. For periods bigger than 1s it should only be divisible by 4.
    * <p/>
    * Possible values for correct periods, are given in the constructor of this class (there is a
    * list of valid frequencies). Below are shown valid periods for fractionary common frequencies:
    *
    *     T    Freq      uSecs (T*1M, divisible by 4)
    *     1.5  0.6666...   1500000
    *     2.0  0.5         2000000
    *     2.5  0.4         2500000
    *     3.0  0.3333...   3000000
    *     4.0  0.25        4000000
    *     5.0  0.2         5000000
    *     6.0  0.1666...   6000000
    *     8.0  0.125       8000000
    *    10.0  0.1        10000000
    *    12.5  0.08       12500000
    *    15.0  0.0666...  15000000
    *    16.0  0.0625     16000000
    *    20.0  0.05       20000000
    *    25.0  0.04       25000000
    *    30.0  0.0333...  30000000
    *    32.0  0.03125    32000000
    *    40.0  0.025      40000000
    *    50.0  0.02       50000000
    *    55.0  0.01818... 55000000
    *    60.0  0.01666... 60000000
    *
    * Due to limits on 32bits integers, the maximum amount of time is 70 minutes. For any period
    * multiple of 60 seconds, just multiply 60000000 by the number of minutes of the required
    * period.
    */
    void setPeriod(uint32_t uSecs);

    /**
     * Same as loop(micros()), which is useful when the current micros time is not needed in a
     * variable (maybe there are no more uses for it in the calling code).
     * <p/>
     * This method is intended to be called on every loop() iteration of the main program.
     */
    void loop();

    /**
    * Executes the logic for the clock object by looking at the current time and deciding whether
    * the callback should be called.
     * <p/>
     * This method is intended to be called on every loop() iteration of the main program.
    */
    void loop(uint32_t current_usecs);
};

#endif // __SAMPLE_COLLECTOR_H__
