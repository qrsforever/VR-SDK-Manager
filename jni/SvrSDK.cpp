#include "SvrSDK.h"
#include "LocalSocket.h"
#include "VrLog.h"

#include <stdlib.h>

#define REQ_SVR_VERSION     "svr_sdk_version"
#define REQ_SVR_FEATURE     "svr_feature"

namespace VR {

SvrSDK::SvrSDK(VrSocket* s)
    : VrSDK(s)
    , mServerVersion("")
{
    /*   Inner S SDK ,  FEATURE  */
    addItem("Svr_S_001S", SDK_FEATURE_DEFAULT | SDK_FEATURE_MEDIA_SURFACE);
}

SvrSDK::~SvrSDK()
{
}

const char*
SvrSDK::serverVersion()
{
    if (mServerVersion.empty())
        mServerVersion = mClientSock->acquire(REQ_SVR_VERSION);
    return mServerVersion.c_str();
}

uint64_t
SvrSDK::featureSupported()
{
    std::string majorVer = serverVersion();
    std::size_t found = majorVer.find('.');
    if (found != std::string::npos)
        majorVer = majorVer.substr(0, found);
    std::map<std::string, uint64_t>::iterator it = mVFs.find(majorVer);
    if (it != mVFs.end())
        return it->second;

    char rbuff[64] = { 0 };
    sprintf(rbuff, "%s:%s", REQ_SVR_FEATURE, SVR_CLI_VERSION);
    std::string t = mClientSock->acquire(rbuff);
    LOGI("SvrSDK::featureSupported feature[%s]\n", t.c_str());
    if ('-' == t[0])
        return SDK_FEATURE_NONE;
    uint64_t feature = atoll(t.c_str());
    addItem(majorVer.c_str(), feature);
    return feature;
}

}

