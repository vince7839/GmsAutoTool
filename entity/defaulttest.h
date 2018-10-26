#ifndef DEFAULTTEST_H
#define DEFAULTTEST_H

#include<entity/spectest.h>
#include<util/specutil.h>
class DefaultTest : public SpecTest
{
public:
    static const QString FEATURE_RU;
    enum SpecType{
        SPEC_IS_GMS_GO,
        SPEC_IS_RU,
        SPEC_IS_EXPRESS,
        SPEC_FINGERPRINT_INCREMENTAL,
        SPEC_FINGERPRINT_USER,
        SPEC_FINGERPRINT_RELEASE,
        SPEC_FINGERPRINT_NAME,
        SPEC_FINGERPRINT_DEVICE,
        SPEC_FINGERPRINT_SAME,
        SPEC_FINGERPRINT_LIMIT,
        SPEC_MANUFACTURER_LIMIT,
        SPEC_OEM_UNLOCK,
        SPEC_RAM_1G,
        SPEC_PATCH_VALID,
        SPEC_MTKLOG_VERSION,
        SPEC_FOTA_VERSION,
        SPEC_DATA_SIZE,
        SPEC_LOCATION_MODE,
        SPEC_BLUETOOTH_OFF,
        SPEC_ROTATION_OFF,
        SPEC_COMPANY_LIMIT
    };
    DefaultTest(QString device,SpecType type,QString expect = "");
    void run();
    QString getName();
    QString getExpect();
    QString getResult();
    status getStatus();
private:
    SpecType type;
    QString result;
    QString expect;
    QString name;
    status status = SpecTest::UNKNOWN;
    QString device;
    SpecUtil*util;
};

#endif // DEFAULTTEST_H
