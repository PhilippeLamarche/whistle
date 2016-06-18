//
// Copyright 1997 by Craig Stuart Sapp, All Rights Reserved.
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri May  9 22:30:32 PDT 1997
// Last Modified: Sun Dec 14 03:29:39 GMT-0800 1997
// Filename:      ...sig/maint/code/sigBase/FileIO.cpp
// Web Address:   http://sig.sapp.org/src/sigBase/FileIO.cpp
// Documentation: http://sig.sapp.org/doc/classes/FileIO
// Syntax:        C++
//
// Description:   Derived from the fstream class, this class has
//                functions which allow writing binary files in
//                both little and big endian formats.  Useful for
//                writing files such as soundfiles and MIDI files
//                which require numbers to be stored in a particular
//                endian format.
//

#include "sigConfiguration.h"

#include "FileIO.h"

#ifndef OLDCPP
   using namespace std;
   #include <iostream>
#endif

#define OTHEREND



//////////////////////////////
//
// FileIO::FileIO --
//

FileIO::FileIO(void) {
   // do nothing
};

#ifndef OLDCPP
   FileIO::FileIO(const char* filename, ios::openmode state) :
#else
   FileIO::FileIO(const char* filename, int state) :
#endif

   fstream(filename, state | ios::binary) {
   // do nothing
};



//////////////////////////////
//
// FileIO::~FileIO --
//

FileIO::~FileIO() {
   // do nothing
}



//////////////////////////////
//
// FileIO::readBigEndian --
//	Read numbers from a file as big endian
//

void FileIO::readBigEndian(int8_t& aNumber) {
   #ifdef OTHEREND
      readNotMachineEndian(aNumber);
   #else
      readMachineEndian(aNumber);
   #endif
}

void FileIO::readBigEndian(uint8_t& aNumber) {
   #ifdef OTHEREND
      readNotMachineEndian(aNumber);
   #else
      readMachineEndian(aNumber);
   #endif
}

void FileIO::readBigEndian(int16_t& aNumber) {
   #ifdef OTHEREND
      readNotMachineEndian(aNumber);
   #else
      readMachineEndian(aNumber);
   #endif
}

void FileIO::readBigEndian(uint16_t& aNumber) {
   #ifdef OTHEREND
      readNotMachineEndian(aNumber);
   #else
      readMachineEndian(aNumber);
   #endif
}

void FileIO::readBigEndian(int32_t& aNumber) {
   #ifdef OTHEREND
      readNotMachineEndian(aNumber);
   #else
      readMachineEndian(aNumber);
   #endif
}

void FileIO::readBigEndian(uint32_t& aNumber) {
   #ifdef OTHEREND
      readNotMachineEndian(aNumber);
   #else
      readMachineEndian(aNumber);
   #endif
}

/*
void FileIO::readBigEndian(int& aNumber) {
   #ifdef OTHEREND
      readNotMachineEndian(aNumber);
   #else
      readMachineEndian(aNumber);
   #endif
}

void FileIO::readBigEndian(uint16_t& aNumber) {
   #ifdef OTHEREND
      readNotMachineEndian(aNumber);
   #else
      readMachineEndian(aNumber);
   #endif
}
*/

void FileIO::readBigEndian(float& aNumber) {
   #ifdef OTHEREND
      readNotMachineEndian(aNumber);
   #else
      readMachineEndian(aNumber);
   #endif
}

void FileIO::readBigEndian(double& aNumber) {
   #ifdef OTHEREND
      readNotMachineEndian(aNumber);
   #else
      readMachineEndian(aNumber);
   #endif
}



//////////////////////////////
//
// FileIO::readLittleEndian --
//	Read numbers from a file as little endian
//

void FileIO::readLittleEndian(int8_t& aNumber) {
   #ifdef OTHEREND
      readMachineEndian(aNumber);
   #else
      readNotMachineEndian(aNumber);
   #endif
}

void FileIO::readLittleEndian(uint8_t& aNumber) {
   #ifdef OTHEREND
      readMachineEndian(aNumber);
   #else
      readNotMachineEndian(aNumber);
   #endif
}

void FileIO::readLittleEndian(int16_t& aNumber) {
   #ifdef OTHEREND
      readMachineEndian(aNumber);
   #else
      readNotMachineEndian(aNumber);
   #endif
}

void FileIO::readLittleEndian(uint16_t& aNumber) {
   #ifdef OTHEREND
      readMachineEndian(aNumber);
   #else
      readNotMachineEndian(aNumber);
   #endif
}

void FileIO::readLittleEndian(int32_t& aNumber) {
   #ifdef OTHEREND
      readMachineEndian(aNumber);
   #else
      readNotMachineEndian(aNumber);
   #endif
}

void FileIO::readLittleEndian(uint32_t& aNumber) {
   #ifdef OTHEREND
      readMachineEndian(aNumber);
   #else
      readNotMachineEndian(aNumber);
   #endif
}

/*
void FileIO::readLittleEndian(int& aNumber) {
   #ifdef OTHEREND
      readMachineEndian(aNumber);
   #else
      readNotMachineEndian(aNumber);
   #endif
}

void FileIO::readLittleEndian(uint16_t& aNumber) {
   #ifdef OTHEREND
      readMachineEndian(aNumber);
   #else
      readNotMachineEndian(aNumber);
   #endif
}
*/

void FileIO::readLittleEndian(float& aNumber) {
   #ifdef OTHEREND
      readMachineEndian(aNumber);
   #else
      readNotMachineEndian(aNumber);
   #endif
}

void FileIO::readLittleEndian(double& aNumber) {
   #ifdef OTHEREND
      readMachineEndian(aNumber);
   #else
      readNotMachineEndian(aNumber);
   #endif
}



//////////////////////////////
//
// FileIO::readMachineEndian --
//	Read numbers from a file in the same endian as the computer.
//

void FileIO::readMachineEndian(int8_t& aNumber) {
   this->read((char*)(&aNumber), sizeof(aNumber));
}

void FileIO::readMachineEndian(uint8_t& aNumber) {
   this->read((char*)(&aNumber), sizeof(aNumber));
}

void FileIO::readMachineEndian(int16_t& aNumber) {
   this->read((char*)&aNumber, sizeof(aNumber));
}

void FileIO::readMachineEndian(uint16_t& aNumber) {
   this->read((char*)&aNumber, sizeof(aNumber));
}

void FileIO::readMachineEndian(int32_t& aNumber) {
   this->read((char*)&aNumber, sizeof(aNumber));
}

void FileIO::readMachineEndian(uint32_t& aNumber) {
   this->read((char*)&aNumber, sizeof(aNumber));
}

/*
void FileIO::readMachineEndian(int& aNumber) {
   this->read((char*)&aNumber, sizeof(aNumber));
}

void FileIO::readMachineEndian(uint16_t& aNumber) {
   this->read((char*)&aNumber, sizeof(aNumber));
}
*/

void FileIO::readMachineEndian(float& aNumber) {
   this->read((char*)&aNumber, sizeof(aNumber));
}

void FileIO::readMachineEndian(double& aNumber) {
   this->read((char*)&aNumber, sizeof(aNumber));
}



//////////////////////////////
//
// FileIO::readNotMachineEndian --
//	Read numbers from a file with different endian from the computer.
//

void FileIO::readNotMachineEndian(int8_t& aNumber) {
   this->read((char*)&aNumber, sizeof(aNumber));
   aNumber = flipBytes(aNumber);
}

void FileIO::readNotMachineEndian(uint8_t& aNumber) {
   this->read((char*)&aNumber, sizeof(aNumber));
   aNumber = flipBytes(aNumber);
}

void FileIO::readNotMachineEndian(int16_t& aNumber) {
   this->read((char*)&aNumber, sizeof(aNumber));
   aNumber = flipBytes(aNumber);
}

void FileIO::readNotMachineEndian(uint16_t& aNumber) {
   this->read((char*)&aNumber, sizeof(aNumber));
   aNumber = flipBytes(aNumber);
}

void FileIO::readNotMachineEndian(int32_t& aNumber) {
   this->read((char*)&aNumber, sizeof(aNumber));
   aNumber = flipBytes(aNumber);
}

void FileIO::readNotMachineEndian(uint32_t& aNumber) {
   this->read((char*)&aNumber, sizeof(aNumber));
   aNumber = flipBytes(aNumber);
}

/*
void FileIO::readNotMachineEndian(int& aNumber) {
   this->read((char*)&aNumber, sizeof(aNumber));
   aNumber = flipBytes(aNumber);
}

void FileIO::readNotMachineEndian(uint16_t& aNumber) {
   this->read((char*)&aNumber, sizeof(aNumber));
   aNumber = flipBytes(aNumber);
}
*/

void FileIO::readNotMachineEndian(float& aNumber) {
   this->read((char*)&aNumber, sizeof(aNumber));
   aNumber = flipBytes(aNumber);
}

void FileIO::readNotMachineEndian(double& aNumber) {
   this->read((char*)&aNumber, sizeof(aNumber));
   aNumber = flipBytes(aNumber);
}



//////////////////////////////
//
// FileIO::writeBigEndian --
//

void FileIO::writeBigEndian(int8_t aNumber) {
   #ifdef OTHEREND
      writeNotMachineEndian(aNumber);
   #else
      writeMachineEndian(aNumber);
   #endif
}

void FileIO::writeBigEndian(uint8_t aNumber) {
   #ifdef OTHEREND
      writeNotMachineEndian(aNumber);
   #else
      writeMachineEndian(aNumber);
   #endif
}

void FileIO::writeBigEndian(int16_t aNumber) {
   #ifdef OTHEREND
      writeNotMachineEndian(aNumber);
   #else
      writeMachineEndian(aNumber);
   #endif
}

void FileIO::writeBigEndian(uint16_t aNumber) {
   #ifdef OTHEREND
      writeNotMachineEndian(aNumber);
   #else
      writeMachineEndian(aNumber);
   #endif
}

void FileIO::writeBigEndian(int32_t aNumber) {
   #ifdef OTHEREND
      writeNotMachineEndian(aNumber);
   #else
      writeMachineEndian(aNumber);
   #endif
}

void FileIO::writeBigEndian(uint32_t aNumber) {
   #ifdef OTHEREND
      writeNotMachineEndian(aNumber);
   #else
      writeMachineEndian(aNumber);
   #endif
}

/*
void FileIO::writeBigEndian(int aNumber) {
   #ifdef OTHEREND
      writeNotMachineEndian(aNumber);
   #else
      writeMachineEndian(aNumber);
   #endif
}

void FileIO::writeBigEndian(uint16_t aNumber) {
   #ifdef OTHEREND
      writeNotMachineEndian(aNumber);
   #else
      writeMachineEndian(aNumber);
   #endif
}
*/

void FileIO::writeBigEndian(float aNumber) {
   #ifdef OTHEREND
      writeNotMachineEndian(aNumber);
   #else
      writeMachineEndian(aNumber);
   #endif
}

void FileIO::writeBigEndian(double aNumber) {
   #ifdef OTHEREND
      writeNotMachineEndian(aNumber);
   #else
      writeMachineEndian(aNumber);
   #endif
}



//////////////////////////////
//
// FileIO::writeLittleEndian --
//

void FileIO::writeLittleEndian(int8_t aNumber) {
   #ifdef OTHEREND
      writeMachineEndian(aNumber);
   #else
      writeNotMachineEndian(aNumber);
   #endif
}

void FileIO::writeLittleEndian(uint8_t aNumber) {
   #ifdef OTHEREND
      writeMachineEndian(aNumber);
   #else
      writeNotMachineEndian(aNumber);
   #endif
}

void FileIO::writeLittleEndian(int16_t aNumber) {
   #ifdef OTHEREND
      writeMachineEndian(aNumber);
   #else
      writeNotMachineEndian(aNumber);
   #endif
}

void FileIO::writeLittleEndian(uint16_t aNumber) {
   #ifdef OTHEREND
      writeMachineEndian(aNumber);
   #else
      writeNotMachineEndian(aNumber);
   #endif
}

void FileIO::writeLittleEndian(int32_t aNumber) {
   #ifdef OTHEREND
      writeMachineEndian(aNumber);
   #else
      writeNotMachineEndian(aNumber);
   #endif
}

void FileIO::writeLittleEndian(uint32_t aNumber) {
   #ifdef OTHEREND
      writeMachineEndian(aNumber);
   #else
      writeNotMachineEndian(aNumber);
   #endif
}

/*
void FileIO::writeLittleEndian(int aNumber) {
   #ifdef OTHEREND
      writeMachineEndian(aNumber);
   #else
      writeNotMachineEndian(aNumber);
   #endif
}

void FileIO::writeLittleEndian(uint16_t aNumber) {
   #ifdef OTHEREND
      writeMachineEndian(aNumber);
   #else
      writeNotMachineEndian(aNumber);
   #endif
}
*/

void FileIO::writeLittleEndian(float aNumber) {
   #ifdef OTHEREND
      writeMachineEndian(aNumber);
   #else
      writeNotMachineEndian(aNumber);
   #endif
}

void FileIO::writeLittleEndian(double aNumber) {
   #ifdef OTHEREND
      writeMachineEndian(aNumber);
   #else
      writeNotMachineEndian(aNumber);
   #endif
}



//////////////////////////////
//
// FileIO::writeMachineEndian --
//

void FileIO::writeMachineEndian(int8_t aNumber) {
   this->write((char*)&aNumber, sizeof(aNumber));
}

void FileIO::writeMachineEndian(uint8_t aNumber) {
   this->write((char*)&aNumber, sizeof(aNumber));
}

void FileIO::writeMachineEndian(int16_t aNumber) {
   this->write((char*)&aNumber, sizeof(aNumber));
}

void FileIO::writeMachineEndian(uint16_t aNumber) {
   this->write((char*)&aNumber, sizeof(aNumber));
}

void FileIO::writeMachineEndian(int32_t aNumber) {
   this->write((char*)&aNumber, sizeof(aNumber));
}

void FileIO::writeMachineEndian(uint32_t aNumber) {
   this->write((char*)&aNumber, sizeof(aNumber));
}

/*
void FileIO::writeMachineEndian(int aNumber) {
   this->write((char*)&aNumber, sizeof(aNumber));
}

void FileIO::writeMachineEndian(uint16_t aNumber) {
   this->write((char*)&aNumber, sizeof(aNumber));
}
*/

void FileIO::writeMachineEndian(float aNumber) {
   this->write((char*)&aNumber, sizeof(aNumber));
}

void FileIO::writeMachineEndian(double aNumber) {
   this->write((char*)&aNumber, sizeof(aNumber));
}



//////////////////////////////
//
// FileIO::writeNotMachineEndian --
//

void FileIO::writeNotMachineEndian(int8_t aNumber) {
   // aNumber = flipBytes(aNumber);
   this->write((char*)&aNumber, sizeof(aNumber));
}

void FileIO::writeNotMachineEndian(uint8_t aNumber) {
   // aNumber = flipBytes(aNumber);
   this->write((char*)&aNumber, sizeof(aNumber));
}

void FileIO::writeNotMachineEndian(int16_t aNumber) {
   aNumber = flipBytes(aNumber);
   this->write((char*)&aNumber, sizeof(aNumber));
}

void FileIO::writeNotMachineEndian(uint16_t aNumber) {
   aNumber = flipBytes(aNumber);
   this->write((char*)&aNumber, sizeof(aNumber));
}

void FileIO::writeNotMachineEndian(int32_t aNumber) {
   aNumber = flipBytes(aNumber);
   this->write((char*)&aNumber, sizeof(aNumber));
}

void FileIO::writeNotMachineEndian(uint32_t aNumber) {
   aNumber = flipBytes(aNumber);
   this->write((char*)&aNumber, sizeof(aNumber));
}

/*
void FileIO::writeNotMachineEndian(int16_t aNumber) {
   aNumber = flipBytes(aNumber);
   this->write((char*)&aNumber, sizeof(aNumber));
}

void FileIO::writeNotMachineEndian(uint16_t aNumber) {
   aNumber = flipBytes(aNumber);
   this->write((char*)&aNumber, sizeof(aNumber));
}
*/

void FileIO::writeNotMachineEndian(float aNumber) {
   aNumber = flipBytes(aNumber);
   this->write((char*)&aNumber, sizeof(int16_t));
}

void FileIO::writeNotMachineEndian(double aNumber) {
   aNumber = flipBytes(aNumber);
   this->write((char*)&aNumber, sizeof(int32_t));
}


///////////////////////////////////////////////////////////////////////////
//
// private functions
//


//////////////////////////////
//
// flipBytes -- flip the bytes in a number
//

int8_t FileIO::flipBytes(int8_t aNumber) {
   return aNumber;
}


uint8_t FileIO::flipBytes(uint8_t aNumber) {
   return aNumber;
}


int16_t FileIO::flipBytes(int16_t aNumber) {
   static uint8_t output[2];
   static uint8_t* input;
   input = (uint8_t*)(&aNumber);

   output[0] = input[1];
   output[1] = input[0];

   return *((int16_t*)(&output));
}


uint16_t FileIO::flipBytes(uint16_t aNumber) {
   static uint8_t output[2];
   static uint8_t* input;
   input = (uint8_t*)(&aNumber);

   output[0] = input[1];
   output[1] = input[0];

   return *((uint16_t*)(&output));
}


int32_t FileIO::flipBytes(int32_t aNumber) {
   static uint8_t output[4];
   static uint8_t* input;
   input = (uint8_t*)(&aNumber);

   output[0] = input[3];
   output[1] = input[2];
   output[2] = input[1];
   output[3] = input[0];

   return *((int32_t*)(&output));
}


uint32_t FileIO::flipBytes(uint32_t aNumber) {
   static uint8_t output[4];
   static uint8_t* input;
   input = (uint8_t*)(&aNumber);

   output[0] = input[3];
   output[1] = input[2];
   output[2] = input[1];
   output[3] = input[0];

   return *((uint32_t*)(&output));
}

/*
int FileIO::flipBytes(int aNumber) {
   static uint8_t output[sizeof(uint16_t)];
   static uint8_t* input;
   input = (uint8_t*)(&aNumber);

   for(uint16_t i=0; i<sizeof(int); i++) {
      output[i] = input[sizeof(int)-1-i];
   }

   return *((int*)(&output));
}


uint16_t FileIO::flipBytes(uint16_t aNumber) {
   static uint8_t output[sizeof(uint16_t)];
   static uint8_t* input;
   input = (uint8_t*)(&aNumber);

   for(uint16_t i=0; i<sizeof(uint16_t); i++) {
      output[i] = input[sizeof(uint16_t)-1-i];
   }

   return *((uint16_t*)(&output));
}
*/



float FileIO::flipBytes(float aNumber) {
   static uint8_t output[4];
   static uint8_t* input;
   input = (uint8_t*)(&aNumber);

   output[0] = input[3];
   output[1] = input[2];
   output[2] = input[1];
   output[3] = input[0];

   return *((float*)(&output));
}


double FileIO::flipBytes(double aNumber) {
   static uint8_t output[8];
   static uint8_t* input;
   input = (uint8_t*)(&aNumber);

   output[0] = input[7];
   output[1] = input[6];
   output[2] = input[5];
   output[3] = input[4];
   output[4] = input[3];
   output[5] = input[2];
   output[6] = input[1];
   output[7] = input[0];

   return *((double*)(&output));
}



/*   This is what I want to use
template<class type>
type FileIO::flipBytes(type aThing) {
   uint8_t* input = (uint8_t*)(&aNumber);
   uint8_t output[sizeof(aThing)];

   for(int i=0; i<sizeof(aThing); i++) {
      output[i] = input[sizeof(aThing) - 1 - i];
   }

   return *((type*)(&output));
}
*/



// md5sum: 98791588b78006c4a17c290acb82d15e FileIO.cpp [20050403]
