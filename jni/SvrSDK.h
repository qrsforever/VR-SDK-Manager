#ifndef __VRManager_SvrSDK_H
#define __VRManager_SvrSDK_H

#include "VrSDK.h"

#define SVR_CLI_VERSION     "Svr_C_001S.201610260000"

namespace VR {

class SvrSDK : public VrSDK  {
public:
    SvrSDK(VrSocket* s);
    ~SvrSDK();

    int type() { return eST_Svr; }
    virtual uint64_t featureSupported();
    const char* serverVersion();
    const char* clientVersion() { return SVR_CLI_VERSION; }
private:
    std::string mServerVersion;
};

}

#endif
