#include "LeVRManager.h"
#include "MobilePhoneInfo.h"
#include <jni.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "VrLog.h"

#ifndef NELEM
#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))
#endif

#define VRMANAGER_CLASS_NAME    "com/leeco/vr/LeVRManager"

static int _GetHMDVersion();
static int _SearchOffset(const char* cstr, int tHWVer);

extern "C"  {

// C# JNI Inteface
const char* unityGetExternVRSDKVersion()
{
    return VRSDK_EXPORT_VERSION;
}

const char* unityGetClientVRSDKVersion(int sdktype)
{
    return LeVRMgr()->getClientVRSDKVersion(sdktype);
}

const char* unityGetServerVRSDKVersion(int sdktype)
{
    return LeVRMgr()->getServerVRSDKVersion(sdktype);
}

int findSupportedInternalVRSDK(uint64_t fsup)
{
    return LeVRMgr()->findSupportedInternalVRSDK(fsup);
}

uint64_t listVRSDKFeatureSupported(int sdktype)
{
    return LeVRMgr()->listVRSDKFeatureSupported(sdktype);
}

}

// Java JNI Inteface
static void __native_init(JNIEnv *env)
{
    LOGI("__native_init\n");
}

static int __native_getViewOffset(JNIEnv *env, jobject thiz)
{
    LOGI("__native_getViewOffset\n");

    jclass buildCls = env->FindClass("android/os/Build");
    jfieldID field = env->GetStaticFieldID(buildCls, "MODEL", "Ljava/lang/String;" );
    jstring jstr = (jstring)env->GetStaticObjectField(buildCls, field);
    const char *cstr = env->GetStringUTFChars(jstr, 0);

    int hmd = _GetHMDVersion();
    int mobiletype = get_mobilephone_type(cstr);
    int offset = get_centerline_offset(mobiletype, hmd);
    LOGI("Model[%s] HMD[%d] Offset[%d]\n", cstr, hmd, offset);

    env->ReleaseStringUTFChars(jstr, cstr);
    env->DeleteLocalRef(jstr);
    env->DeleteLocalRef(buildCls);
    return offset;
}

static int __native_getHMDVersion(JNIEnv *env, jobject thiz)
{
    return _GetHMDVersion();
}

static JNINativeMethod gMethods[] = {
    { "native_init",            "()V",                            (void *)__native_init },
    { "native_getViewOffset",   "()I",                            (void *)__native_getViewOffset },
    { "native_getHMDVersion",   "()I",                            (void *)__native_getHMDVersion },
};

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
    LOGI("JNI_OnLoad\n");
    JNIEnv* env = 0;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK)
        return JNI_ERR;
    jclass clazz = env->FindClass(VRMANAGER_CLASS_NAME);
    if (clazz) {
        LOGI("RegisterNatives native method: %s\n", VRMANAGER_CLASS_NAME);
        env->RegisterNatives(clazz, gMethods, NELEM(gMethods));
    }
    return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved)
{
    LOGI("JNI_OnUnload\n");
}

static int _GetHMDVersion()
{
    int hDevice = -1;
    DIR* dir = opendir("/dev");
    if (dir){
        dirent* entry = readdir(dir);
        while (entry) {
            if (strstr(entry->d_name, "ovr" )) {
                char devicePath[32];
                sprintf(devicePath, "/dev/%s", entry->d_name);
                hDevice = open(devicePath, O_RDWR);
                if (hDevice < 0)
                    hDevice = open(devicePath, O_RDONLY);
                if (hDevice >= 0)
                    break;
            }
            entry = readdir(dir);
        }
    }
    closedir(dir);
    if (hDevice < 0) {
        LOGW("Read Lvr node error!\n");
        return -1;
    }
    struct ExternalChipInfo {
        unsigned char byteData1[4];
        unsigned char byteHWVersion;
        unsigned char byteData2[15];
    } stExternalChipInfo;
    char *pData = (char*)(&stExternalChipInfo);
    pData[0] = 0x80;
    int nResult = ::ioctl(hDevice, _IOC(_IOC_WRITE|_IOC_READ, 'H', 0x07, 20), pData);
    close(hDevice);
    if (nResult < 0) {
        LOGW("Failed to read the external chip hardware version\n");
        return -1;
    }
    return (stExternalChipInfo.byteHWVersion - 0x30);
}
