#include "fs.h"
#include <dirent.h>
#include <sys/stat.h>
#include <iostream>
#include <sstream>
#include <fstream>

// ---------------------------------------------------------------------

bool FS::fileExists(const std::string &sFilename) {
    struct stat st;
    bool bExists = (stat(sFilename.c_str(), &st) == 0);
    if (bExists) {
        return (st.st_mode & S_IFDIR) == 0;
    }
	return false;
}

// ---------------------------------------------------------------------

bool FS::dirExists(const std::string &sDirname) {
    struct stat st;
    bool bExists = (stat(sDirname.c_str(), &st) == 0);
    if (bExists) {
        return (st.st_mode & S_IFDIR) != 0;
    }
	return false;
}

// ---------------------------------------------------------------------

std::vector<std::string> FS::listOfDirs(const std::string &sDirname) {
    std::vector<std::string> vDirs;
    if (!FS::dirExists(sDirname)) {
        return vDirs;
    }
    DIR *dir = opendir(sDirname.c_str());
    struct dirent *entry = readdir(dir);
    while (entry != NULL) {
        if (entry->d_type == DT_DIR) {
            std::string sDir(entry->d_name);
            if (sDir != "." && sDir != "..") {
                vDirs.push_back(sDir);
            }
        }
        entry = readdir(dir);
    }
    closedir(dir);
    return vDirs;
}

// ---------------------------------------------------------------------

std::vector<std::string> FS::listOfFiles(const std::string &sDirname) {
    std::vector<std::string> vFiles;
    if (!FS::dirExists(sDirname)) {
        return vFiles;
    }
    DIR *dir = opendir(sDirname.c_str());
    struct dirent *entry = readdir(dir);
    while (entry != NULL) {
        if (entry->d_type != DT_DIR) {
            std::string sDir(entry->d_name);
            if (sDir != "." && sDir != "..") {
                vFiles.push_back(sDir);
            }
        }
        entry = readdir(dir);
    }
    closedir(dir);
    return vFiles;
}

// ---------------------------------------------------------------------

bool FS::makeDir(const std::string &sDirname) {
    struct stat st;
    int nStatus = mkdir(sDirname.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (nStatus == 0) {
        return true;
    }
    if (nStatus == EACCES) {
        std::cout << "FAILED create folder " << sDirname << std::endl;
        return false;
    }
    std::cout << "nStatus: " << nStatus << std::endl;
	return true;
}

// ---------------------------------------------------------------------

bool FS::writeFile(const std::string &sFilename, const std::string &sContent) {
    
    std::ofstream f(sFilename, std::ios::out);
    if (!f) {
        std::cout << "FAILED could not create file to wtite " << sFilename << std::endl;
        return false;
    }

    f << sContent << std::endl;
    f.close();
	return true;
}

// ---------------------------------------------------------------------

bool FS::writeFile(const std::string &sFilename, const char *pBuffer, const int nBufferSize) {
    std::ofstream f(sFilename, std::ios::out | std::ios::binary);
    if (!f) {
        std::cout << "FAILED could not create file to wtite " << sFilename << std::endl;
        return false;
    }
    f.write(pBuffer, nBufferSize);
    f.close();
	return true;
}

// ---------------------------------------------------------------------
