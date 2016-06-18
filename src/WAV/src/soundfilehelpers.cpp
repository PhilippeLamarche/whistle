//
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Sun Sep  8 11:29:58 PDT 2002
// Last Modified: Sun Sep  8 11:30:01 PDT 2002
// Filename:      soundfilehelpers.cpp
// Based On:      http://sig.sapp.org/src/sigSignal/soundfilehelpers.cpp
// Syntax:        C++
// Copyright:     Copyright 2002 Craig Stuart Sapp
//

#include "soundfilehelpers.h"

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
// linear16ToMulaw8
//
// reference: http://www.omnigroup.com/MailArchive/next-prog/1994/0042.html
//

#define BIAS 0x84 /* define the add-in bias for 16 bit samples */
#define CLIP 32635

uint16_t linear16ToMulaw8(int16_t aSample) {

   static int16_t exponentLookupTable[256] = {
      0,0,1,1,2,2,2,2,3,3,3,3,3,3,3,3,
      4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
      5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
      5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
      6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
      6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
      6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
      6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
      7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
      7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
      7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
      7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
      7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
      7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
      7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
      7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7
   };

   static int16_t sign, exponent, mantissa, magnitude;
   static uint16_t output;

   // put the sample into sign-magnitude form
   sign = (aSample >> 8) & 0x80;                  // set aside the sign
   magnitude = sign ? -aSample : aSample;         // get magnitude
   if (magnitude > CLIP) magnitude = CLIP;        // clip the magnitude

   // Convert from 16 bit linear to ulaw.
   magnitude += BIAS;
   exponent   = exponentLookupTable[(uint8_t)((magnitude >> 7) & 0xFF)];
   mantissa   = (magnitude >> (exponent + 3)) & 0x0F;
   output     = ~(sign | (exponent << 4) | mantissa);

   return output;
}



//////////////////////////////
//
// SampleTo8 -- converts a number in the range -1.0 to 1.0
//	to a number in the range -127 to 128.  If the
//      number is out of the input range then it will be limited to
//	the maximum in that direction (and not wrap around).
//

int8_t SampleTo8(double aSample) {
   static int8_t output;

   // prevent wrap around:
   if (aSample >= 1.0) {
      output = (int8_t)0x7f;
   } else if (aSample <= -1.0) {
      output = (int8_t)0x80;
   } else {
      output = (int8_t)(aSample * 0x7f + 0.5);
   }

   return output;
}


//////////////////////////////
//
// SampleTo16 -- converts a number in the range -1.0 to 1.0
//	to a number in the range -32768 to 32767.  If the number
//      is out of the input range then it will be limited to the
//	maximum in the direction (and not wrap around).
//

int16_t SampleTo16(double aSample) {
    int16_t output;

    if (aSample > 0) {
        output = (int16_t)(aSample * 0x8000 + 0.5);
    } else {
        output = (int16_t)(aSample * 0x8000 - 0.5);
    }


   // prevent wrap around:
   if (output > 32767) output = 32767;
   else if (output < -32768) output = -32768;

   return (int16_t)output;
}



//////////////////////////////
//
// SampleTo32 -- converts a number in the range -1.0 to 1.0
//	to a number in the range -2147483647 to 2147483648.  If the
//      number is out of the input range then it will be limited to
//	the maximum in that direction (and not wrap around).
//

int32_t SampleTo32(double aSample) {
   static int32_t output;

   // prevent wrap around:
   if (aSample >= 1.0) {
      output = (int32_t)0x80000000;
   } else if (aSample <= -1.0) {
      output = (int32_t)0x7fffffff;
   } else {
      output = (int32_t)(aSample * 0x7fffffff + 0.5);
   }

   return output;
}




//////////////////////////////
//
// soundWriteSample8M -- read channel samples as
//      8-bit Mulaw values
//

void soundWriteSample8M_B(FileIO& outFile, double aSample) {
   outFile.writeBigEndian(linear16ToMulaw8(SampleTo16(aSample)));
}


void soundWriteSample8M_L(FileIO& outFile, double aSample) {
   outFile.writeLittleEndian(linear16ToMulaw8(SampleTo16(aSample)));
}



//////////////////////////////
//
// soundWriteSample8L
//

void soundWriteSample8L_B(FileIO& outFile, double aSample) {
   outFile.writeBigEndian(SampleTo8(aSample));
}


void soundWriteSample8L_L(FileIO& outFile, double aSample) {
   outFile.writeBigEndian(SampleTo8(aSample));
}



//////////////////////////////
//
// soundWriteSample16L
//

void soundWriteSample16L_B(FileIO& outFile, double aSample) {
   outFile.writeBigEndian(SampleTo16(aSample));
}


void soundWriteSample16L_L(FileIO& outFile, double aSample) {
   outFile.writeLittleEndian(SampleTo16(aSample));
}



//////////////////////////////
//
// soundWriteSample24L
//

void soundWriteSample24L_B(FileIO& outFile, double aSample) {
   static int32_t output;
   output = SampleTo32(aSample);
   outFile.writeBigEndian((uint8_t)(output >> 12));
   outFile.writeBigEndian((uint8_t)(output >>  8));
   outFile.writeBigEndian((uint8_t)(output >>  4));
}


void soundWriteSample24L_L(FileIO& outFile, double aSample) {
   static int32_t output;
   output = SampleTo32(aSample);
   outFile.writeBigEndian((uint8_t)(output >>  4));
   outFile.writeBigEndian((uint8_t)(output >>  8));
   outFile.writeBigEndian((uint8_t)(output >> 12));
}



//////////////////////////////
//
// soundWriteSample32L
//

void soundWriteSample32L_B(FileIO& outFile, double aSample) {
   outFile.writeBigEndian(SampleTo32(aSample));
}


void soundWriteSample32L_L(FileIO& outFile, double aSample) {
   outFile.writeBigEndian(SampleTo32(aSample));
}



//////////////////////////////
//
// soundWriteSample32F
//

void soundWriteSample32F_B(FileIO& outFile, double aSample) {
   outFile.writeBigEndian((float)aSample);
}


void soundWriteSample32F_L(FileIO& outFile, double aSample) {
   outFile.writeLittleEndian((float)aSample);
}



//////////////////////////////
//
// soundWriteSample64F
//

void soundWriteSample64F_B(FileIO& outFile, double aSample) {
   outFile.writeBigEndian((double)aSample);
}


void soundWriteSample64F_L(FileIO& outFile, double aSample) {
   outFile.writeLittleEndian((double)aSample);
}


// md5sum: dbaa9c4c97b826ecf4682e55a8ddd3ee soundfilehelpers.cpp [20050403]
