#include "LeVRManager.h"
#include "LocalSocket.h"

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "VrLog.h"
#include "LvrSDK.h"
#include "SvrSDK.h"

static VR::LeVRManager *g_m = 0;

VR::LeVRManager* LeVRMgr()
{
    if (!g_m) {
        g_m = new VR::LeVRManager();
        g_m->initSDK();
    }
    LOGI("LeVRManager (%p)\n", g_m);
    return g_m;
}

namespace VR {

LeVRManager::LeVRManager()
{
#if defined(VR_BUILDTIME)
    LOGI("Unity LeVRManager BuildTime:%s\n", VR_BUILDTIME);
#endif
}

LeVRManager::~LeVRManager()
{
    if (mSock)
        delete mSock;
    mSock = 0;

    for (size_t i = 0; i < mSuppSDKS.size(); ++i)
        delete mSuppSDKS[i];
    mSuppSDKS.clear();
}

int
LeVRManager::initSDK()
{
    mSock = new LocalSocket(VR_SOCKET_NAME);
    if (!mSock)
        return -1;

    std::string res = mSock->acquire(REQ_SUPPORT_SDKS);
    if (strcasestr(res.c_str(), "svr")) {
        LOGI("Support Svr");
        mSuppSDKS.push_back(new SvrSDK(mSock));
    }
    if (strcasestr(res.c_str(), "lvr")) {
        LOGI("Support Lvr");
        mSuppSDKS.push_back(new LvrSDK(mSock));
    }
#if VR_DEBUG
    for (size_t i = 0; i < mSuppSDKS.size(); ++i)
        mSuppSDKS[i]->debugShow();
#endif
    return 0;
}

int
LeVRManager::findSupportedInternalVRSDK(uint64_t fsup)
{
    LOGI("findSupportedInternalVRSDK(%llu)\n", fsup);
    VrSDK *v = 0;
    int cnt = (int)mSuppSDKS.size();
    for (int i = 0; i < cnt; ++i) {
        v = mSuppSDKS[i];
        if (fsup == (fsup & v->featureSupported()))
            return v->type();
    }
    return eST_None;
}

uint64_t
LeVRManager::listVRSDKFeatureSupported(int sdktype)
{
    LOGI("listVRSDKFeatureSupported(%d)\n", sdktype);
    VrSDK * v = 0;
    int cnt = (int)mSuppSDKS.size();
    for (int i = 0; i < cnt; ++i) {
        v = mSuppSDKS[i];
        if (sdktype == v->type())
            return v->featureSupported();
    }
    return SDK_FEATURE_NONE;
}

const char*
LeVRManager::getClientVRSDKVersion(int sdktype)
{
    LOGI("getClientVRSDKVersion(%d)\n", sdktype);
    VrSDK * v = 0;
    int cnt = (int)mSuppSDKS.size();
    for (int i = 0; i < cnt; ++i) {
        v = mSuppSDKS[i];
        if (sdktype == v->type())
            return v->clientVersion();
    }
    return 0;
}

const char*
LeVRManager::getServerVRSDKVersion(int sdktype)
{
    LOGI("getServerVRSDKVersion(%d)\n", sdktype);
    VrSDK * v = 0;
    int cnt = (int)mSuppSDKS.size();
    for (int i = 0; i < cnt; ++i) {
        v = mSuppSDKS[i];
        if (sdktype == v->type())
            return v->serverVersion();
    }
    return 0;
}

} // End namespace VR
