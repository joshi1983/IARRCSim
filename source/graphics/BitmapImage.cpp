#include "BitmapImage.hpp"
#include "../io/LEFStreams_.hpp"
#include <iostream>
#include <cstdlib>

using namespace std;

const char headerData1[12]={0x00 ,0x00 ,0x00 ,0x00 ,0x36 ,0x00 ,0x00
  ,0x00 ,0x28 ,0x00 ,0x00 ,0x00};
const char headerData2[2]={0x01,0x00};
const char paddingBuffer[4]={0,0,0,0};

/**
  Returns the sum of (first byte's value) of each values[i] * blendRatios[i]
  This can be used for averaging nearby pixel colour values together
*/
double getBlendedByteValues(int values[],int numValues,double blendRatios[])
{
 double result=0.0;

   // loop through values
   for (int i=0;i<numValues;i++)
   {
      unsigned char v = values[i]&0xff;
      result += v*blendRatios[i];
   }
   return result;
}

BitmapImage::BitmapImage(): pixelData(NULL)
{
   setSize(0,0);
}

BitmapImage::BitmapImage(const char* filename): pixelData(NULL)
{
   setSize(0,0);
   loadFromBitmapFile(filename);
}

BitmapImage::BitmapImage(int width,int height): pixelData(NULL)
{
   setSize(width,height);
}

BitmapImage::~BitmapImage()
{
  freePixelData();
}

void BitmapImage::freePixelData()
{
  //  cout << "freePixelData() called."<<endl;

    modificationMutex.enterCriticalSection();
   if (pixelData==NULL) // if already freed
   {
      modificationMutex.leaveCriticalSection();
      return;
   }

   width = 0;
   height = 0;
  //  cout << "freePixelData() going to free all pixelData."<<endl;
   free(pixelData);
   pixelData = NULL;
   // cout << "freePixelData() complete."<<endl;
   modificationMutex.leaveCriticalSection();
}

void BitmapImage::setSize(int width,int height)
{
  // cout << "setSize called for width="<<width<<", height="<<height<<endl;
     modificationMutex.enterCriticalSection();

   // if no change in dimensions, don't do anything more.
   if (width==this->width && height==this->height)
   {
      modificationMutex.leaveCriticalSection();
      return;
   }

   if (pixelData!=NULL)
      freePixelData();

   //cout << "setSize finished freeing pixel data."<<endl;
   this->width = width;
   this->height = height;

   if (width==0 || height==0)
   {
       modificationMutex.leaveCriticalSection();
      return;
   }
  // cout << "setSize dimensions test out fine."<<endl;

   pixelData = (int*)malloc(width*height*sizeof(int));
  // cout << "BitmapImage::setSize could tried to allocate memory."<<endl;

   if (pixelData==NULL)
   {
	 cout << "Unable to allocate memory for bitmap pixel rows."<<endl;
      this->width=0;
	 this->height=0;
   }
   modificationMutex.leaveCriticalSection();
  // cout << "setSize complete."<<endl;
}

int BitmapImage::getWidth()
{
    modificationMutex.enterCriticalSection();
    int result= width;
    modificationMutex.leaveCriticalSection();

    return result;
}

int BitmapImage::getHeight()
{
        modificationMutex.enterCriticalSection();
   int result=height;
        modificationMutex.leaveCriticalSection();

        return result;
}

int BitmapImage::getPixel(int x,int y)
{
    int result=0;

   modificationMutex.enterCriticalSection();

   if ((x<0) || (x>=width) || (y<0) || (y>=height))
      result= 0;
   else
      result= pixelData[x+y*width];

    modificationMutex.leaveCriticalSection();

    return result;
}

void BitmapImage::setPixel(int x,int y,int colour)
{

   modificationMutex.enterCriticalSection();

   if ((x>=0) && (x<width) && (y>=0) && (y<height))
      pixelData[x+y*width] = colour;

   modificationMutex.leaveCriticalSection();
}

int BitmapImage::loadFromBitmapFile(const char*filename)
{
  LEInputStream in_file;
  char buffer[100];
  int fileSize;
  int sizeOfHeader;
  int bytesofPixelData;
  int height,width;
  int bitDepth=0;
  int compressionMethod=0;

   in_file.open(filename,ios::binary|ios::in);

   if (in_file.fail())
   {
      printf("WARNING: bitmap file can't open: %s\n",filename);
      return -1;
   }

   in_file.read(buffer,2);
    if (buffer[0]!='B' || buffer[1]!='M')
    {
       perror("Invalid Bitmap File Format");
       return 1;
    }
   fileSize = in_file.read4ByteInt();
   in_file.read(buffer,4);
   sizeOfHeader = in_file.read4ByteInt();
   in_file.read(buffer,4);
   width = in_file.read4ByteInt();
   height = in_file.read4ByteInt();
   in_file.read(buffer,2);

   bitDepth = in_file.read2ByteShort();
   if (bitDepth!=24 && bitDepth!=32 && bitDepth!=16)
   {
      cout << "problem with unsupported bit depth: "<<bitDepth<<"\n";
      return 3;
   }

   compressionMethod = in_file.read4ByteInt();
   if (compressionMethod!=0)
   {
      cout << "Unsupported compression method: "<<compressionMethod<<"\n";
      return 2;
   }

   bytesofPixelData = in_file.read4ByteInt();
   if (bytesofPixelData+sizeOfHeader!=fileSize)
   {
      cout << "bitmap block size inconsistant\n";
      return 4;
   }

   // read the rest of the header
   in_file.read(buffer,sizeOfHeader-38);

   setSize(width,height);

      modificationMutex.enterCriticalSection();

   /*
   the number of bytes in a scanline must be a multiple of 4 so a padding is used to fill the gap
   */
   int padding=(width*(bitDepth/8))%4;

   if (padding!=0)
      padding = 4-padding;

  // loop through rows of pixels
  for (int y=height-1;y>=0;y--)
  {
      for (int x=0;x<width;x++)
      {
          int * p = &(pixelData[x+y*width]);
           (*p)=0;
          if (bitDepth==32)
          {
            // read each directly from the file
            (*p) = in_file.read4ByteInt();
          }
          else if (bitDepth==24)
          {
            // read each directly from the file
            (*p) = in_file.read3ByteInt();
          }
          else if (bitDepth==16)
          {
               int encodedColour=0;
            // 5 bits per RGB value needs to be expanded to full 8 bits
            encodedColour = in_file.read2ByteShort();
             encodedColour =
               ((encodedColour&0x1f)<<3) // blue
               |((encodedColour&0x3e0)<<6) // green
               |((encodedColour&0x7c00)<<9) // red
              ;
              (*p) = encodedColour;
          }
      }
      if (y==0) break;

      if (padding!=0)
         in_file.read(buffer,padding);
  }
     modificationMutex.leaveCriticalSection();

   in_file.close();
   return 0;
}

void BitmapImage::saveToBitmapFile(const char* filename)
{
    saveToBitmapFile(filename,24);
}

void BitmapImage::saveToBitmapFile(const char* filename,int bitDepth)
{
  if (bitDepth!=16 && bitDepth!=24 && bitDepth!=32)
  {
     cout << "Unsupported bit depth: "<<bitDepth<<" must be 16,24 or 32.\n";
     cout << "bit depth assumed to be 24 for save to continue.\n";
     bitDepth = 24;
  }
  LEOutputStream out_file;


  int compressionMethod = 0; // uncompressed

     modificationMutex.enterCriticalSection();

  int fileSize = 54+width*height*bitDepth/8;
  int bytesofPixelData = width*height*bitDepth/8;

   out_file.open(filename,ios::binary|ios::out);

  out_file.write("BM",2);
  // write some other bytes of data to the file

  out_file.write4ByteInt(fileSize);

  out_file.write(headerData1,sizeof(headerData1));

  out_file.write4ByteInt(width);
  out_file.write4ByteInt(height);

  out_file.write(headerData2,sizeof(headerData2));
  out_file.write4ByteInt(bitDepth);
  out_file.write2ByteShort(compressionMethod);
  out_file.write4ByteInt(bytesofPixelData);

  char c = '\0';
  for (int i=0;i<16;i++)
      out_file.write(&c,sizeof(char));

    // each scanline or row of pixels must be a multiple of 4 bytes so a padding is used
   int padding=(width*bitDepth/8)%4;
   if (padding!=0)
      padding = 4-padding;


  // loop through rows of pixels
  for (int y=height-1;y>=0;y--)
  {
      for (int x=0;x<width;x++)
      {
        int encodedPixel =  pixelData[x+y*width];
         if (bitDepth==32)
           // write each directly to the file
           out_file.write4ByteInt(encodedPixel);
         else if (bitDepth==24)
           // write each directly to the file
           out_file.write3ByteInt(encodedPixel);
         else if (bitDepth==16)
         {
            encodedPixel = ((encodedPixel&0xf80000)>>9) // red
               | ((encodedPixel&0xf800)>>6) // green
               | ((encodedPixel&0xf8) >> 3);  // blue
           out_file.write2ByteShort(encodedPixel);
         }
      }
      if (y==0)
         break;

      if (padding!=0)
         out_file.write(paddingBuffer,padding);
         // doesn't matter what the values are so question marks are fine
  }
     modificationMutex.leaveCriticalSection();

   out_file.close();
}

void write32BitIntLittleEndian(int i,char *location)
{
  location[0]=(char)i;
  location[1]=(char)(i >> 8);
  location[2]=(char)(i >> 16);
  location[3]=(char)(i >> 24);
}

void write16BitIntLittleEndian(int i,char *location)
{
  location[0]=(char)i;
  location[1]=(char)(i >> 8);
}


void copyDataBytes(char *dest,const char *src,int numBytes)
{
  for (int i=0;i<numBytes;i++)
	  dest[i]=src[i];
}

char* BitmapImage::getAsBitmapFileContents(int *len)
{
         modificationMutex.enterCriticalSection();

 int stepWidth=width*3;
 int padding=0;
 int bitDepth=24;

  if (stepWidth%4!=0)
  {
	  padding=(4-stepWidth%4);
	  stepWidth=stepWidth+padding;
  }

 int bytesOfPixelData=stepWidth*height;
 int totalSize=bytesOfPixelData+54;
 char *result=(char*)malloc(totalSize);
 int compressionMethod = 0; // uncompressed

 (*len)=totalSize;

 if (result==NULL)
 {
   (*len)=0;
   modificationMutex.leaveCriticalSection();
    return NULL;
 }
  result[0]='B';
  result[1]='M';
  write32BitIntLittleEndian(totalSize,result+2);
  copyDataBytes(result+6,headerData1,sizeof(headerData1));
  write32BitIntLittleEndian(width,result+18);
  write32BitIntLittleEndian(height,result+22);
  copyDataBytes(result+26,headerData2,sizeof(headerData2));

  write32BitIntLittleEndian(bitDepth,result+28);
  write16BitIntLittleEndian(compressionMethod,result+32);
  write32BitIntLittleEndian(bytesOfPixelData,result+34);

  for (int i=0;i<16;i++)
      result[i+38]=0;

  int index=54;

 // loop through rows of pixels
  for (int y=height-1;y>=0;y--)
  {
      for (int x=0;x<width;x++)
      {
        int encodedPixel =  pixelData[x+y*width];
           result[index]=encodedPixel;
           result[index+1]=encodedPixel>>8;
           result[index+2]=encodedPixel>>16;
		 index+=3;
      }
      if (padding!=0)
	 {
         copyDataBytes(result+index,paddingBuffer,padding);
         index+=padding;
	 }
  }

  modificationMutex.leaveCriticalSection();

  return result;
}

void BitmapImage::fillWith(int colour)
{
    modificationMutex.enterCriticalSection();
  for (int y=0;y<height;y++)
     for (int x=0;x<width;x++)
     {
        setPixelUnchecked(x,y,colour);
     }

     modificationMutex.leaveCriticalSection();
}

void BitmapImage::fillRect(int left,int top,int width,int height,int colour)
{
  modificationMutex.enterCriticalSection();

  if (width+left>this->width)
     width = this->width-left;

  if (height+top>this->height)
     height = this->height-top;

  for (int y=top+height;y>top;y--)
   for (int x=left+width;x>left;x--)
     setPixelUnchecked(x,y,colour);

  modificationMutex.leaveCriticalSection();
}

unsigned char BitmapImage::getChannel(int colour,int channel)
{
   switch (channel)
   {
      case channel_red:
          return getRed(colour);

      case channel_green:
          return getGreen(colour);

      case channel_blue:
          return getBlue(colour);

      case channel_alpha:
          return getAlpha(colour);

   }
   return 0;
}

int BitmapImage::getRed(int colour)
{
    return ((colour>>16) & 255);
}

int BitmapImage::getGreen(int colour)
{
    return ((colour>>8) & 255);
}

int BitmapImage::getBlue(int colour)
{
    return (colour & 255);
}

int BitmapImage::getAlpha(int colour)
{
  return (colour>>24)&255;
}

int BitmapImage::getColourForRGB(int red,int green,int blue)
{
    if (red>255)
       red = 255;
    else if (red<0)
       red = 0;

    if (green>255)
       green = 255;
    else if (green<0)
       green = 0;

    if (blue>255)
       blue = 255;
    else if (blue<0)
       blue= 0;

   return ((red << 16) | (green << 8) | (blue));
}

void BitmapImage::setPixelRGB(int x,int y,int red,int green,int blue)
{
   setPixel(x,y,getColourForRGB(red,green,blue));
}

void BitmapImage::setPixelUnchecked(int x,int y,int colour)
{
  pixelData[x+y*width]=colour;
}
