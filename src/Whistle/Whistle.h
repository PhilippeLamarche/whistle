#ifndef WHISTLE_H
#define WHISTLE_H

#include <stdlib.h>
#include <vector>

#include "soundfile.h" //To save .wav



enum whistleKeyInterpolation{INTERPOLATION_STEP_UP,INTERPOLATION_STEP_DOWN,INTERPOLATION_LINEAR};

struct WhistleKey
{
    double                  time ;
    double                  value;
    whistleKeyInterpolation interpol;
};

typedef std::vector< WhistleKey > whistleKeysList_t;



class Whistle
{
    public:
        Whistle();
        Whistle(const double imperfectTremorFreqShift, const double imperfectTremorLengthMin, const double imperfectTremorLengthRand, const double imperfectTremorDepth,
                const double imperfectAmplitudeDepth, const double fadeInTime, const double fadeOutTime);


        void generateWhistle();
        void blowOneCycle(double freq, double amp);
        void blowOneNote(double freq, double amp, double length);
        void pause(double length);

        void insertFrequencyKey(double time, double value, whistleKeyInterpolation interpolationType);
        void insertAmplitudeKey(double time, double value, whistleKeyInterpolation interpolationType);
        void insertTremorKey   (double time, double value, whistleKeyInterpolation interpolationType);

        void activateWriteToFile(std::string path); //Should be called before generateWhistle()
        void closeFile();

        inline bool addNoise() const { return m_addNoise; }
        inline void addNoise(const bool setter) { m_addNoise = setter; }
        inline bool writeToStream() const { return m_writeToStream; }
        inline void writeToStream(const bool setter) { m_writeToStream = setter; }
        inline bool verbose() const { return m_verbose; }
        inline void verbose(const bool setter) { m_verbose = setter; }

    private:
        void addImperfection(double* freq, double* amp);
        void updateFreqFromKey(double* freq);
        void updateAmpAndTremorFromKey(double* amp);

        void oneCycle(double freq, double amp);
        void playTone(double freq, double amp, double length);
        void print(int16_t a);

        void insertKey(double time, double value, whistleKeyInterpolation interpolationType, whistleKeysList_t* keyList);// time is in milliseconds
        void updateKeysInTime(whistleKeysList_t* keyList);

        int assertAndClampFrequency(double* value) const;
        int assertAndClampAmplitude(double* value) const;
        int assertAndClampTremor   (double* value) const;

        std::string formatKey(const WhistleKey key) const;
        std::string formatKey(const double time, const double value, const whistleKeyInterpolation interpol) const;// time is in milliseconds


    private:
        double m_imperfectTremorFreqShift ; // in Hz (maybe not ideal. could be in %Hz, needs verification)
        double m_imperfectTremorLengthMin ; // in seconds
        double m_imperfectTremorLengthRand; // in seconds
        double m_imperfectTremorDepth     ; // between 0 and 1

        double m_imperfectAmplitudeDepth  ; // between 0 and 1

        double m_fadeInTime               ; // in seconds
        double m_fadeOutTime              ; // in seconds

        bool   m_addNoise                 ;
        double m_noiseAmp                 ; // between 0 and 1

        unsigned int m_sampling ;
        int16_t      m_maxVolume;

        whistleKeysList_t m_freqKeysList  ;
        whistleKeysList_t m_ampKeysList   ;
        whistleKeysList_t m_tremorKeysList;

        SoundFileWrite* m_outputFile;
        bool m_writeToFile  ;
        bool m_writeToStream;

        bool m_verbose;

//  internal:
        double m_elapsedTime; // in microseconds
        double m_offsetY    ; // between 0 and 1 This holds the last value that has been printed. It enables the cycles to be perfectly synced

        double m_initialImperfectTremorTime; //This holds the time from when an imperfect tremor (unplanned) is happenning
        double m_finalImperfectTremorTime  ; //This holds the time until when an imperfect tremor should happen
        double m_imperfectTremorAmpDiff    ; //This holds the amount of imperfection in the current imperfect tremor

        WhistleKey m_lastFreqKey  ;
        WhistleKey m_lastAmpKey   ;
        WhistleKey m_lastTremorKey;
};


#endif // WHISTLE_H
