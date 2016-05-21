#include "LEFStreams_.hpp"


void LEOutputStream::write2ByteShort(short int s)
{
  #ifdef  BIG_ENDIAN_DEFAULT
  write(((char*)&s)+sizeof(short int)-1,1);
  write(((char*)&s)+sizeof(short int)-2,1);  
  #else
  write((char*)&s,2);
  #endif                                  
}

void LEOutputStream::write3ByteInt(int i)
{
  #ifdef  BIG_ENDIAN_DEFAULT
  write(((char*)&i)+sizeof(int)-1,1);
  write(((char*)&i)+sizeof(int)-2,1);  
  write(((char*)&i)+sizeof(int)-3,1);
  #else
  write((char*)&i,3);
  #endif                                  
}

void LEOutputStream::write4ByteInt(int i)
{
  #ifdef  BIG_ENDIAN_DEFAULT
  write(((char*)&i)+sizeof(int)-1,1);
  write(((char*)&i)+sizeof(int)-2,1);  
  write(((char*)&i)+sizeof(int)-3,1);
  write(((char*)&i)+sizeof(int)-4,1);  
  #else
  write((char*)&i,4);
  #endif                                  
}

void LEOutputStream::write10ByteDouble(double d)
{
  #ifdef  BIG_ENDIAN_DEFAULT
  for (int b=9;b>=0;b--)
     write(((char*)&d)+sizeof(double)-b,1);   
  #else
  write((char*)&d,10);   
  #endif    
}
