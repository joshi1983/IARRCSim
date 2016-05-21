#ifndef BITMAP_IMAGE_HEADER_FILE
#define BITMAP_IMAGE_HEADER_FILE

#include "../threads/Mutex.hpp"

class BitmapImage;

/**
A class for working with the Windows Bitmap File Format.
This supports bit depths of 16, 24, and 32 with uncompressed pixel data.

This does not support compressed bitmaps or bit depths of 1,4, or 8.
 Compression is rarely used with this file format, though.
*/
class BitmapImage
{
   private:
     int width,height;
     int *pixelData;
     Mutex modificationMutex;
     void freePixelData();
     void setPixelUnchecked(int x,int y,int colour);

   public:
       const int static channel_red=0;
       const int static channel_green=1;
       const int static channel_blue=2;
       const int static channel_alpha=3;

     static unsigned char getChannel(int colour,int channel);
     static int getRed(int colour);
     static int getGreen(int colour);
     static int getBlue(int colour);
     static int getAlpha(int colour);
     static int getColourForRGB(int red,int green,int blue);

     BitmapImage();
     BitmapImage(const char* filename);
     BitmapImage(int w,int h);
     virtual ~BitmapImage();
     void setSize(int width,int height);

     /** loads uncompressed bitmaps with colour depths of 16,24, and 32
     returns 0, if successfully loaded.
     */
     int loadFromBitmapFile(const char*filename);

     /** saves bitmaps using 24 bit colour depth */
     void saveToBitmapFile(const char* filename);

     /** saves using the specified depth of 16, 24, or 32 */
     void saveToBitmapFile(const char* filename,int bitDepth);

	/** uses 24-bit colour depth */
     char* getAsBitmapFileContents(int *len);

     int getHeight();
     int getWidth();

     int getPixel(int x,int y);

    void setPixel(int x,int y,int colour);
     void fillWith(int colour);
     void fillRect(int left,int top,int width,int height,int colour);
     void setPixelRGB(int x,int y,int red,int green,int blue);
};

#endif
