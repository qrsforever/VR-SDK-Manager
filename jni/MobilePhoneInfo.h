#ifndef __MobilePhoneInfo__H_
#define __MobilePhoneInfo__H_

#include <string>

namespace VR {

enum MobilePhone_Type {
    MP_LETV_X1,
    MP_LETV_X2,
    MP_LETV_S2,
    MP_LETV_X6,
    MP_LETV_X7,
    MP_LETV_X7_PLUS,
    MP_LETV_VR_MAX1,
    MP_LETV_X10,
    MP_LETV_TURBO,
    MP_LETV_ZL1,

    MP_MAX,
};

enum HelmetDevice_Type {
    HD_LETV_UND0,
    HD_LETV_UND1,
    COOL_PRO,
    COOL_PRO_1S,

    HD_MAX,
};

MobilePhone_Type getMobilePhoneType(const char * model);

int getCenterLineOffset(MobilePhone_Type mpt, HelmetDevice_Type hdt);

}

extern "C"
inline int get_mobilephone_type(const char* model)
{
    return (int)VR::getMobilePhoneType(model);
}

inline int get_centerline_offset(int mpt, int hdt)
{
    if (mpt < 0 || mpt >= VR::MP_MAX || hdt < 0 || hdt >= VR::HD_MAX)
        return 0;
    return VR::getCenterLineOffset((VR::MobilePhone_Type)mpt, (VR::HelmetDevice_Type)hdt);
}
#endif
