/*
    ANTENNA LISTENER

    Example: Detect Spikes.

    The signal on analogPin 0 is read. If the next reading
    is deviating more than 0.3 * standard deviation from the
    average of the last 15 signals a spike is detected and the
    value is printed.
    If nothing is detected the new average and standard deviation
    are returned.

    This example code is in the public domain.

*/

#include <AntennaListener.h>

#define ap 0
#define soa 15

unsigned    int     spikeBuffer[soa];
unsigned    int     spikeStdBuffer[soa];

const       float   stdFactor           =   0.5;

AntennaListener     antenna(ap, 200, soa, soa, stdFactor);

void setup() {
    Serial.begin(9600);

    antenna.begin(spikeBuffer);
}

void loop() {
    if(antenna.detectSpike(spikeBuffer, spikeStdBuffer)) {
        Serial.print("Spike detected! Value: ");
        Serial.println(antenna.returnAnalogIn());
    } else {
        Serial.print("No spike detected. New Average: ");
        Serial.print(antenna.returnAverage());
        Serial.print(" New Standard Deviation: ");
        Serial.println(antenna.returnStd());
    }
}