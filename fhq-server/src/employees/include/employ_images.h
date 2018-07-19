#ifndef EMPLOY_IMAGES_H
#define EMPLOY_IMAGES_H

#include <employees.h>
#include <iserver.h>

class EmployImages : public EmployBase {
private:
    std::string TAG;
    
    // singletone object
    // EmployImages() { };
    // ~EmployImages() { };
    EmployImages(EmployImages const&) = delete; // disabled
    EmployImages& operator= (EmployImages const&) = delete;  // disabled

public:
    EmployImages();
    /*static EmployImages* getInstance() {
        static EmployImages *pInstance;
        return pInstance;
    };*/
    static std::string name() { return "EmployImages"; }
    virtual bool init();
    bool doThumbnailImagePng(const std::string &sourceFilepath, const std::string &targetFilepath, int width_resize, int height_resize);

};

#endif // EMPLOY_IMAGES_H
