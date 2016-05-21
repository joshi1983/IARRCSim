#ifndef FILE_DIRECTORY_HEADER
#define FILE_DIRECTORY_HEADER

#include <string>
#include <list>

class FileDirectory
{
 private:

 public:

   static bool fileExists(const char *filename);
   static std::list<std::string> getFileNamesInDirectory(const std::string &dirName);
};

#endif