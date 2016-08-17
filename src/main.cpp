#include <iostream>
#include <cmath>

#include "Whistle/Whistle.h"


#define NUM_NOTES 88
#define OCTAVE(n) (((n)+9)/12)
#define NOTENAME(n) (NOTE_NAME[((n)+9)%12])
#define FREQUENCY(n) ( pow( pow(2.,1./12.), (n)-49. ) * 440. + .5)
const char* NOTE_NAME[12]={"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"};


const int    TREMOR_FREQ_SHIFT  = 10   ; // in Hz (maybe not ideal. could be in %Hz, needs verification)
const double TREMOR_LENGTH_MIN  = 0.240; // in seconds
const double TREMOR_LENGTH_RAND = 0.100; // in seconds
const double TREMOR_DEPTH       = 0.200; // between 0 and 1
const double AMPLITUDE_DEPTH    = 0.200; // between 0 and 1
const double FADEIN_TIME        = 0.050; // in seconds
const double FADEOUT_TIME       = 0.100; // in seconds


using namespace std;

void zelda(Whistle& whistle);
void tune1(Whistle& whistle);
void stepTest(Whistle& whistle);
void musicalScale(Whistle& whistle);


int main(int argc, char** argv)
{
    Whistle whistle(TREMOR_FREQ_SHIFT, TREMOR_LENGTH_MIN, TREMOR_LENGTH_RAND, TREMOR_DEPTH, AMPLITUDE_DEPTH, FADEIN_TIME, FADEOUT_TIME);
    //whistle.verbose(true);
    whistle.addNoise(true);
    whistle.secondHarmonicRelativeAmp(0.01);
    whistle.writeToStream(false);
    whistle.activateWriteToFile("output/out.wav");

    //zelda(whistle);
    //stepTest(whistle);
    //tune1(whistle);
    musicalScale(whistle);

    whistle.generateWhistle();

    whistle.closeFile();

    return 0;
}

void musicalScale(Whistle& whistle)
{
    double time[] = {  0.0, 0.41, 0.73, 0.88, 1.16, 1.35, 1.68, 1.86, 2.17, 2.31, 2.63, 2.84, 3.21, 3.42, 3.96, 4.505 };
    double amps[] = {  0.0,  0.4,  0.0,  0.9,  0.0,  0.2,  0.0,  0.2,  0.0,  0.2,  0.0,  0.5,  0.0,  0.3,  0.0,   0.0 };
    double freq[] = {  885,  885, 1109, 1109, 1319, 1319, 1760, 1760, 1319, 1319, 1109, 1109,  880,  880,  880,   880 };
    //double freq[] = {  880,  880,  880, 1109, 1109, 1319, 1319, 1760, 1760, 1319, 1319, 1109, 1109,  880,  880,   880 };

    for(unsigned int i=0;i<sizeof(time)/sizeof(time[0]);i++)
    {
        if(i==0)
            whistle.insertFrequencyKey( time[i], freq[i], INTERPOLATION_STEP_DOWN);
        else
            whistle.insertFrequencyKey( time[i]+0.07, freq[i], INTERPOLATION_STEP_DOWN);
        whistle.insertAmplitudeKey( time[i], amps[i], INTERPOLATION_STEP_DOWN);
    }
}

void stepTest(Whistle& whistle)
{
    double time[] = {  0.0, 0.66, 1.60, 2.42, 3.25, 4.20, 5.06, 6.12, 6.97, 7.71, 8.50 };
    double amps[] = {  0.0,  0.5,  0.8,  0.5,  0.2,  0.5,  0.6,  0.4,  0.2,  0.0,  0.0 };
    double freq[] = { 1020, 1020, 1175, 1020,  904, 1020, 1175, 1020,  904,  904,  904 };

    for(unsigned int i=0;i<sizeof(time)/sizeof(time[0]);i++)
    {
        whistle.insertFrequencyKey( time[i], freq[i], INTERPOLATION_STEP_DOWN);
        whistle.insertAmplitudeKey( time[i], amps[i], INTERPOLATION_STEP_DOWN);
    }
}


void tune1(Whistle& whistle)
{
    double time[] = { 0.0, 0.88, 1.60, 1.77, 2.97, 3.26, 3.62, 3.86, 4.40, 4.50, 5.19, 5.41, 6.25, 6.95, 7.31, 7.42, 8.09, 9.21, 9.53, 9.65, 10.03, 10.10, 10.86, 10.93, 11.55, 11.85, 12.67, 12.84, 13.17, 13.35, 13.67, 13.92, 14.60, 14.91, 15.31, 15.38, 15.76, 16.00, 17.56, 20.0};
    double amps[] = { 0.0,  0.6,  0.0,  0.6,  0.0,  0.4,  0.0,  0.4,  0.0,  0.4,  0.0,  0.4,  0.0,  0.4,  0.0,  0.4,  0.0,  0.4,  0.0,  0.6,   0.0,   0.6,   0.0,   0.8,   0.0,   0.6,   0.0,   0.4,   0.0,   0.4,   0.0,   0.4,   0.0,   0.4,   0.0,   0.4,   0.0,   0.3,   0.0,  0.0};
    double freq[] = {1026, 1026, 1026, 1026,  865,  865,  865,  865, 1026, 1026, 1026, 1026,  753,  753,  753,  753,  685,  685,  777,  777,   834,   834,   996,   996,   857,   857,   736,   736,   662,   662,   744,   744,   608,   608,   550,   550,   496,   496,   496,  496};

    for(unsigned int i=0;i<sizeof(time)/sizeof(time[0]);i++)
    {
        whistle.insertFrequencyKey( time[i], freq[i], INTERPOLATION_STEP_DOWN);
        whistle.insertAmplitudeKey( time[i], amps[i], INTERPOLATION_STEP_DOWN);
    }
}


void zelda(Whistle& whistle)
{
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
}


void test()
{
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
}


