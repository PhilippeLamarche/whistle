//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Copyright:     Copyright 2002 Craig Stuart Sapp
// Creation Date: Fri Jan 18 22:25:27 PST 2002
// Last Modified: Wed Jan 23 20:24:34 PST 2002
// Last Modified: Sat Feb  2 12:22:17 PST 2002 (fixed looping and goto)
// Filename:      SoundFileRead.cpp
// Based On:      http://sig.sapp.org/src/sigSignal/SoundFileIn.cpp
// Syntax:        C++
//

#include "SoundFileRead.h"

#include <stdlib.h>
#include <string.h>

#ifndef OLDCPP
   #include <iostream>
   using namespace std;
#else
   #include <iostream.h>
#endif


//////////////////////////////
//
// SoundFileRead::SoundFileRead --
//    defaults: aFileName = "", start = -1, dur = -1
//

SoundFileRead::SoundFileRead(const char* aFileName, double start, double dur) {
   filename = NULL;
   curIndex = 0;
   begIndex = 0;
   endIndex = 0;
   loopingQ = 0;
   endianType = 0;

   if (strlen(aFileName) > 0) {
      setFile(aFileName, start, dur);
      // set the size of the current array to the number of channels
   } else {
      current.setSize(4);
      current.allowGrowth(1);
      current.zero();
   }
}



//////////////////////////////
//
// SoundFileRead::~SoundFileRead --  What to do when destroying a
//     SoundFileRead object.
//

SoundFileRead::~SoundFileRead() {
   if (filename != NULL) {
      delete [] filename;
      filename = NULL;
   }
}



/////////////////////////////
//
// SoundFileRead::getCurrentSample16Bit -- return a 16 bit
//     version of the current channel sample.
//     default value: channel = 0
//

int SoundFileRead::getCurrentSample16Bit(int channel) {
   return (int)((int32_t)0x8000 * getCurrentSampleDouble(channel));
   // rounding necessary?
}



//////////////////////////////
//
// SoundFileRead::getNextSample16Bit -- increment the current sample array
//     and return the requested channel value.  Returns
//     default value: channel = 0
//

int SoundFileRead::getNextSample16Bit(int channel) {
   incrementSample();
   return getCurrentSample16Bit(channel);
}



/////////////////////////////
//
// SoundFileRead::getCurrentSample24Bit -- return a 24 bit
//     version of the current channel sample.
//     default value: channel = 0
//

int SoundFileRead::getCurrentSample24Bit(int channel) {
   return (int)((int32_t)0x800000 * getCurrentSampleDouble(channel));
}



//////////////////////////////
//
// SoundFileRead::getNextSample24Bit -- increment the current sample array
//     and return the requested channel value.  Returns
//     default value: channel = 0
//

int SoundFileRead::getNextSample24Bit(int channel) {
   incrementSample();
   return getCurrentSample24Bit(channel);
}



/////////////////////////////
//
// SoundFileRead::getCurrentSampleDouble -- return a 16 bit
//     version of the current channel sample.
//     default value: channel = 0
//

double SoundFileRead::getCurrentSampleDouble(int channel) {
   return current[channel];
}



//////////////////////////////
//
// SoundFileRead::getNextSampleDouble -- increment the current sample
//     array and return the requested channel value.  Returns
//     default value: channel = 0
//

double SoundFileRead::getNextSampleDouble(int channel) {
   incrementSample();
   return getCurrentSampleDouble(channel);
}



//////////////////////////////
//
// SoundFileRead::getCurrentSampleIndex -- return the sample index of
//     the current sample data.
//

int SoundFileRead::getCurrentSampleIndex(void) {
   return curIndex;
}



//////////////////////////////
//
// SoundFileRead::incrementSample -- read the next sample set and
//     store in current array.
//

void SoundFileRead::incrementSample(void) {
   curIndex++;

   if (curIndex > endIndex && loopingQ) {
      gotoSample(begIndex);
      return;
   } else if (curIndex < 0 || curIndex > endIndex) {
      for (int i=0; i<getChannels(); i++) {
         current[i] = 0;
      }
      return;
   }

   fillCurrentArray();
}



//////////////////////////////
//
// SoundFileRead::getFilename --
//

const char* SoundFileRead::getFilename(void) {
   if (filename == NULL) {
      return "";
   } else {
      return filename;
   }
}



//////////////////////////////
//
// SoundFileRead::setStartAndDuration --
//

void SoundFileRead::setStartAndDuration(double start, double dur) {
   // determine the beginning and ending samples according to start and dur
   int maxIndex = getSamples() - 1;
   if (maxIndex < 0) {
      cerr << "Error: sound file " << filename << " has no data!" << endl;
      exit(1);
   }
   if (start >= 0) {
      begIndex = (int32_t)(start * getSrate());
   } else {
      begIndex = 0;
   }
   if (begIndex > maxIndex) begIndex = 0;
   if (dur >= 0) {
      endIndex = begIndex + (int32_t)(dur * getSrate());
   } else {
      endIndex = maxIndex;
   }
   if (endIndex > maxIndex) {
      endIndex = maxIndex;
   }

}



//////////////////////////////
//
// SoundFileRead::reset --  Go to the starting point in the file,
//     either the start of the file, or the start specified in
//     the setFile() function.
//     default: sampleIndex = -1
//

void SoundFileRead::reset(int sampleIndex) {
   if (sampleIndex < 0) {
      gotoSample(begIndex);
   } else {
      gotoSample(sampleIndex);
   }
}



//////////////////////////////
//
// SoundFileRead::gotoSample -- Move to the current sample position
//     in the soundfile and start reading from that point.
//

void SoundFileRead::gotoSample(int sampleIndex) {
   int offset = 0;
   if (sampleIndex < 0) {
      offset = getDataByteOffset();
      sndfile.seekg(offset); // used to be: sndfile.seekp(offset);
      sampleIndex = 0;
   } else {
      offset = (int)(getDataByteOffset() +
         (sampleIndex + begIndex) * getChannels() *
         getBitsPerSample() / 8.0);
      sndfile.seekg(offset);  // used to be: sndfile.seekp(offset);
   }

   curIndex = sampleIndex;
   fillCurrentArray();
}



//////////////////////////////
//
// SoundFileRead::setFile -- start reading a soundfile at the current
//     start time and for the given duration.
//

void SoundFileRead::setFile(const char* aFileName, double start, double dur) {
   if (filename != NULL) delete [] filename;
   filename = new char[strlen(aFileName)+1];
   strcpy(filename, aFileName);

   setHeader(filename);

   if (sndfile.is_open()) sndfile.close();
   sndfile.open(filename, ios::in | ios::binary);
   if(!sndfile.is_open()) {
      cerr << "Error: sound file " << filename
           << " could not be opened!" << endl;
      exit(1);
   }

   setStartAndDuration(start, dur);
   determineReadingFunction();

   // set the file to the current sample
   gotoSample(begIndex);
}



//////////////////////////////
//
// SoundFileRead::setLoopOption -- turn looping on or off
// SoundFileRead::setLoopOn     -- turn looping on
// SoundFileRead::setLoopOff    -- turn looping off
//

void SoundFileRead::setLoopOption(int mode) {
   if (mode == 0) {
      loopingQ = 0;
   } else {
      loopingQ = 1;
   }
}


void SoundFileRead::setLoopOn(void)  { setLoopOption(1); }
void SoundFileRead::setLoopOff(void) { setLoopOption(0); }



///////////////////////////////////////////////////////////////////////////
//
// private functions
//

// The following functions are used by the readSample function.
// These functions have to be outside of the SoundFileRead class
// because Microsoft's Visual C++ 5.0 compiler cannot handle pointers
// to class member functions.

// big endian samples
double         soundReadSample8M_B         (FileIO& soundFile);
double         soundReadSample8L_B         (FileIO& soundFile);
double         soundReadSample16L_B        (FileIO& soundFile);
double         soundReadSample24L_B        (FileIO& soundFile);
double         soundReadSample32L_B        (FileIO& soundFile);
double         soundReadSample32F_B        (FileIO& soundFile);
double         soundReadSample64F_B        (FileIO& soundFile);

// little endian samples
double        soundReadSample8M_L          (FileIO& soundFile);
double        soundReadSample8L_L          (FileIO& soundFile);
double        soundReadSample16L_L         (FileIO& soundFile);
double        soundReadSample24L_L         (FileIO& soundFile);
double        soundReadSample32L_L         (FileIO& soundFile);
double        soundReadSample32F_L         (FileIO& soundFile);
double        soundReadSample64F_L         (FileIO& soundFile);




//////////////////////////////
//
// SoundFileRead::determineSampleReadingFunction --
//

void SoundFileRead::determineReadingFunction(void) {

   switch (getInputType()) {
      case TYPE_SND:                        // big endian samples
         endianType = SIG_BIG_ENDIAN;
         switch (getNextFormat()) {
            case SND_FORMAT_MULAW_8:
               readSample = soundReadSample8M_B;
               break;
            case SND_FORMAT_LINEAR_8:
               readSample = soundReadSample8L_B;
               break;
            case SND_FORMAT_LINEAR_16:
               readSample = soundReadSample16L_B;
               break;
            case SND_FORMAT_LINEAR_24:
               readSample = soundReadSample24L_B;
               break;
            case SND_FORMAT_LINEAR_32:
               readSample = soundReadSample32L_B;
               break;
            case SND_FORMAT_FLOAT:
               readSample = soundReadSample32F_B;
               break;
            case SND_FORMAT_DOUBLE:
               readSample = soundReadSample64F_B;
               break;
            default:
               cerr << "Error: unknown input soundfile format: "
                    << getNextFormat() << endl;
               exit(1);
         }
         break;
      case TYPE_WAV_PCM:                    // little endian samples
         endianType = SIG_LITTLE_ENDIAN;
         switch (getNextFormat()) {
            case SND_FORMAT_MULAW_8:
               readSample = soundReadSample8M_L;
               break;
            case SND_FORMAT_LINEAR_8:
               readSample = soundReadSample8L_L;
               break;
            case SND_FORMAT_LINEAR_16:
               readSample = soundReadSample16L_L;
               break;
            case SND_FORMAT_LINEAR_24:
               readSample = soundReadSample24L_L;
               break;
            case SND_FORMAT_LINEAR_32:
               readSample = soundReadSample32L_L;
               break;
            case SND_FORMAT_FLOAT:
               readSample = soundReadSample32F_L;
               break;
            case SND_FORMAT_DOUBLE:
               readSample = soundReadSample64F_L;
               break;
            default:
               cerr << "Error: unknown input soundfile format: "
                    << getNextFormat() << endl;
               exit(1);
         }
         break;
      default: ;
         // the soundfile name is probably a dummy, and will not be used
         // cerr << "Error: unknown soundfile type: "
         //      << getInputType() << endl;
         // exit(1);
   }
}



//////////////////////////////
//
// SoundFileRead::fillCurrentArray -- read the next set of samples
//     from the soundfile and store in the current array.
//

void SoundFileRead::fillCurrentArray(void) {
   for (int i=0; i<getChannels(); i++) {
      current[i] = readSample(sndfile);
   }
}



///////////////////////////////////////////////////////////////////////////
//
// Helper functions which are intended to be private
// These functions have to be outside of the SoundFileRead class
// because Microsoft's Visual C++ 5.0 compiler cannot handle pointers
// to class member functions.
//



//////////////////////////////
//
// mulaw8ToLinear16 -- converts 8-bit mulaw to
//	16-bit linear quantization
//

int16_t mulaw8ToLinear16(uint8_t index) {

   static const int16_t MuLaw[256] = {
   // negative values
    (int16_t)0x8284, (int16_t)0x8684, (int16_t)0x8a84, (int16_t)0x8e84,
    (int16_t)0x9284, (int16_t)0x9684, (int16_t)0x9a84, (int16_t)0x9e84,
    (int16_t)0xa284, (int16_t)0xa684, (int16_t)0xaa84, (int16_t)0xae84,
    (int16_t)0xb284, (int16_t)0xb684, (int16_t)0xba84, (int16_t)0xbe84,
    (int16_t)0xc184, (int16_t)0xc384, (int16_t)0xc584, (int16_t)0xc784,
    (int16_t)0xc984, (int16_t)0xcb84, (int16_t)0xcd84, (int16_t)0xcf84,
    (int16_t)0xd184, (int16_t)0xd384, (int16_t)0xd584, (int16_t)0xd784,
    (int16_t)0xd984, (int16_t)0xdb84, (int16_t)0xdd84, (int16_t)0xdf84,
    (int16_t)0xe104, (int16_t)0xe204, (int16_t)0xe304, (int16_t)0xe404,
    (int16_t)0xe504, (int16_t)0xe604, (int16_t)0xe704, (int16_t)0xe804,
    (int16_t)0xe904, (int16_t)0xea04, (int16_t)0xeb04, (int16_t)0xec04,
    (int16_t)0xed04, (int16_t)0xee04, (int16_t)0xef04, (int16_t)0xf004,
    (int16_t)0xf0c4, (int16_t)0xf144, (int16_t)0xf1c4, (int16_t)0xf244,
    (int16_t)0xf2c4, (int16_t)0xf344, (int16_t)0xf3c4, (int16_t)0xf444,
    (int16_t)0xf4c4, (int16_t)0xf544, (int16_t)0xf5c4, (int16_t)0xf644,
    (int16_t)0xf6c4, (int16_t)0xf744, (int16_t)0xf7c4, (int16_t)0xf844,
    (int16_t)0xf8a4, (int16_t)0xf8e4, (int16_t)0xf924, (int16_t)0xf964,
    (int16_t)0xf9a4, (int16_t)0xf9e4, (int16_t)0xfa24, (int16_t)0xfa64,
    (int16_t)0xfaa4, (int16_t)0xfae4, (int16_t)0xfb24, (int16_t)0xfb64,
    (int16_t)0xfba4, (int16_t)0xfbe4, (int16_t)0xfc24, (int16_t)0xfc64,
    (int16_t)0xfc94, (int16_t)0xfcb4, (int16_t)0xfcd4, (int16_t)0xfcf4,
    (int16_t)0xfd14, (int16_t)0xfd34, (int16_t)0xfd54, (int16_t)0xfd74,
    (int16_t)0xfd94, (int16_t)0xfdb4, (int16_t)0xfdd4, (int16_t)0xfdf4,
    (int16_t)0xfe14, (int16_t)0xfe34, (int16_t)0xfe54, (int16_t)0xfe74,
    (int16_t)0xfe8c, (int16_t)0xfe9c, (int16_t)0xfeac, (int16_t)0xfebc,
    (int16_t)0xfecc, (int16_t)0xfedc, (int16_t)0xfeec, (int16_t)0xfefc,
    (int16_t)0xff0c, (int16_t)0xff1c, (int16_t)0xff2c, (int16_t)0xff3c,
    (int16_t)0xff4c, (int16_t)0xff5c, (int16_t)0xff6c, (int16_t)0xff7c,
    (int16_t)0xff88, (int16_t)0xff90, (int16_t)0xff98, (int16_t)0xffa0,
    (int16_t)0xffa8, (int16_t)0xffb0, (int16_t)0xffb8, (int16_t)0xffc0,
    (int16_t)0xffc8, (int16_t)0xffd0, (int16_t)0xffd8, (int16_t)0xffe0,
    (int16_t)0xffe8, (int16_t)0xfff0, (int16_t)0xfff8, (int16_t)0x0000,
   // positive values
    (int16_t)0x7d7c, (int16_t)0x797c, (int16_t)0x757c, (int16_t)0x717c,
    (int16_t)0x6d7c, (int16_t)0x697c, (int16_t)0x657c, (int16_t)0x617c,
    (int16_t)0x5d7c, (int16_t)0x597c, (int16_t)0x557c, (int16_t)0x517c,
    (int16_t)0x4d7c, (int16_t)0x497c, (int16_t)0x457c, (int16_t)0x417c,
    (int16_t)0x3e7c, (int16_t)0x3c7c, (int16_t)0x3a7c, (int16_t)0x387c,
    (int16_t)0x367c, (int16_t)0x347c, (int16_t)0x327c, (int16_t)0x307c,
    (int16_t)0x2e7c, (int16_t)0x2c7c, (int16_t)0x2a7c, (int16_t)0x287c,
    (int16_t)0x267c, (int16_t)0x247c, (int16_t)0x227c, (int16_t)0x207c,
    (int16_t)0x1efc, (int16_t)0x1dfc, (int16_t)0x1cfc, (int16_t)0x1bfc,
    (int16_t)0x1afc, (int16_t)0x19fc, (int16_t)0x18fc, (int16_t)0x17fc,
    (int16_t)0x16fc, (int16_t)0x15fc, (int16_t)0x14fc, (int16_t)0x13fc,
    (int16_t)0x12fc, (int16_t)0x11fc, (int16_t)0x10fc, (int16_t)0x0ffc,
    (int16_t)0x0f3c, (int16_t)0x0ebc, (int16_t)0x0e3c, (int16_t)0x0dbc,
    (int16_t)0x0d3c, (int16_t)0x0cbc, (int16_t)0x0c3c, (int16_t)0x0bbc,
    (int16_t)0x0b3c, (int16_t)0x0abc, (int16_t)0x0a3c, (int16_t)0x09bc,
    (int16_t)0x093c, (int16_t)0x08bc, (int16_t)0x083c, (int16_t)0x07bc,
    (int16_t)0x075c, (int16_t)0x071c, (int16_t)0x06dc, (int16_t)0x069c,
    (int16_t)0x065c, (int16_t)0x061c, (int16_t)0x05dc, (int16_t)0x059c,
    (int16_t)0x055c, (int16_t)0x051c, (int16_t)0x04dc, (int16_t)0x049c,
    (int16_t)0x045c, (int16_t)0x041c, (int16_t)0x03dc, (int16_t)0x039c,
    (int16_t)0x036c, (int16_t)0x034c, (int16_t)0x032c, (int16_t)0x030c,
    (int16_t)0x02ec, (int16_t)0x02cc, (int16_t)0x02ac, (int16_t)0x028c,
    (int16_t)0x026c, (int16_t)0x024c, (int16_t)0x022c, (int16_t)0x020c,
    (int16_t)0x01ec, (int16_t)0x01cc, (int16_t)0x01ac, (int16_t)0x018c,
    (int16_t)0x0174, (int16_t)0x0164, (int16_t)0x0154, (int16_t)0x0144,
    (int16_t)0x0134, (int16_t)0x0124, (int16_t)0x0114, (int16_t)0x0104,
    (int16_t)0x00f4, (int16_t)0x00e4, (int16_t)0x00d4, (int16_t)0x00c4,
    (int16_t)0x00b4, (int16_t)0x00a4, (int16_t)0x0094, (int16_t)0x0084,
    (int16_t)0x0078, (int16_t)0x0070, (int16_t)0x0068, (int16_t)0x0060,
    (int16_t)0x0058, (int16_t)0x0050, (int16_t)0x0048, (int16_t)0x0040,
    (int16_t)0x0038, (int16_t)0x0030, (int16_t)0x0028, (int16_t)0x0020,
    (int16_t)0x0018, (int16_t)0x0010, (int16_t)0x0008, (int16_t)0x0000
   };

   return MuLaw[index];
}



//////////////////////////////
//
// soundReadSample8M -- read channel samples as
//	8-bit Mulaw values.  Both of the following
//	functions do exactly the same thing.
//

double soundReadSample8M_B(FileIO& soundfile) {
   uint8_t sample;
   soundfile.readBigEndian(sample);
   return mulaw8ToLinear16(sample) / (int32_t)0x8000;
}


double soundReadSample8M_L(FileIO& soundfile) {
   uint8_t sample;
   soundfile.readLittleEndian(sample);
   return mulaw8ToLinear16(sample) / (int32_t)0x8000;
}



//////////////////////////////
//
// soundReadSample8L -- both functions do exactly the same thing,
//     which is to read an 8-bit linear sound code.
//

double soundReadSample8L_B(FileIO& soundfile) {
   int8_t sample;
   soundfile.readBigEndian(sample);
   return (double)sample / (int32_t)0x80;
}

double soundReadSample8L_L(FileIO& soundfile) {
   int8_t sample;
   soundfile.readLittleEndian(sample);
   return (double)sample / (int32_t)0x80;
}



//////////////////////////////
//
// soundReadSample16L -- Read samples as 16 bit linear int values.
//

double soundReadSample16L_B(FileIO& soundfile) {
   int16_t sample;
   soundfile.readBigEndian(sample);
   return (double)sample / (int32_t)0x8000;
}


double soundReadSample16L_L(FileIO& soundfile) {
   int16_t sample;
   soundfile.readLittleEndian(sample);
   return (double)sample / (int32_t)0x8000;
}



//////////////////////////////
//
// soundReadSample24L -- Read samples as 24 bit linear long int values.
//

double soundReadSample24L_B(FileIO& soundfile) {
   unsigned char temp[3];
   int32_t sample;
   soundfile.readLittleEndian(temp[0]);
   soundfile.readLittleEndian(temp[1]);
   soundfile.readLittleEndian(temp[2]);
   sample = temp[0];
   sample = (sample << 8) | temp[1];
   sample = (sample << 8) | temp[2];
   if (temp[0] & 0x80) {
      sample = sample | 0xff000000;
   }
   return (double)sample / (int32_t)0x800000;
}


double soundReadSample24L_L(FileIO& soundfile) {
   unsigned char temp[3];
   int32_t sample;
   soundfile.readLittleEndian(temp[2]);
   soundfile.readLittleEndian(temp[1]);
   soundfile.readLittleEndian(temp[0]);
   sample = temp[0];
   sample = (sample << 8) | temp[1];
   sample = (sample << 8) | temp[2];
   if (temp[0] & 0x80) {
      sample = sample | 0xff000000;
   }
   return (double)sample / (int32_t)0x800000;
}



//////////////////////////////
//
// soundReadSample32L -- Read samples as 32 bit linear long int values.
//

double soundReadSample32L_B(FileIO& soundfile) {
   int32_t sample;
   soundfile.readBigEndian(sample);
   return (double)sample / (uint32_t)0x80000000;
}


double soundReadSample32L_L(FileIO& soundfile) {
   int32_t sample;
   soundfile.readLittleEndian(sample);
   return (double)sample / (uint32_t)0x80000000;
}



//////////////////////////////
//
// soundReadSample32F -- Read samples a float values.
//

double soundReadSample32F_B(FileIO& soundfile) {
   float sample;
   soundfile.readBigEndian(sample);
   return (double)sample;
}


double soundReadSample32F_L(FileIO& soundfile) {
   float sample;
   soundfile.readLittleEndian(sample);
   return (double)sample;
}



//////////////////////////////
//
// soundReadSample64F -- Read samples as double float values.
//


double soundReadSample64F_B(FileIO& soundfile) {
   double sample;
   soundfile.readBigEndian(sample);
   return sample;
}


double soundReadSample64F_L(FileIO& soundfile) {
   double sample;
   soundfile.readLittleEndian(sample);
   return sample;
}



// md5sum: b5400cc45a110b33a3aae40cae759522 SoundFileRead.cpp [20050403]
