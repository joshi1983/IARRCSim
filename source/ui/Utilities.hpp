#ifndef UI_UTILITIES_HEADER
#define UI_UTILITIES_HEADER

/**
 A class filled with some functions that are generally useful.
*/
class Utilities
{
 public:
   static void launchURL(const char *url);
   static void launchRelativeDocument(const char *relPath);
};

#endif