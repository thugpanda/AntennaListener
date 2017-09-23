/*
 * AntennaListener.h - Library for reading Antenna Spikes in the N*
 * Reads the antennas and counts the spikes over a fixed value of time.
 */

#include "Arduino.h"
#include "AntennaListener.h"
AntennaListener::AntennaListener(int aP_, int t_, int l_, uint8_t soa_, float std_) {
    /* Constructor.
     *
     * Sets AntennaPin, Threshold and initiates _spikeCounter.
     */
    antennaPin      =   aP_;
    _spikeThreshold =   t_;
    _spikeSTD       =   std_;
    _spikeCounter   =   0;
    _spikeDectLag   =   l_;
    _sizeOfArray    =   soa_;
    _std            =   0;
    _avg            =   0;
}

void AntennaListener::begin(int buffer[]) {
    /* Initialize.
     * Add the values from the last 20 readings to the array. Asc order.
     * Calculate mean and standard deviation.
     * After this everything is ready to go.
     */
    for(int i = 0; i < _sizeOfArray; i++) {
        buffer[i]   =   readAntenna();
    }
    _avg    =   mean(buffer);
    _std    =   std(buffer, _avg);
}

void AntennaListener::bufferShifter(int newValue, int buffer[]) {
    /* Remove oldest, add newest
     * The oldest entry is shifted to the left and therefore falling out of
     * the array. After that every value is shifted 1 index to the left. In
     * the end the newest value is inserted to the rightmost index of the array.
     * As the function is working with a reference of the array there is no copy
     * created but the original buffer modified. This should speed things up and
     * nothing is returned.
     */
    for(int i = 0; i < _sizeOfArray-1; i++) {
        buffer[i] = buffer[i+1];
    }
    buffer[_sizeOfArray-1]    =   newValue;
}

bool AntennaListener::countSpikes(int overflow, int t_) {
    /* Count the spikes oveer a period of time. Use custom threshold.
     */
    _spikeThreshold    =   t_;
    if(!_timerFlag) {
        initiateTimer(overflow);
    }
    while(!queryTimer() && _timerFlag) {
        if(readAntenna() > _spikeThreshold) {
            _spikeCounter++;
        }
    }
    if(_spikeCounter > 0) {
        return true;
    } else {
        return false;
    }
}

bool AntennaListener::countSpikes(int overflow) {
    /* Count the spikes over a period of time.
     * The timer gets queried and every reading gets checked against the threshold.
     * If it is higher than the threshold it adds towards the counter.
     * Returns true if finished. Manipulates _spikerCounter.
     */
    if(!_timerFlag) {
       initiateTimer(overflow);
    }
    while(!queryTimer() && _timerFlag) {
        if(readAntenna() > _spikeThreshold) {
            _spikeCounter++;
        }
    }
    if(_spikeCounter > 0) {
        return true;
    } else {
        return false;
    }
}

bool AntennaListener::detectSpike(int buffer[], int stdBuffer[]) {
    /* Detect a spike by surrounding.
     * The antenna listener stores the value in _analogIn. If this value minus the average
     * value is bigger than the threshold multiplied with the standard deviation the function does it magic.
     * The value passed to everything then is half the value of the signal (influence = 0.5)
     * added to the inverted value of the half of the last converted valie.
     * If the signal is smaller it becomes 0.
     * In every case the value (be it real or 0) is shifted into the bufferstack. The average and
     * the standard deviation are calculated. Function returns 0.
     */
    int     _dAnalogIn  =   0;
    bool    _return     =   false;

    
    if( abs(readAntennaStore() - _avg) > (_spikeSTD * _std)) {
        _return     =   true;
        _dAnalogIn  =   stdBuffer[_sizeOfArray-1];
    } else {
        _return = false;
        _dAnalogIn =   (_analogIn / 3) + (-1 * (stdBuffer[_sizeOfArray-1] / 3));
    }
    bufferShifter(_analogIn, buffer);
    _avg   =   mean(buffer);
    _std   =   std(stdBuffer, _avg);
    bufferShifter(_dAnalogIn, stdBuffer);
    return _return;
}

bool AntennaListener::detectSpikeByValue(int t_) {
    /* Detect spike by querying relation to threshold.
     */
    if(readAntennaStore() > t_) {
        return true;
    } else {
        return false;
    }
}


int AntennaListener::mean(int buffer[]) {
    /* Return mean of array.
     */
    int     sum =   0;
    for(int i = 0; i < _sizeOfArray; i++) {
        sum += buffer[i];
    }
    return (sum /= _sizeOfArray);
}

void AntennaListener::measureHighestSpike() {
    /* Store the highest peak.
     * If the last measurement was higher than the current highest measurement
     * the last one gets saved as the highest one.
     * As readAntenna() reads the analogPin on every call the values might differ
     * from within two reads. In order to avoid this glitch the read is stored in a
     * variable before it is compared to analogTop. If it is higher, instead of reading
     * a new value by calling readAntenna() the saved value gets set to highest.
     */
    int measurement =   readAntenna();
    if(measurement > _analogTop) {
        _analogTop   =   measurement;
    }
}

void AntennaListener::initiateTimer(int overflow) {
    /* Initiate the timer with desired values.
     * int overflow is the time in millis after which the timer overflows.
     */
    _startMillis    =   millis();
    _overflowMillis =   overflow + _startMillis;
    _timerFlag      =   true;
}

void AntennaListener::initiateTimerMicros(int overflow) {
    /* Initiate the timer with desired values.
     * int overflow is the time in micros after which the timer overflows.
     * Consider the resolution of your IC.
     */
    _startMicros       =   micros();
    _overflowMicros    =   overflow + _startMicros;
    _timerFlagMicros   =   true;
}

bool AntennaListener::queryTimer() {
    /* Run the timer.
     * If the current millis is bigger than the overflow point set with
     * AntennaListener::initiateTimer() the timer returns true, else false.
     */
    if(millis() >= _overflowMillis) {
        _timerFlag  =   false;
        return true;
    }
    return false;
}

bool AntennaListener::queryTimerMicros() {
    /* Run the timer.
     * If the current micros is bigger than the overflow point set with
     * AntennaListener::initiateTimerMicros() the timer returns true, else false.
     */
    if(micros() >= _overflowMicros) {
        return true;
        _timerFlagMicros   =   false;
    }
    return false;
}

int AntennaListener::readAntenna() {
    /* reads the analogInput of the antennaPin
     */
    return analogRead(antennaPin);
}

int AntennaListener::readAntennaStore() {
    /* reads the analogInput on the antennaPin and stores it to analogIn.
     */
     _analogIn  =   analogRead(antennaPin);
    return _analogIn;
}

uint8_t AntennaListener::readOnlySpikeCounter() {
    /* Read the value but do not reset.
     */
    return _spikeCounter;
}

uint16_t AntennaListener::readSpikeCounter() {
    /* Read the value and return it. Reset _spikeCounter.
     * As a function terminates after return it is required to copy the
     * value of _spikeCounter, reset it and return the saved value.
     */
    int     _sc             =   _spikeCounter;
            _spikeCounter   =   0;
    return  _sc;
}

void AntennaListener::resetHighestSpike() {
    /* Reset the highest measured spike.
     */
    _analogTop  =   0;
}

void AntennaListener::resetSpikeCounter() {
    /* Reset the spike counter. Return null.
     */
    _spikeCounter  =   0;
}

int AntennaListener::returnAnalogIn() {
    /* Return _analogIn
     */
    return _analogIn;
}

int AntennaListener::returnAverage() {
    /* Return latest average.
     */
    return _avg;
}

int AntennaListener::returnHighestSpike() {
    /* Return highest measured spike.
     */
    return _analogTop;
}

int AntennaListener::returnLatestReading(int buffer[]) {
    /* Return the last element from the stack.
     */
    return buffer[_sizeOfArray];
}

uint8_t AntennaListener::returnSizeOfArray() {
    return _sizeOfArray;
}

int AntennaListener::returnSpikeThreshold() {
    /* Return spike threshold.
     */
    return _spikeThreshold;    
}

double AntennaListener::returnStd() {
    /* Return latest standard deviation.
     */
    return _std;
}

bool AntennaListener::returnTimerFlag() {
    /* Return timer flag.
     */
    return _timerFlag;
}

double AntennaListener::std(int buffer[], int avg) {
    /* Calculate standard deviation of array.
     * Sums the squared values of the differential of each value in the
     * array from the mean. The square root of the mean of this sum
     * is then returned. Tada, standard deviation.
     */
    double   sqDevSum    =   0.0;
    for(int i = 0; i < _sizeOfArray; i++) {
        sqDevSum += sq(avg - buffer[i]);    //squares the difference from value to mean
    }
    return sqrt(sqDevSum / _sizeOfArray);   //returns the std
}
