#include "VrSDK.h"
#include "LocalSocket.h"
#include "VrLog.h"

namespace VR {

VrSDK::VrSDK(VrSocket* socket)
    : mClientSock(socket)
{
}

VrSDK::~VrSDK()
{
    mVFs.clear();
}

int
VrSDK::addItem(const char* v, uint64_t f)
{
    LOGI("addItem(%s, %llu)\n", v, f);
    std::map<std::string, uint64_t>::iterator it = mVFs.find(v);
    if (it != mVFs.end())
        return -1;
    mVFs.insert(std::pair<std::string, uint64_t>(v, f));
    return 0;
}

#if VR_DEBUG
void
VrSDK::debugShow()
{
    LOGI("Export SDK Version[%s] Type[%d]\n", VRSDK_EXPORT_VERSION, type());
    std::map<std::string, uint64_t>::iterator it;
    for (it = mVFs.begin(); it != mVFs.end(); ++it)
       LOGI("\t InnerSDK[%s] Feature[%llu]\n", it->first.c_str(), it->second);
}
#endif

}
