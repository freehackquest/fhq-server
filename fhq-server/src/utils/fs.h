#ifndef FS_H
#define FS_H

#include <string>
#include <vector>

class FS {

    public:
        static bool fileExists(const std::string &sFilename);
        static bool dirExists(const std::string &sFilename);
        static std::vector<std::string> listOfDirs(const std::string &sDirname);
        static std::vector<std::string> listOfFiles(const std::string &sDirname);     
        static bool makeDir(const std::string &sDirname);
        static bool writeFile(const std::string &sFilename, const std::string &sContent);
        static bool writeFile(const std::string &sFilename, const char *pBuffer, const int nBufferSize);
};

#endif // FS_H
