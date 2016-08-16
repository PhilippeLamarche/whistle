//http://cboard.cprogramming.com/c-programming/158247-musical-notes-valid-program.html

#include <iostream>
#include <cmath>
#include <stdint.h>
#include <string>
#include <ctime>
#include <fstream>
#include <sstream>
#include <iomanip>

#ifndef M_PI
    #define M_PI 3.1415926535897932384626433832795
#endif // M_PI

#include "Whistle.h"

using namespace std;


// Fun fact: My whistle ranges from approximately 450Hz to 3kHz this is roughly equivalent to A4 to F#7 (2 octaves and 5/6)


const double MAXFREQUENCY      = 20000.0;
const int    TIMEPRECISION     = 6      ; //Microseconds (10^-6 seconds)
const int    RANDPOSSIBILITIES = 1000   ; //Number of possibiles for a random draw

//   vvvvvvvv - PID Constants - vvvvvvvv
const double FREQ_Kp =  0.0005;
const double FREQ_Ki =  0.0000; //Conviniently, this parameter can be though as the overshoot
const double FREQ_Kd = -0.9700;

const double FREQ_Kp_RANGE = 0.000000;
const double FREQ_Ki_RANGE = 0.000000; //Conviniently, this parameter can be though as the overshoot
const double FREQ_Kd_RANGE = 0.000000;

const double AMP_Kp = 60;
const double AMP_Ki = 10;
const double AMP_Kd = 10;
//   ^^^^^^^^ - PID Constants - ^^^^^^^^


double round(double valueToRound,int precision);
bool   dividedByZero(double value);
double randFrom0To1();
double randFromMinus1To1();
void   clamp(const double minimum, double* value, const double maximum);
void   clamp(const int16_t minimum, int16_t* value, const int16_t maximum);
void   clampFreq(double* freq);


Whistle::Whistle():
    Whistle(5, 0.06, 0.10, 0.10, 0.20, 0.05, 0.10)
{}

Whistle::Whistle(const double imperfectTremorFreqShift, const double imperfectTremorLengthMin, const double imperfectTremorLengthRand, const double imperfectTremorDepth,
                 const double imperfectAmplitudeDepth, const double fadeInTime, const double fadeOutTime):
    m_imperfectTremorFreqShift  (imperfectTremorFreqShift),
    m_imperfectTremorLengthMin  (imperfectTremorLengthMin),
    m_imperfectTremorLengthRand (imperfectTremorLengthRand),
    m_imperfectTremorDepth      (imperfectTremorDepth),
    m_imperfectAmplitudeDepth   (imperfectAmplitudeDepth),
    m_fadeInTime                (fadeInTime),
    m_fadeOutTime               (fadeOutTime),
    m_addNoise                  (false),
    m_noiseAmp                  (0.005),
    m_secondHarmonicAmp         (0.1),
    m_sampling                  (44100),
    m_maxVolume                 (32767),
    m_writeToFile               (false),
    m_writeToStream             (false),
    m_verbose                   (false),
    m_elapsedTime               (0),
    m_offsetX                   (0),
    m_offsetY                   (0),
    m_initialImperfectTremorTime(0),
    m_finalImperfectTremorTime  (0),
    m_imperfectTremorAmpDiff    (0),
    m_PID_curentKp              (0),
    m_PID_curentKi              (0),
    m_PID_curentKd              (0),
    m_PID_Integral              (0),
    m_PID_lastError             (0),
    m_lastFreqKey               {0,-1,INTERPOLATION_STEP_UP},
    m_lastAmpKey                {0,-1,INTERPOLATION_STEP_UP},
    m_lastTremorKey             {0,-1,INTERPOLATION_STEP_UP}
{
    srand(time(0));

#ifdef USE_CSV
    m_csvOut.open("output/outPID.csv");
    m_csvOut <<",," <<endl
             <<",Target,Value" <<endl;
#endif //#ifdef USE_CSV
}


void Whistle::generateWhistle()
{
    if(m_verbose)
        cout <<"Whistle Generation Beginning!" <<endl;


//--------------------------------------------------------------------------------------------------------------------------------------

    double finalTime = 0.0,
           freq      = 0.0,
           amp       = 0.0;

    if(m_freqKeysList.size()<=0)
        m_freqKeysList.push_back({0.0,880.0,INTERPOLATION_STEP_DOWN});
    if(m_ampKeysList.size()<=0)
        m_ampKeysList.push_back({0.0,1.0,INTERPOLATION_STEP_DOWN});
    if(m_tremorKeysList.size()<=0)
        m_tremorKeysList.push_back({0.0,0.0,INTERPOLATION_STEP_DOWN});

    finalTime = max( max(m_freqKeysList.back().time,m_ampKeysList.back().time), m_tremorKeysList.back().time );

    while(m_elapsedTime<finalTime)
    {
        updateFreqFromKey(&freq);
        updateAmpAndTremorFromKey(&amp);

        if(amp<0.001)
            amp=0;

        blowOneCycle(freq, amp);
    }

//--------------------------------------------------------------------------------------------------------------------------------------

#ifdef USE_CSV
    m_csvOut.close();
#endif //#ifdef USE_CSV

    if(m_verbose)
        cout <<"Whistle Generation Finished!" <<endl;
}


void Whistle::updateFreqFromKey(double* freq)
{
    if(m_lastFreqKey.value==-1) //First time entering
        (*freq) = m_freqKeysList[0].value;

    bool isNewValue = updateKeysInTime(&m_freqKeysList);

    double error = m_lastFreqKey.value-(*freq);
    double derivative;

    if(isNewValue)
    {
        m_PID_Integral = 0;
        derivative = 0;

        m_PID_curentKp = FREQ_Kp + randFrom0To1()*FREQ_Kp_RANGE;
        m_PID_curentKi = FREQ_Ki + randFrom0To1()*FREQ_Ki_RANGE;
        m_PID_curentKd = FREQ_Kd + randFrom0To1()*FREQ_Kd_RANGE;
    }
    else
    {
        m_PID_Integral += error;
        derivative = error - m_PID_lastError;
    }
    m_PID_lastError = error;

    (*freq) += error * m_PID_curentKp + m_PID_Integral * m_PID_curentKi + derivative * m_PID_curentKd;

#ifdef USE_CSV
    m_csvOut <<"," <<m_lastFreqKey.value <<"," <<(*freq) <<endl;
#endif //#ifdef USE_CSV
}

void Whistle::updateAmpAndTremorFromKey(double* amp)
{
    if(m_lastAmpKey.value==-1)
        (*amp) = m_ampKeysList[0].value;

    updateKeysInTime(&m_ampKeysList);
    updateKeysInTime(&m_tremorKeysList);

    (*amp) += (m_lastAmpKey.value-(*amp)) / AMP_Kp;
}

bool Whistle::updateKeysInTime(whistleKeysList_t* keysList)
{
    bool differentValue=false;
    if(keysList->size()>0)
    {
        if( m_elapsedTime >= (*keysList)[0].time )
        {
            if(keysList==&m_freqKeysList)
            {
                differentValue = m_lastFreqKey.value != (*keysList)[0].value;
                m_lastFreqKey = (*keysList)[0];
            }
            else if(keysList==&m_ampKeysList)
            {
                differentValue = m_lastAmpKey.value != (*keysList)[0].value;
                m_lastAmpKey = (*keysList)[0];
            }
            else if(keysList==&m_tremorKeysList)
            {
                differentValue = m_lastTremorKey.value != (*keysList)[0].value;
                m_lastTremorKey = (*keysList)[0];
            }
            else
            {
                cerr <<"Error: KeysList unrecongnized in updateKeysInTime(whistleKeysList_t* keysList)" <<endl;
                return false;
            }

            keysList->erase(keysList->begin());

            return differentValue;
        }
    }
    return false;
}

void Whistle::addImperfection(double* freq, double* amp)
{
    if(m_elapsedTime >= m_finalImperfectTremorTime)
    {
        m_imperfectTremorAmpDiff = (*amp) * randFrom0To1() * m_imperfectTremorDepth;
        m_initialImperfectTremorTime = m_elapsedTime;
        m_finalImperfectTremorTime = m_initialImperfectTremorTime + (m_imperfectTremorLengthMin + ( randFrom0To1() * m_imperfectTremorLengthRand ));
    }

    double imperfectTremorMean     = (*amp) - m_imperfectTremorAmpDiff/2,
           imperfectTremorProgress = (m_elapsedTime-m_initialImperfectTremorTime) / (m_finalImperfectTremorTime-m_initialImperfectTremorTime);

    double modAmp  = m_imperfectTremorAmpDiff * cos(2*M_PI * imperfectTremorProgress)/2 + imperfectTremorMean;
    double modFreq = (*freq) + (randFromMinus1To1() * m_imperfectTremorFreqShift) - ( ((*amp)-modAmp) * m_imperfectTremorFreqShift / m_imperfectTremorDepth );

    clamp(0,&modAmp,1);
    clampFreq(&modFreq);

    (*amp)  = modAmp;
    (*freq) = modFreq;
}


void Whistle::blowOneCycle(double freq, double amp)
{
    //double modAmpConst = 1 - (randFrom0To1() * m_imperfectAmplitudeDepth); //This is so all the whistles aren't the exact same amplitude

    double modFreq=freq,
           modAmp=amp;//*modAmpConst;

    addImperfection(&modFreq, &modAmp);

    oneCycle(modFreq, modAmp);
}


void Whistle::blowOneNote(double freq, double amp, double length)
{
    int    totalTremorTime   = 0,
           currentTremorTime = 0;
    double randomTremorAmp   = 0.0,
           modFreq           = freq + ( randFromMinus1To1() * m_imperfectTremorFreqShift),
           freqIncr          = (freq - modFreq) / (freq / m_fadeInTime),
           modAmp            = 0.0,
           modAmpConst       = 1 - (randFrom0To1() * m_imperfectAmplitudeDepth);

    for(int i=0;i<length*freq;i++)
    {
        double fadeAmp = 0,
               t = double(i)/freq;

        if(t<m_fadeInTime)
        {
            modFreq += freqIncr;
            fadeAmp = ( 1.0-cos(t*M_PI/m_fadeInTime) ) / 2;
        }
        else
        {
            modFreq=freq;
            if(t>double(length)-m_fadeOutTime)
                fadeAmp = ( 1.0-cos((t-length)*M_PI/m_fadeOutTime) ) / 2;
            else
                fadeAmp=1;
        }

        if(currentTremorTime<=0)
        {
            randomTremorAmp = amp - (randFrom0To1() * m_imperfectTremorDepth);

            //totalTremorTime is in cycle so we multiply by freq to converts from seconds to cycles
            totalTremorTime = m_imperfectTremorLengthMin*freq + ( randFrom0To1() * m_imperfectTremorLengthRand*freq );
            currentTremorTime = totalTremorTime;
        }
        else
            currentTremorTime--;

        // A cosine that oscillates between amp and randomTremorAmp
        modAmp  = ( amp-randomTremorAmp ) * cos( 2*M_PI * currentTremorTime / totalTremorTime )/2 + ( amp+randomTremorAmp )/2;
        modAmp *= fadeAmp * modAmpConst;

        oneCycle( modFreq-((amp-modAmp)*m_imperfectTremorFreqShift/m_imperfectTremorDepth), modAmp);
    }
}


int16_t Whistle::getSound(double freq, double amp, double sampleUnit)
{
    return ((m_maxVolume * amp * sin( (sampleUnit + m_offsetX*m_sampling) * 2*M_PI * freq / m_sampling ) )/*1st Harmonic*/ +
            (m_maxVolume * amp*m_secondHarmonicAmp * sin( (sampleUnit + m_offsetX*m_sampling) * 2*M_PI * 2*freq / m_sampling ) ))/*2nd Harmonic*/ / (1+m_secondHarmonicAmp);//normalized to amp
}


void Whistle::oneCycle(double freq, double amp)
{
    assertAndClampFrequency(&freq);
    assertAndClampAmplitude(&amp);

    int16_t tempSound;

    double sampleUnit=0;
    for(;sampleUnit<(1/freq-m_offsetX)*m_sampling;sampleUnit++)
    {
        tempSound  = getSound(freq, amp, sampleUnit);
        print(tempSound);
    }

    m_elapsedTime += sampleUnit / m_sampling;
    int16_t nextSound = getSound(freq, amp, sampleUnit);

    //Now we interpol or extrapol to find the time offset too much or missing
    double slope = double(nextSound-tempSound) / (1/double(m_sampling));
    m_offsetX = nextSound/slope;
}


void Whistle::playTone(double freq, double amp, double length)
{
    double rounded = round(length*freq,TIMEPRECISION);

    for(double i=0;i<rounded;i++)
    {
        oneCycle(freq,amp);
    }
}


void Whistle::pause(double length)
{
    for(double t=0;t<m_sampling*length;t++)
    {
        print(0);
    }
}

void Whistle::insertKey(double time, double value, whistleKeyInterpolation interpol, whistleKeysList_t* keyList)
{
    if(keyList == &m_freqKeysList)
        assertAndClampFrequency(&value);
    else if(keyList == &m_ampKeysList)
        assertAndClampAmplitude(&value);
    else if(keyList == &m_tremorKeysList)
        assertAndClampTremor(&value);
    else
    {
        cerr <<"Error: keyList can't be found!" <<endl;
        return;
    }


    if(keyList->size()==0)
    {
        keyList->push_back( {time,value,interpol} );

        if(m_verbose)
        {
            cout <<"Frequency Key #0 " <<formatKey((*keyList)[0]) <<" has been added!" <<endl
                 <<"0: " <<formatKey((*keyList)[0]) <<endl <<endl;
        }
    }
    else
    {
        for(whistleKeysList_t::reverse_iterator rit=keyList->rbegin(); rit!=keyList->rend(); rit++)
        {
            if(time >= rit->time)
            {
                whistleKeysList_t::iterator it = keyList->end();

                if(time == rit->time)
                    cerr <<"Warning: Frequency Key " <<formatKey(time,value,interpol) <<" conflicts with another key's time. This key is discarded." <<endl;
                else
                    it = keyList->insert(rit.base(), {time,value,interpol} );

                if(m_verbose)
                {
                    if(it!=keyList->end())
                        cout <<"Frequency Key #" <<distance(keyList->begin(),it) <<" " <<formatKey(time,value,interpol) <<" has been added!" <<endl;
                    else
                        cout <<"Frequency Key not added!" <<endl;
                    for(unsigned int i=0;i<keyList->size();i++)
                    {
                        cout <<i <<": " <<formatKey((*keyList)[i]) <<endl;
                    }
                    cout <<endl;
                }

                return;
            }
        }
    }
}


void Whistle::insertFrequencyKey(double time, double value, whistleKeyInterpolation interpol)
{
    insertKey(time,value,interpol,&m_freqKeysList);
}

void Whistle::insertAmplitudeKey(double time, double value, whistleKeyInterpolation interpol)
{
    insertKey(time,value,interpol,&m_ampKeysList);
}

void Whistle::insertTremorKey(double time, double value, whistleKeyInterpolation interpol)
{
    insertKey(time,value,interpol,&m_tremorKeysList);
}


void Whistle::activateWriteToFile(string path)
{
    if(m_writeToFile)
    {
        if(m_outputFile==NULL)
        {
            cerr <<"Error: NULL pointer in writeToFile(string path)." <<endl;
            return;
        }
        m_outputFile->close();
    }

    m_outputFile = new SoundFileWrite(path.c_str());
    m_writeToFile = true;
}

void Whistle::closeFile()
{
    if(m_writeToFile)
    {
        m_outputFile->close();
        m_writeToFile = false;
    }
}


void Whistle::print(int16_t soundData)
{
    if(m_addNoise)
    {
        int16_t noise = m_noiseAmp*randFromMinus1To1()*m_maxVolume;
        if( (noise >= 0 && int16_t(soundData + noise) >= soundData) ||
            (noise <= 0 && int16_t(soundData + noise) <= soundData) ) //prevents overflow
        {
            soundData += noise;
            clamp(-m_maxVolume, &soundData, m_maxVolume);
        }
    }

    if(m_writeToStream)
        cout <<(char)((soundData&0xff00)>>8) <<(char)(soundData&0xff);//Big-Endian

    if(m_writeToFile)
    {
        m_outputFile->writeSample16Bit(soundData);
    }
}




string Whistle::formatKey(const WhistleKey key) const
{
    return formatKey(key.time,key.value,key.interpol);
}

string Whistle::formatKey(const double time, const double value, const whistleKeyInterpolation interpol) const
{
    stringstream ss;
    ss <<"{ " <<time <<" ; " <<value <<" ; ";
    if(interpol==INTERPOLATION_STEP_UP)
        ss <<"Step Up";
    else if(interpol==INTERPOLATION_STEP_DOWN)
        ss <<"Step Down";
    else if(interpol==INTERPOLATION_LINEAR)
        ss <<"Linear";
    else
        ss <<"Unknown";
    ss <<" }";
    return ss.str();
}


int Whistle::assertAndClampFrequency(double* value) const
{
    if(value==NULL)
    {
        cerr <<"Error: NULL pointer in assertAndClampFrequency(double* value)" <<endl;
        return -1;
    }

    if(*value <= 0.0)
    {
        cerr <<"Error: Frequency \"" <<*value <<"\" can't be smaller or equal to 0Hz. Clipping..." <<endl;
        *value = 0.001;//Arbitrary
        return 1;
    }
    else if(*value > MAXFREQUENCY)
    {
        cerr <<"Error: Frequency \"" <<*value <<"\" can't be bigger than " <<MAXFREQUENCY <<"Hz. Clipping..." <<endl;
        *value = MAXFREQUENCY;
        return 2;
    }
    else
        return 0;
}

int Whistle::assertAndClampAmplitude(double* value) const
{
    if(value==NULL)
    {
        cerr <<"Error: NULL pointer in assertAndClampAmplitude(double* value)" <<endl;
        return -1;
    }

    if(*value < 0.0)
    {
        cerr <<"Error: Volume \"" <<*value <<"\" can't be smaller than 0. Clipping..." <<endl;
        *value = 0.0;
        return 1;
    }
    else if(*value > 1.0)
    {
        cerr <<"Error: Volume \"" <<*value <<"\" can't be bigger than 1. Clipping..." <<endl;
        *value = 1.0;
        return 2;
    }
    else
        return 0;
}

int Whistle::assertAndClampTremor(double* value) const
{
    if(value==NULL)
    {
        cerr <<"Error: NULL pointer in assertAndClampTremor(double* value)" <<endl;
        return -1;
    }

    if(*value < 0.0)
    {
        cerr <<"Error: Tremor \"" <<*value <<"\" can't be smaller than 0. Clipping..." <<endl;
        *value = 0.0;
        return 1;
    }
    else if(*value > 1.0)
    {
        cerr <<"Error: Tremor \"" <<*value <<"\" can't be bigger than 1. Clipping..." <<endl;
        *value = 1.0;
        return 2;
    }
    else
        return 0;
}


bool dividedByZero(double value)
{
    return value != value; //Might not be portable to other compiler.
}

double round(double valueToRound,int precision)
{
    return round( valueToRound * pow(10,precision) ) / pow(10,precision);
}

double randFrom0To1()
{
    return double( rand() % (RANDPOSSIBILITIES+1) ) / RANDPOSSIBILITIES;
}

double randFromMinus1To1()
{
    return ( double( rand() % (RANDPOSSIBILITIES*2+1) ) / RANDPOSSIBILITIES ) - 1;
}

void clamp(const double minimum, double* value, const double maximum)
{
    if(*value > maximum)
        *value = maximum;
    else if(*value < minimum)
        *value = minimum;
}

void clamp(const int16_t minimum, int16_t* value, const int16_t maximum)
{
    if(*value > maximum)
        *value = maximum;
    else if(*value < minimum)
        *value = minimum;
}

void clampFreq(double* freq)
{
    if(*freq > MAXFREQUENCY)
        *freq = MAXFREQUENCY;
    else if(*freq <= 0)
        *freq = 0.001; //Arbitrary
}




/*

void Whistle::blowOneNote(double freq, double amp, double length)
{
    int    totalTremorTime   = 0,
           currentTremorTime = 0;
    double randomTremorAmp   = 0.0,
           modFreq           = freq + ( randFromMinus1To1() * m_imperfectTremorFreqShift),
           freqIncr          = (freq - modFreq) / (freq / m_fadeInTime),
           modAmp            = 0.0,
           modAmpConst       = 1 - (randFrom0To1() * m_amplitudeDepth);

    for(int i=0;i<length*freq;i++)
    {
        double fadeAmp = 0,
               t = double(i)/freq;

        if(t<m_fadeInTime)
        {
            modFreq += freqIncr;
            fadeAmp = ( 1.0-cos(t*M_PI/m_fadeInTime) ) / 2;
        }
        else
        {
            modFreq=freq;
            if(t>double(length)-m_fadeOutTime)
                fadeAmp = ( 1.0-cos((t-length)*M_PI/m_fadeOutTime) ) / 2;
            else
                fadeAmp=1;
        }

        if(currentTremorTime<=0)
        {
            randomTremorAmp = amp - (randFrom0To1() * m_imperfectTremorDepth);

            //totalTremorTime is in cycle so we multiply by freq to converts from seconds to cycles
            totalTremorTime = m_imperfectTremorLengthMin*freq + ( randFrom0To1() * m_imperfectTremorLengthRand*freq );
            currentTremorTime = totalTremorTime;
        }
        else
            currentTremorTime--;

        // A cosine that oscillates between amp and randomTremorAmp
        modAmp  = ( amp-randomTremorAmp ) * cos( 2*M_PI * currentTremorTime / totalTremorTime )/2 + ( amp+randomTremorAmp )/2;
        modAmp *= fadeAmp * modAmpConst;

        oneCycle( modFreq-((amp-modAmp)*m_imperfectTremorFreqShift/m_imperfectTremorDepth), modAmp);
    }
}

*/

/*

SoundFileWrite soundfile("output/out.wav");

for (int i=0; i<1000; i++)
{
    soundfile.writeSample16Bit((int16_t)(i));
}

*/

/*

#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32) || defined(__WINDOWS__) || defined(__TOS_WIN__)

    #include <windows.h>

    void delay( unsigned long ms )
    {
        Sleep( ms );
    }

#else  // presume POSIX

    #include <unistd.h>

    void delay( unsigned long ms )
    {
        usleep( ms * 1000 );
    }

#endif

*/
