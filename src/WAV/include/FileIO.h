//
// Copyright 1997 by Craig Stuart Sapp, All Rights Reserved.
// Programmer:    Craig Stuart Sapp <craig@ccrma.stanford.edu>
// Creation Date: Fri May  9 22:30:32 PDT 1997
// Last Modified: Sun Dec 14 05:26:16 GMT-0800 1997
// Filename:      ...sig/maint/code/base/FileIO/FileIO.h
// Web Address:   http://sig.sapp.org/include/sigBase/FileIO.h
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

#ifndef _FILEIO_H_INCLUDED
#define _FILEIO_H_INCLUDED

#ifndef OLDCPP
   #include <fstream>
#else
   #include <fstream.h>
#endif


// templates would be nice to use here, but they don't seem
// to work intuitively...

class FileIO : public std::fstream {
   public:
                     FileIO                (void);
#ifndef OLDCPP
                     FileIO      (const char* filename, std::ios::openmode state);
#else
                     FileIO      (const char* filename, int state);
#endif

                    ~FileIO                ();

      void           readBigEndian         (int8_t& aNumber);
      void           readBigEndian         (uint8_t& aNumber);
      void           readBigEndian         (int16_t& aNumber);
      void           readBigEndian         (uint16_t& aNumber);
      void           readBigEndian         (int32_t& aNumber);
      void           readBigEndian         (uint32_t& aNumber);
/*    void           readBigEndian         (int& aNumber);
      void           readBigEndian         (uint& aNumber);*/
      void           readBigEndian         (float& aNumber);
      void           readBigEndian         (double& aNumber);

      void           readLittleEndian      (int8_t& aNumber);
      void           readLittleEndian      (uint8_t& aNumber);
      void           readLittleEndian      (int16_t& aNumber);
      void           readLittleEndian      (uint16_t& aNumber);
      void           readLittleEndian      (int32_t& aNumber);
      void           readLittleEndian      (uint32_t& aNumber);
/*    void           readLittleEndian      (int& aNumber);
      void           readLittleEndian      (uint& aNumber);*/
      void           readLittleEndian      (float& aNumber);
      void           readLittleEndian      (double& aNumber);

      void           readMachineEndian     (int8_t& aNumber);
      void           readMachineEndian     (uint8_t& aNumber);
      void           readMachineEndian     (int16_t& aNumber);
      void           readMachineEndian     (uint16_t& aNumber);
      void           readMachineEndian     (int32_t& aNumber);
      void           readMachineEndian     (uint32_t& aNumber);
/*    void           readMachineEndian     (int& aNumber);
      void           readMachineEndian     (uint& aNumber);*/
      void           readMachineEndian     (float& aNumber);
      void           readMachineEndian     (double& aNumber);

      void           readNotMachineEndian  (int8_t& aNumber);
      void           readNotMachineEndian  (uint8_t& aNumber);
      void           readNotMachineEndian  (int16_t& aNumber);
      void           readNotMachineEndian  (uint16_t& aNumber);
      void           readNotMachineEndian  (int32_t& aNumber);
      void           readNotMachineEndian  (uint32_t& aNumber);
/*    void           readNotMachineEndian  (int& aNumber);
      void           readNotMachineEndian  (uint& aNumber);*/
      void           readNotMachineEndian  (float& aNumber);
      void           readNotMachineEndian  (double& aNumber);

      void           writeBigEndian        (int8_t aNumber);
      void           writeBigEndian        (uint8_t aNumber);
      void           writeBigEndian        (int16_t aNumber);
      void           writeBigEndian        (uint16_t aNumber);
      void           writeBigEndian        (int32_t aNumber);
      void           writeBigEndian        (uint32_t aNumber);
/*    void           writeBigEndian        (int aNumber);
      void           writeBigEndian        (uint aNumber);*/
      void           writeBigEndian        (float aNumber);
      void           writeBigEndian        (double aNumber);

      void           writeLittleEndian     (int8_t aNumber);
      void           writeLittleEndian     (uint8_t aNumber);
      void           writeLittleEndian     (int16_t aNumber);
      void           writeLittleEndian     (uint16_t aNumber);
      void           writeLittleEndian     (int32_t aNumber);
      void           writeLittleEndian     (uint32_t aNumber);
/*    void           writeLittleEndian     (int aNumber);
      void           writeLittleEndian     (uint aNumber);*/
      void           writeLittleEndian     (float aNumber);
      void           writeLittleEndian     (double aNumber);

      void           writeMachineEndian    (int8_t aNumber);
      void           writeMachineEndian    (uint8_t aNumber);
      void           writeMachineEndian    (int16_t aNumber);
      void           writeMachineEndian    (uint16_t aNumber);
      void           writeMachineEndian    (int32_t aNumber);
      void           writeMachineEndian    (uint32_t aNumber);
/*    void           writeMachineEndian    (int aNumber);
      void           writeMachineEndian    (uint aNumber);*/
      void           writeMachineEndian    (float aNumber);
      void           writeMachineEndian    (double aNumber);

      void           writeNotMachineEndian (int8_t aNumber);
      void           writeNotMachineEndian (uint8_t aNumber);
      void           writeNotMachineEndian (int16_t aNumber);
      void           writeNotMachineEndian (uint16_t aNumber);
      void           writeNotMachineEndian (int32_t aNumber);
      void           writeNotMachineEndian (uint32_t aNumber);
/*    void           writeNotMachineEndian (int aNumber);
      void           writeNotMachineEndian (uint aNumber);*/
      void           writeNotMachineEndian (float aNumber);
      void           writeNotMachineEndian (double aNumber);

   protected:

      int8_t           flipBytes             (int8_t aNumber);
      uint8_t          flipBytes             (uint8_t aNumber);
      int16_t          flipBytes             (int16_t aNumber);
      uint16_t         flipBytes             (uint16_t aNumber);
      int32_t           flipBytes             (int32_t aNumber);
      uint32_t          flipBytes             (uint32_t aNumber);
/*    int            flipBytes             (int aNumber);
      uint           flipBytes             (uint aNumber);*/
      float          flipBytes             (float aNumber);
      double         flipBytes             (double aNumber);

};



#endif  /* _FILEIO_H_INCLUDED */



// md5sum: 20f4083bc14ee144905e6902659f7494 FileIO.h [20050403]
