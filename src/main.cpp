#include <iostream>
#include <cmath>

#include "Whistle/Whistle.h"


#define NUM_NOTES 88
#define OCTAVE(n) (((n)+9)/12)
#define NOTENAME(n) (NOTE_NAME[((n)+9)%12])
#define FREQUENCY(n) ( pow( pow(2.,1./12.), (n)-49. ) * 440. + .5)
const char* NOTE_NAME[12]={"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"};


const int    TREMOR_FREQ_SHIFT  = 10   ; // in Hz (maybe not ideal. could be in %Hz, needs verification)
const double TREMOR_LENGTH_MIN  = 0.120; // in seconds
const double TREMOR_LENGTH_RAND = 0.100; // in seconds
const double TREMOR_DEPTH       = 0.200; // between 0 and 1
const double AMPLITUDE_DEPTH    = 0.200; // between 0 and 1
const double FADEIN_TIME        = 0.050; // in seconds
const double FADEOUT_TIME       = 0.100; // in seconds


using namespace std;


int main(int argc, char** argv)
{
    Whistle whistle(TREMOR_FREQ_SHIFT, TREMOR_LENGTH_MIN, TREMOR_LENGTH_RAND, TREMOR_DEPTH, AMPLITUDE_DEPTH, FADEIN_TIME, FADEOUT_TIME);
    //whistle.verbose(true);
    //whistle.addNoise(true);
    whistle.writeToStream(false);
    whistle.activateWriteToFile("output/out.wav");

    whistle.insertFrequencyKey( 0.0, 916, INTERPOLATION_STEP_DOWN);
    whistle.insertAmplitudeKey( 0.0, 0.0, INTERPOLATION_STEP_DOWN);

    whistle.insertFrequencyKey( 0.039, 916, INTERPOLATION_STEP_DOWN);
    whistle.insertAmplitudeKey( 0.039, 0.8, INTERPOLATION_STEP_DOWN);

        whistle.insertAmplitudeKey( 1.4, 0.0, INTERPOLATION_STEP_DOWN);
        whistle.insertFrequencyKey( 1.8, 677, INTERPOLATION_STEP_DOWN);

    whistle.insertFrequencyKey( 2.1, 677, INTERPOLATION_STEP_DOWN);
    whistle.insertAmplitudeKey( 2.1, 0.5, INTERPOLATION_STEP_DOWN);

        whistle.insertAmplitudeKey( 2.411, 0.0, INTERPOLATION_STEP_DOWN);
        whistle.insertFrequencyKey( 2.450, 677, INTERPOLATION_STEP_DOWN);

    whistle.insertFrequencyKey( 2.502, 677, INTERPOLATION_STEP_DOWN);
    whistle.insertAmplitudeKey( 2.502, 0.5, INTERPOLATION_STEP_DOWN);

        whistle.insertAmplitudeKey( 2.832, 0.0, INTERPOLATION_STEP_DOWN);
        whistle.insertFrequencyKey( 2.870, 916, INTERPOLATION_STEP_DOWN);

    whistle.insertFrequencyKey( 2.922, 916, INTERPOLATION_STEP_DOWN);
    whistle.insertAmplitudeKey( 2.922, 0.9, INTERPOLATION_STEP_DOWN);

        whistle.insertAmplitudeKey( 3.245, 0.0, INTERPOLATION_STEP_DOWN);
        whistle.insertFrequencyKey( 3.290, 819, INTERPOLATION_STEP_DOWN);

    whistle.insertFrequencyKey( 3.323, 819, INTERPOLATION_STEP_DOWN);
    whistle.insertAmplitudeKey( 3.323, 0.3, INTERPOLATION_STEP_DOWN);

    whistle.insertFrequencyKey( 3.523, 710, INTERPOLATION_STEP_DOWN);
    whistle.insertAmplitudeKey( 3.523, 0.3, INTERPOLATION_STEP_DOWN);

        whistle.insertAmplitudeKey( 3.730, 0.0, INTERPOLATION_STEP_DOWN);
        whistle.insertFrequencyKey( 3.760, 812, INTERPOLATION_STEP_DOWN);

    whistle.insertFrequencyKey( 3.788, 812, INTERPOLATION_STEP_DOWN);
    whistle.insertAmplitudeKey( 3.788, 0.3, INTERPOLATION_STEP_DOWN);

        whistle.insertAmplitudeKey( 4.907, 0.0, INTERPOLATION_STEP_DOWN);
        whistle.insertAmplitudeKey( 5.062, 0.0, INTERPOLATION_STEP_DOWN);

    whistle.generateWhistle();

    whistle.closeFile();


    /*
    int note=48;
    double A=1.0;
    const double pauseTime=0.1;
    const double noteTime =0.5;
    */

    /*
    whistle.pause(0.231);
    whistle.blow(916,0.8,1.4);
    whistle.pause(0.658);
    whistle.blow(677,0.5,0.373);
    whistle.pause(0.04);
    whistle.blow(677,0.5,0.356);
    whistle.pause(0.06);
    whistle.blow(916,0.9,0.350);
    whistle.pause(0.054);
    whistle.blow(819,0.3,0.214);
    whistle.blow(710,0.3,0.202);
    whistle.pause(0.054);
    whistle.blow(812,0.3,1.139);
    whistle.pause(0.231);
    */

    /*
    int note=45;
    double A=1.0;
    const double pauseTime=0.1;
    const double noteTime =1.0;
    //cerr <<NOTENAME(note) <<endl;

    whistle.blowOneNote(FREQUENCY(note),A,noteTime*1);
    whistle.pause(pauseTime);
    whistle.blowOneNote(FREQUENCY(note+4),A,noteTime*1);
    whistle.pause(pauseTime);
    whistle.blowOneNote(FREQUENCY(note+7),A,noteTime*1.0);
    whistle.pause(pauseTime);
    whistle.blowOneNote(FREQUENCY(note+12),A,noteTime*1.0);
    whistle.pause(pauseTime);
    whistle.blowOneNote(FREQUENCY(note+7),A,noteTime*1.0);
    whistle.pause(pauseTime);
    whistle.blowOneNote(FREQUENCY(note+4),A,noteTime*1.0);
    whistle.pause(pauseTime);
    whistle.blowOneNote(FREQUENCY(note),A,noteTime*1.0);
    //*/

    /*
    whistle(FREQUENCY(note),A,noteTime*0.75);
    pause(pauseTime);
    whistle(FREQUENCY(note),A,noteTime*0.5);
    pause(pauseTime*0.5);
    whistle(FREQUENCY(note+2),A,noteTime*1.0);
    pause(pauseTime);
    whistle(FREQUENCY(note),A,noteTime*1.0);
    pause(pauseTime);
    whistle(FREQUENCY(note+5),A,noteTime*1.0);
    pause(pauseTime);
    whistle(FREQUENCY(note+4),A,noteTime*2.0);
    pause(pauseTime*3);

    whistle(FREQUENCY(note),A,noteTime*0.75);
    pause(pauseTime);
    whistle(FREQUENCY(note),A,noteTime*0.5);
    pause(pauseTime*0.5);
    whistle(FREQUENCY(note+2),A,noteTime*1.0);
    pause(pauseTime);
    whistle(FREQUENCY(note),A,noteTime*1.0);
    pause(pauseTime);
    whistle(FREQUENCY(note+7),A,noteTime*1.0);
    pause(pauseTime);
    whistle(FREQUENCY(note+5),A,noteTime*2.0);
    pause(pauseTime*3);
    //*/

    /*
    whistle(FREQUENCY(note),A,noteTime*0.75);
    pause(pauseTime);
    whistle(FREQUENCY(note),A,noteTime*0.5);
    pause(pauseTime*0.5);
    whistle(FREQUENCY(note+12),A,noteTime*1.25);
    pause(pauseTime);
    whistle(FREQUENCY(note+9),A,noteTime*1.0);
    pause(pauseTime);
    whistle(FREQUENCY(note+5),A,noteTime*1.0);
    pause(pauseTime);
    whistle(FREQUENCY(note+4),A,noteTime*1.25);
    pause(pauseTime);
    whistle(FREQUENCY(note+2),A,noteTime*2.5);
    pause(pauseTime*3);
    //*/

    /*
    whistle(FREQUENCY(note+10),A,noteTime*0.75);
    pause(pauseTime);
    whistle(FREQUENCY(note+10),A,noteTime*0.5);
    pause(pauseTime*0.5);
    whistle(FREQUENCY(note+9),A,noteTime*1.0);
    pause(pauseTime);
    whistle(FREQUENCY(note+5),A,noteTime*1.0);
    pause(pauseTime);
    whistle(FREQUENCY(note+7),A,noteTime*1.0);
    pause(pauseTime);
    whistle(FREQUENCY(note+5),A,noteTime*2.0);
    pause(pauseTime*3);
    //*/


    //cerr <<FREQUENCY(note) <<endl;
    //cerr <<NOTENAME(note) <<endl;


    return 0;
}



