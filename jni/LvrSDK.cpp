#include "LvrSDK.h"
#include "LocalSocket.h"
#include "VrLog.h"

#include <stdlib.h>
#include <string.h>

#define REQ_LVR_VERSION     "lvr_sdk_version"
#define REQ_LVR_FEATURE     "lvr_feature"

namespace VR {

LvrSDK::LvrSDK(VrSocket *s)
    : VrSDK(s)
    , mServerVersion("")
{
    /*   Inner S SDK ,  FEATURE  */
    addItem("Lvr_S_001S", SDK_FEATURE_DEFAULT | SDK_FEATURE_MEDIA_SURFACE);
}

LvrSDK::~LvrSDK()
{
}

const char*
LvrSDK::serverVersion()
{
    if (mServerVersion.empty())
        mServerVersion = mClientSock->acquire(REQ_LVR_VERSION);
    return mServerVersion.c_str();
}

uint64_t
LvrSDK::featureSupported()
{
    std::string majorVer = serverVersion();
    std::size_t found = majorVer.find('.');
    if (found != std::string::npos)
        majorVer = majorVer.substr(0, found);
    std::map<std::string, uint64_t>::iterator it = mVFs.find(majorVer);
    if (it != mVFs.end())
        return it->second;

    char rbuff[64] = { 0 };
    sprintf(rbuff, "%s:%s", REQ_LVR_FEATURE, LVR_CLI_VERSION);
    std::string t = mClientSock->acquire(rbuff);
    LOGI("LvrSDK::featureSupported feature[%s]\n", t.c_str());
    if ('-' == t[0])
        return SDK_FEATURE_NONE;
    uint64_t feature = atoll(t.c_str());
    addItem(majorVer.c_str(), feature);
    return feature;
}

}
