#ifndef __VRManager_LvrSDK_H
#define __VRManager_LvrSDK_H

#include "VrSDK.h"

#define LVR_CLI_VERSION     "Lvr_C_001S.201610260000"

namespace VR {

class LvrSDK : public VrSDK  {
public:
    LvrSDK(VrSocket* s);
    ~LvrSDK();

    int type() { return eST_Lvr; }
    virtual uint64_t featureSupported();
    const char* serverVersion();
    const char* clientVersion() { return LVR_CLI_VERSION; }
private:
    std::string mServerVersion;
};

}

#endif
