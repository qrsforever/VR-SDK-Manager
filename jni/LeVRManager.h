#ifndef __VRManager__H_
#define __VRManager__H_

#include "VrSDK.h"
#include <vector>

namespace VR {

class VrSocket;
class LvrSDK;
class SvrSDK;

class LeVRManager {
public:
    LeVRManager();
    ~LeVRManager();
    int initSDK();
    int findSupportedInternalVRSDK(uint64_t fsup);
    uint64_t listVRSDKFeatureSupported(int sdktype);
    const char* getClientVRSDKVersion(int sdktype);
    const char* getServerVRSDKVersion(int sdktype);

private:
    VrSocket *mSock;
    std::vector<VrSDK*> mSuppSDKS;
};

}

VR::LeVRManager* LeVRMgr();

#endif
