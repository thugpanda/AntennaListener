/*
 * AntennaListener.h - Library for reading Antenna Spikes in the N*
 * Reads the antennas and counts the spikes over a fixed value of time.
 */

#ifndef AntennaListener_h
#define AntennaListener_h

#if (ARDUINO >= 100)
       #include <Arduino.h>
#else
       #include <WProgam.h>
#endif


class AntennaListener {
    public:
            AntennaListener(int ap_, int t_, int l_, uint8_t soa_, float _std);  //constructor
            void        begin(int buffer[]);                            //fill the array on init
            bool        countSpikes(int overflow);                      //count the spikes occurring over fixed time
            bool        countSpikes(int overflow, int threshold);       //count spikes over time but overwrite threshold
            bool        detectSpike(int buffer[], int stdBuffer[]);     //detects spikes using standard deviation, saves values to buffer[] and processed values to stdBuffer[]
            bool        detectSpikeByValue(int t_);                           //detects spikes using threshold
            void        measureHighestSpike();                          //determine the highest spike
            uint16_t    readSpikeCounter();                             //return the value of the spikeCounter and reset it
            uint16_t    readOnlySpikeCounter();                         //return the value of the spikeCounter. Do not reset.
            void        resetSpikeCounter();                            //reset the value of the spikeCounter. Return null.
            void        resetHighestSpike();                            //reset the highest measured spike
            int         returnAnalogIn();                               //returns the latest stored spike
            int         returnAverage();                                //returns the latest average
            int         returnHighestSpike();                           //returns the value of the highest spike
            int         returnLatestReading(int buffer[]);              //returns the latest stored reading]
            uint8_t     returnSizeOfArray();
            int         returnSpikeThreshold();                         //returns the spike threshold
            double      returnStd();                                    //returns the latest standard deviation
            bool        returnTimerFlag();                              //returns the timer flag
    private:
            int         _analogIn;                                      //used in measureHighestSpike
            int         _analogTop;                                     //highest spike
            uint8_t     antennaPin;                                     //the pin where the antenna is connected
            int         _avg;                                           //the mean of the last values
            void        bufferShifter(int newValue, int buffer[]);      //shift oldest out to the left, insert neweset right
            void        initiateTimer(int overflow);                    //initiate timer with t in ms
            void        initiateTimerMicros(int overflow);              //initiate timer with t in ms
            int         mean(int buffer[]);                             //calculate the mean of an array
            int         _overflowMillis;                                //used in timer as overflow
            int         _overflowMicros;                                //used in timerMicros as overflow;
            bool        queryTimer();                                   //query whether the time has overflown yet
            bool        queryTimerMicros();                             //query whether the time has overflown yet
            int         readAntenna();                                  //read the value on the antenna input
            int         readAntennaStore();                             //read the value on the antenna input and store it in _analogIn
            uint8_t     _sizeOfArray;                                   //size of the array
            uint16_t    _spikeCounter;                                  //how many spikes occured in time
            uint8_t     _spikeDectLag;                                  //what is the lag of the spikeDetection
            float       _spikeSTD;                                      //threshold for standard deviations
            int         _spikeThreshold;                                //value needed for signal to count as spike
            long        _startMillis;                                   //used in timer as init
            long        _startMicros;                                   //used in timerMicros as init
            double      std(int buffer[], int mean);                    //calculate standard deviation of a buffer
            double      _std;                                           //standard deviation of the last values
            bool        _timerFlag;                                     //whether timer is running
            bool        _timerFlagMicros;                               //whether timerMicros is running
}

#endif
;
