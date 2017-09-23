/*
    ANTENNA LISTENER

    Example: Count spikes.

    The signal on analogPin 0 is read. Over the time of 20 milliseconds
    all readings greater than the threshold are counted.
    If there are no readings bigger than the value nothing has been detected.
    Otherwise the number of counts gets printed and the counter gets reset.

    This example code is in the public domain.

*/

#include <AntennaListener.h>

#define ap 0

unsigned    int timerOverflow   =   20;
unsigned    int spikeThreshold  =   200;

AntennaListener antenna(ap,spikeThreshold,0,0,0);

void setup() {
    Serial.begin(9600);
}

void loop() {
    if(antenna.countSpikes(timerOverflow)) {
        Serial.print("In ");
        Serial.print(timerOverflow);
        Serial.print(" milliseconds I counted: ");
        Serial.println(antenna.readSpikeCounter());
    } else {
        Serial.println("Nothing detected.");
    }
}