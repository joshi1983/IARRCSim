#include "LEFStreams_.hpp"

int LEInputStream::read4ByteInt()
{
  int result=0;
    
    #ifdef  BIG_ENDIAN_DEFAULT
    read(((char*)&result)+sizeof(int)-1,1);
    read(((char*)&result)+sizeof(int)-2,1);
    read(((char*)&result)+sizeof(int)-3,1);
    read(((char*)&result)+sizeof(int)-4,1);    
    #else
    read((char*)&result,4);
    #endif
    
    return result;
}

int LEInputStream::read3ByteInt()
{
  int result=0;
    
    #ifdef  BIG_ENDIAN_DEFAULT
    read(((char*)&result)+sizeof(int)-1,1);
    read(((char*)&result)+sizeof(int)-2,1);
    #else
    read((char*)&result,3);
    #endif
    
    return result;
}

short int LEInputStream::read2ByteShort()
{
  short int result=0;
    #ifdef  BIG_ENDIAN_DEFAULT
    read(((char*)&result)+sizeof(short int)-1,1);
    read(((char*)&result)+sizeof(short int)-2,1);    
    #else
    read((char*)&result,2);
    #endif
    
    return result;
}

double LEInputStream::read10ByteDouble()
{
  double result;
    #ifdef  BIG_ENDIAN_DEFAULT
    for (int b=1;b<10;b++)
      read(((char*)&result)+sizeof(double)-b,1);
    #else
    read((char*)&result,2);
    #endif
    
    return result;
}

