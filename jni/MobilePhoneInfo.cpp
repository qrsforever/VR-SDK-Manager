#include "MobilePhoneInfo.h"
#include "VrLog.h"

#include<map>

//TODO This is only test code.

namespace VR {

static std::map<std::string, MobilePhone_Type> g_mps;
static int g_CenterOffsets[MP_MAX][HD_MAX];

static void registerModel(const char* model, MobilePhone_Type type)
{
    // model: ro.product.model
    g_mps.insert(std::pair<std::string, MobilePhone_Type>(model, type));
}

static void initiateOffset(MobilePhone_Type mpt, HelmetDevice_Type hdt, int offset)
{
    g_CenterOffsets[mpt][hdt] = offset;
}

static std::string MP2Str(MobilePhone_Type mpType)
{
#define CASE_FOR_TYPE(NAME) case NAME: str = #NAME; break;
    std::string str("");
    switch (mpType) {
        CASE_FOR_TYPE(MP_LETV_X1);
        CASE_FOR_TYPE(MP_LETV_X2);
        CASE_FOR_TYPE(MP_LETV_S2);
        CASE_FOR_TYPE(MP_LETV_X6);
        CASE_FOR_TYPE(MP_LETV_X7);
        CASE_FOR_TYPE(MP_LETV_X7_PLUS);
        CASE_FOR_TYPE(MP_LETV_VR_MAX1);
        CASE_FOR_TYPE(MP_LETV_X10);
        CASE_FOR_TYPE(MP_LETV_TURBO);
        CASE_FOR_TYPE(MP_LETV_ZL1);
    default:
        str = "unknow";
        break;
    }
    return str;
}

MobilePhone_Type getMobilePhoneType(const char * model)
{
    MobilePhone_Type type = MP_LETV_X2;
    if (!model)
        return type;

    static bool inited = false;
    if (!inited) {
        // X1
        registerModel("X800",           MP_LETV_X1);
        registerModel("X801",           MP_LETV_X1);

        // X2
        registerModel("Le X820",        MP_LETV_X2);
        registerModel("Le X822",        MP_LETV_X2);
        registerModel("Le X829",        MP_LETV_X2);
        registerModel("Le X829M",       MP_LETV_X2);

        // S2
        registerModel("Le X520",        MP_LETV_S2);
        registerModel("Le X521",        MP_LETV_S2);
        registerModel("Le X522",        MP_LETV_S2);
        registerModel("Le X522M",       MP_LETV_S2);
        registerModel("Le X525",        MP_LETV_S2);
        registerModel("Le X526",        MP_LETV_S2);
        registerModel("Le X527",        MP_LETV_S2);
        registerModel("Le X528",        MP_LETV_S2);
        registerModel("Le X529",        MP_LETV_S2);

        // X6
        registerModel("Le X620",        MP_LETV_X6);
        registerModel("Le X625",        MP_LETV_X6);
        registerModel("Le X621",        MP_LETV_X6);
        registerModel("LEX626",         MP_LETV_X6);
        registerModel("LEX622",         MP_LETV_X6);
        registerModel("LEX623",         MP_LETV_X6);
        registerModel("LEX636",         MP_LETV_X6);

        // X7
        registerModel("LEX650",         MP_LETV_X7);
        registerModel("LEX651",         MP_LETV_X7);
        registerModel("LEX652",         MP_LETV_X7);
        registerModel("LEX6520",        MP_LETV_X7);
        registerModel("LEX659",         MP_LETV_X7);
        registerModel("LEX6590",        MP_LETV_X7);
        registerModel("LEX656",         MP_LETV_X7);
        registerModel("LEX658",         MP_LETV_X7);
        registerModel("LEX657",         MP_LETV_X7);
        registerModel("LEX655",         MP_LETV_X7);
        registerModel("LEX653",         MP_LETV_X7);

        // X7 Plus
        registerModel("LEX550",         MP_LETV_X7_PLUS);
        registerModel("LEX5500",        MP_LETV_X7_PLUS);
        registerModel("LEX555",         MP_LETV_X7_PLUS);
        registerModel("LEX556",         MP_LETV_X7_PLUS);
        registerModel("LEX558",         MP_LETV_X7_PLUS);
        registerModel("LEX557",         MP_LETV_X7_PLUS);

        // MAX
        registerModel("LVR-M-001-AA",   MP_LETV_VR_MAX1);

        // X10
        registerModel("LEX850",         MP_LETV_X10);
        registerModel("LEX856",         MP_LETV_X10);
        registerModel("LEX858",         MP_LETV_X10);
        registerModel("LEX859",         MP_LETV_X10);

        // Turbo
        registerModel("LEX950",         MP_LETV_TURBO);
        registerModel("TBD",            MP_LETV_TURBO);

        // Zl1
        registerModel("LEX720",         MP_LETV_ZL1);
        registerModel("LEX721",         MP_LETV_ZL1);
        registerModel("LEX726",         MP_LETV_ZL1);
        registerModel("LEX728",         MP_LETV_ZL1);
        registerModel("LEX727",         MP_LETV_ZL1);
        registerModel("LEX725",         MP_LETV_ZL1);

        inited = true;
    }

    std::map<std::string, MobilePhone_Type>::iterator it = g_mps.find(model);
    if (it != g_mps.end())
        type = it->second;
    LOGI("MobilePhone: getMobilePhoneType(%s) = %s\n", model, MP2Str(type).c_str());
    return type;
}

// 手机中线偏移量，正值---向type c方向偏移，负值---向type c反方向偏移
int getCenterLineOffset(MobilePhone_Type mpt, HelmetDevice_Type hdt)
{
    static bool inited = false;
    if (!inited) {
        // X1
        initiateOffset(MP_LETV_X1, HD_LETV_UND0, 0);
        initiateOffset(MP_LETV_X1, HD_LETV_UND1, 0);
        initiateOffset(MP_LETV_X1, COOL_PRO, 34);
        initiateOffset(MP_LETV_X1, COOL_PRO_1S, 0);

        // X2
        initiateOffset(MP_LETV_X2, HD_LETV_UND0, 0);
        initiateOffset(MP_LETV_X2, HD_LETV_UND1, 0);
        initiateOffset(MP_LETV_X2, COOL_PRO, -68);
        initiateOffset(MP_LETV_X2, COOL_PRO_1S, -78);

        // S2
        initiateOffset(MP_LETV_S2, HD_LETV_UND0, 0);
        initiateOffset(MP_LETV_S2, HD_LETV_UND1, 0);
        initiateOffset(MP_LETV_S2, COOL_PRO, 54);
        initiateOffset(MP_LETV_S2, COOL_PRO_1S, 0);

        // X6
        initiateOffset(MP_LETV_X6, HD_LETV_UND0, 0);
        initiateOffset(MP_LETV_X6, HD_LETV_UND1, 0);
        initiateOffset(MP_LETV_X6, COOL_PRO, 24);
        initiateOffset(MP_LETV_X6, COOL_PRO_1S, 24);

        // X7
        initiateOffset(MP_LETV_X7, HD_LETV_UND0, 0);
        initiateOffset(MP_LETV_X7, HD_LETV_UND1, 0);
        initiateOffset(MP_LETV_X7, COOL_PRO, 10);
        initiateOffset(MP_LETV_X7, COOL_PRO_1S, 0);

        // X7 Plus
        initiateOffset(MP_LETV_X7_PLUS, HD_LETV_UND0, 0);
        initiateOffset(MP_LETV_X7_PLUS, HD_LETV_UND1, 0);
        initiateOffset(MP_LETV_X7_PLUS, COOL_PRO, 10);
        initiateOffset(MP_LETV_X7_PLUS, COOL_PRO_1S, 0);

        // Max
        initiateOffset(MP_LETV_VR_MAX1, HD_LETV_UND0, 0);
        initiateOffset(MP_LETV_VR_MAX1, HD_LETV_UND1, 0);
        initiateOffset(MP_LETV_VR_MAX1, COOL_PRO, 0);
        initiateOffset(MP_LETV_VR_MAX1, COOL_PRO_1S, 0);

        // X10
        initiateOffset(MP_LETV_X10, HD_LETV_UND0, 0);
        initiateOffset(MP_LETV_X10, HD_LETV_UND1, 0);
        initiateOffset(MP_LETV_X10, COOL_PRO, -58);
        initiateOffset(MP_LETV_X10, COOL_PRO_1S, 0);

        // Turbo
        initiateOffset(MP_LETV_TURBO, HD_LETV_UND0, 0);
        initiateOffset(MP_LETV_TURBO, HD_LETV_UND1, 0);
        initiateOffset(MP_LETV_TURBO, COOL_PRO, 8);
        initiateOffset(MP_LETV_TURBO, COOL_PRO_1S, 0);

        // Zl1
        initiateOffset(MP_LETV_ZL1, HD_LETV_UND0, 0);
        initiateOffset(MP_LETV_ZL1, HD_LETV_UND1, 0);
        initiateOffset(MP_LETV_ZL1, COOL_PRO, 20);
        initiateOffset(MP_LETV_ZL1, COOL_PRO_1S, 0);
        inited = true;
    }
    return g_CenterOffsets[mpt][hdt];
}

}
