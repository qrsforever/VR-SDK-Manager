#ifndef __VR_LOCALSOCKET_H_
#define __VR_LOCALSOCKET_H_

#include <string>

namespace VR {

class VrSocket {
protected:
    VrSocket() { }
public:
    virtual ~VrSocket() { }
    virtual std::string acquire(std::string field) = 0;
};

class LocalSocket : public VrSocket {
public:
    LocalSocket(const char *sockpath);
    ~LocalSocket();
    virtual std::string acquire(std::string field);
private:
    int _CreateSocket();
private:
    int mSockFD;
    std::string mSockPath;
};

}

#endif
