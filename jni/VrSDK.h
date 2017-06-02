#ifndef __VRManager_VRSDK__H
#define __VRManager_VRSDK__H

#include <inttypes.h>
#include <string>
#include <map>

#define VR_SOCKET_NAME  "/dev/socket/vrserver_socket"

namespace VR {

#define VRSDK_EXPORT_VERSION        "2.0.1"

#define SDK_FEATURE_NONE            0
#define SDK_FEATURE_DEFAULT         1 // HeadTracking + ATW + Distortion
#define SDK_FEATURE_MEDIA_SURFACE   2
#define SDK_FEATURE_6D0F_TRACKING   4
#define SDK_FEATURE_MAX             (1 << 63 -1)

#define REQ_SUPPORT_SDKS    "support_sdks"

enum {
    eST_None = 0,
    eST_Lvr = 1,
    eST_Svr = 2,
};

class VrSocket;
class VrSDK {
protected:
    VrSDK(VrSocket* socket);
    int addItem(const char* v, uint64_t f);

public:
    virtual ~VrSDK();
    virtual int type() = 0;
    virtual uint64_t featureSupported() = 0;
    virtual const char* serverVersion() = 0;
    virtual const char* clientVersion() = 0;
#if VR_DEBUG
    void debugShow();
#endif

protected:
    VrSocket* mClientSock;
    std::map<std::string, uint64_t> mVFs;
};

}

#endif
