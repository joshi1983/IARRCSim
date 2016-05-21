#ifndef LITTLE_ENDIAN_OUTPUT_STREAM_HEADER
#define LITTLE_ENDIAN_OUTPUT_STREAM_HEADER

#include <iostream>
#include <fstream>

// #define BIG_ENDIAN_DEFAULT
/**
 uncomment the above line if you want to compile for a computer that uses big endian by default

WARNING: 
  sizeof(double) assumed to be 10 bytes
  sizeof(int) assumed to be 4 bytes
  sizeof(short int) assumed to be 2 bytes
*/
using namespace std;

class LEOutputStream: public ofstream
{
  public:    
      void write4ByteInt(int i);
      
      /** writes the smaller 3 bytes of an integer value in little endian format 
        This is used for certain file formats such as 24 bit pixel data in a Windows bitmap.
      */
      void write3ByteInt(int i);
      void write2ByteShort(short int s);
      void write10ByteDouble(double d);
};

class LEInputStream: public ifstream
{

 public:      
      int read4ByteInt();
      int read3ByteInt();
      short int read2ByteShort();
      double read10ByteDouble();
};

#endif
