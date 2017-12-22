# Antenna Listener
*Antenna Listener* is a library for Arduino and Teensy-like systems. It is able to detect spikes in antenna signals which are send to one of the ADCs of the MCU.  
It is published under *GNU General Public License 3.0*.

## Download
[Head over to the releases](https://github.com/thugpanda/AntennaListener/releases) and extract the downloaded folder. Move it to your Arduino IDE Library folder. Restart the IDE. Import it using `#import <AntennaListener.h>`.

Consult [my website](https://downloads.thugpanda.de/antennalistener) for further documentation. Or use this readme. However you like.

## Library
#### Constructor
```
AntennaListener(int ap_, int t_, int l_, uint8_t soa_, float _std);
```
The constructor needs the following arguments:

* `int ap_`: AntennaPin, the analogPin on which the RF-Signals gets read.
* `int t_`: Fixed threshold value at which a reading counts as a spike.
* `int l_`: stdLag, how many samples are backtracked.
* `uint8_t`: Size of the average & std array. Max 255 values. Normally about 10-25 is a good sample size. Higher value is more  stable and therefore "slower" triggering.
* `float _std`: How big the standard deviation of a value has to be in order to trigger the detection. Depending on the signal values near 0 or as high as multiple deviations are suitable.

```
void AntennaListener::begin(int buffer[]);
```
Initial function called in setup. The buffer passed has to be the same size as the `uint8_t soa_` in the constructor. Fills the buffer with readings and calculates average and std for the first time.

#### Functions
##### count spikes
```
bool AntennaListener::countSpikes(int overflow);
```
Counts the values bigger than the threshold over a custom timespan.

* `int overflow`: Timespan in milliseconds over which all spikes bigger than `int t_` are counted. Returns the value.

```
bool AntennaListener::countSpikes(int overflow, int t_);
```

* `int overflow`: Same as above.
* `int t_:` Custom threshold just for this function.

##### detect spikes
```
bool AntennaListener::detectSpike(int buffer[], int stdBuffer[]));
```
Detects a spike based on the standard deviation. Calculates average and standard deviation on the fly.

* `int buffer[]`: Buffer in which the readings are stored. Has to be size of `uint8_t soa_`.
* `int stdBuffer[]`: Buffer in which the readings for standard deviation are stored (they get converted to a different scale). Has to be size of `uint8_t soa_`.

```
bool AntennaListener::detectSpikeByValue(int t_);
```
Detects a spike based on passing of a threshold.

* `int t_`: Custom threshold.

##### measure spikes
```
void AntennaListener::measureHighestSpike();
```
Reads the antenna and stores the highest reading in a variable. Can be queried by `returnHighestSpike()`. Reset by using `resetHighestSpike()`.

##### reading variables & measurements
```
uint16_t AntennaListener::readOnlySpikeCounter();
```
Reads the spike counter but does not reset it.

```
uint16_t AntennaListener::readSpikeCounter();
```
Reads the spike counter **and clears it**.

```
void AntennaListener::resetSpikeCounter();
```
Resets the spike counter. Returns nothing.

```
void AntennaListener::resetHighestSpike();
```
Resets the highest measured spike.

```
int AntennaListener::returnAnalogIn();
```
Returns the `analogRead()` stored in the last reading-pass.

```
int AntennaListener::returnAverage();
```
Returns the latest calculated average from `detectSpike()`.

```
int AntennaListener::returnHighestSpike();
```
Returns the value of the highest stored spike. Use `resetHighestSpike()` to clear.

```
int AntennaListener::returnLatestReading(int buffer[]));
```
Returns the latest reading of the value buffer.

```
uint8_t AntennaListener::returnSizeOfArray();
```
Returns the size of array used in all `buffer[]` methods.

```
int AntennaListener::returnSpikeThreshold();
```
Returns the value of the spike threshold.

```
double AntennaListener::returnStd();
```
Returns the latest calculated standard deviation.

```
bool AntennaListener::returnTimerFlag();
```
Returns the timer flag used in `countSpikes`.
