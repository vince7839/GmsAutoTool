#include "specbuilder.h"
#include<entity/spectest.h>
#include<entity/proptest.h>
#include<entity/packagetest.h>
#include<util/specutil.h>
#include<util/spectestthread.h>
#include<view/specwidget.h>
#include<entity/defaulttest.h>
#include<QDebug>
#include<entity/cmdtest.h>
#include<entity/equaltest.h>
#include<entity/intenttest.h>
#include<entity/featuretest.h>
SpecBuilder::SpecBuilder()
{

}

SpecBuilder *SpecBuilder::buildPhoneType(int type)
{
    this->phoneType = type;
    return this;
}

SpecBuilder *SpecBuilder::buildMpType(bool isMp)
{
    this->isMp = isMp;
    return this;
}

SpecBuilder *SpecBuilder::buildDevice(QString device)
{
    this->device = device;
    return this;
}

SpecBuilder *SpecBuilder::buildExpress(bool isExpress)
{
    this->isExpress = isExpress;
    return this;
}

QList<SpecTest *> SpecBuilder::create()
{
    QList<SpecTest*> list;
    SpecUtil*util =  SpecUtil::getInstance(device);
    list.append(new DefaultTest(device,DefaultTest::SPEC_IS_GMS_GO));
    list.append(new FeatureTest(device,"com.google.android.feature.EEA_DEVICE",QString::fromUtf8("com.google.android.feature.EEA_DEVICE(欧盟)")));
    list.append(new FeatureTest(device,"com.google.android.paid.search",QString::fromUtf8("com.google.android.paid.search(欧盟)")));
    list.append(new FeatureTest(device,"com.google.android.paid.chrome",QString::fromUtf8("com.google.android.paid.chrome(欧盟)")));
    list.append(new DefaultTest(device,DefaultTest::SPEC_FINGERPRINT_USER,"Yes"));
    list.append(new DefaultTest(device,DefaultTest::SPEC_FINGERPRINT_RELEASE,"Yes"));
    list.append(new DefaultTest(device,DefaultTest::SPEC_FINGERPRINT_NAME,"Yes"));
    list.append(new DefaultTest(device,DefaultTest::SPEC_FINGERPRINT_DEVICE,"Yes"));
    list.append(new PropTest(device,PropTest::PROP_LOW_RAM,QString::fromUtf8("是否低内存（LOW RAM）"),util->isGoVersion() ? "true":"false"));
    list.append(new PropTest(device,PropTest::PROP_SECURITY,QString::fromUtf8("安全Patch")));
    list.append(new PropTest(device,PropTest::PROP_BOARD));
    list.append(new PropTest(device,PropTest::PROP_BRAND));
    list.append(new PropTest(device,PropTest::PROP_DEVICE));
    list.append(new PropTest(device,PropTest::PROP_MANUFACTURER));
    list.append(new PropTest(device,PropTest::PROP_MODEL));
    list.append(new PropTest(device,PropTest::PROP_NAME));
    list.append(new PropTest(device,PropTest::PROP_GMS_VERSION,QString::fromUtf8("GMS包版本")));
    list.append(new PropTest(device,PropTest::PROP_FINGERPRINT_1));
    list.append(new PropTest(device,PropTest::PROP_FINGERPRINT_2));
    list.append(new PropTest(device,PropTest::PROP_FINGERPRINT_3));
    list.append(new DefaultTest(device,DefaultTest::SPEC_FINGERPRINT_SAME,"Yes"));
    list.append(new PropTest(device,PropTest::PROP_CLIENTID));
    list.append(new PropTest(device,PropTest::PROP_CLIENTID_AM));
    list.append(new PropTest(device,PropTest::PROP_CLIENTID_YT));
    list.append(new EqualTest(QString::fromUtf8("两个model属性是否相同"),util->getProp(PropTest::PROP_MODEL),util->getProp(PropTest::PROP_VENDOR_MODEL),true));
    list.append(new EqualTest(QString::fromUtf8("两个device属性是否相同"),util->getProp(PropTest::PROP_DEVICE),util->getProp(PropTest::PROP_VENDOR_DEVICE),true));
    list.append(new EqualTest(QString::fromUtf8("两个name属性是否相同"),util->getProp(PropTest::PROP_NAME),util->getProp(PropTest::PROP_VENDOR_NAME),true));
    list.append(new EqualTest(QString::fromUtf8("两个manufacturer属性是否相同"),util->getProp(PropTest::PROP_MANUFACTURER),util->getProp(PropTest::PROP_VENDOR_MANUFACTURER),true));
    list.append(new EqualTest(QString::fromUtf8("两个brand属性是否相同"),util->getProp(PropTest::PROP_BRAND),util->getProp(PropTest::PROP_VENDOR_BRAND),true));
    list.append(new PropTest(device,PropTest::PROP_API_LEVEL,QString::fromUtf8("First API Level"),isMp ? "" : util->expectApiLevel()));

    QString containExpect = "";
    if(phoneType == SMART_FEATURE_PHONE){
        containExpect = "No";
    }else if(phoneType == GMS_NOT_REQUIRED){
        bool hasGms = !util->getProp(PropTest::PROP_GMS_VERSION).isEmpty();
        qDebug()<<"-------------------has gms:"<<hasGms;
        containExpect = hasGms ? "No" : "Yes";
    }else{
        containExpect = "Yes";
    }
    list.append(new DefaultTest(device,DefaultTest::SPEC_COMPANY_LIMIT,containExpect));


    if(util->isGoVersion()){
        list.append(new PackageTest(device,PackageTest::GOOGLE_GO));
        list.append(new PackageTest(device,PackageTest::YOUTUBE_GO));
        list.append(new PackageTest(device,PackageTest::GMAIL_GO));
        list.append(new PackageTest(device,PackageTest::MAPS_GO));
        list.append(new PackageTest(device,PackageTest::ASSITANT_GO));
    }else{
        list.append(new PackageTest(device,PackageTest::GOOGLE));
        list.append(new PackageTest(device,PackageTest::YOUTUBE));
        list.append(new PackageTest(device,PackageTest::GMAIL));
        list.append(new PackageTest(device,PackageTest::MAPS));
        list.append(new PackageTest(device,PackageTest::DRIVE));
        list.append(new PackageTest(device,PackageTest::PLAY_MUSIC));
        list.append(new PackageTest(device,PackageTest::PLAY_MOVIES));
        list.append(new PackageTest(device,PackageTest::PHOTOS));
        list.append(new PackageTest(device,PackageTest::DUO));
    }
    list.append(new PackageTest(device,PackageTest::CHROME));
    list.append(new PackageTest(device,PackageTest::PLAY_STORE));
    list.append(new DefaultTest(device,DefaultTest::SPEC_DATA_SIZE));
    list.append(new PackageTest(device,PackageTest::LOVELY_FONT,false));
    list.append(new PackageTest(device,PackageTest::NAV_GO,true));
    if(isExpress){

        if(util->isGoVersion()){

            list.append(new DefaultTest(device,DefaultTest::SPEC_RAM_1G,"Yes"));
            list.append(new IntentTest(device,IntentTest::Keyboard_1));
             list.append(new IntentTest(device,IntentTest::Voice_Assistant_Go));
        }else{
            list.append(new DefaultTest(device,DefaultTest::SPEC_RAM_1G,"No"));
            list.append(new IntentTest(device,IntentTest::Gallery_1));
            list.append(new IntentTest(device,IntentTest::Gallery_2));
            list.append(new IntentTest(device,IntentTest::Gallery_3));
            list.append(new IntentTest(device,IntentTest::Gallery_4));
            list.append(new IntentTest(device,IntentTest::Calendar));
             list.append(new IntentTest(device,IntentTest::Voice_Assistant));
        }
        list.append(new IntentTest(device,IntentTest::Keyboard_2));
        list.append(new IntentTest(device,IntentTest::Browser_1));
        list.append(new IntentTest(device,IntentTest::Browser_2));
        list.append(new IntentTest(device,IntentTest::Messaging_1));
        list.append(new IntentTest(device,IntentTest::Messaging_1));
        list.append(new IntentTest(device,IntentTest::Email_1));
        list.append(new IntentTest(device,IntentTest::Email_2));
        list.append(new IntentTest(device,IntentTest::Email_3));
        list.append(new IntentTest(device,IntentTest::Email_4));
        list.append(new IntentTest(device,IntentTest::Email_5));
        list.append(new IntentTest(device,IntentTest::Search));
        list.append(new DefaultTest(device,DefaultTest::SPEC_IS_EXPRESS));
        list.append(new DefaultTest(device,DefaultTest::SPEC_PATCH_VALID,"Yes"));
        list.append(new DefaultTest(device,DefaultTest::SPEC_FINGERPRINT_LIMIT,"Yes"));
        list.append(new DefaultTest(device,DefaultTest::SPEC_MANUFACTURER_LIMIT,"Yes"));
        list.append(new DefaultTest(device,DefaultTest::SPEC_MTKLOG_VERSION,"Yes"));
        list.append(new DefaultTest(device,DefaultTest::SPEC_FOTA_VERSION,"Yes"));
        list.append(new DefaultTest(device,DefaultTest::SPEC_FINGERPRINT_INCREMENTAL));
        list.append(new DefaultTest(device,DefaultTest::SPEC_LOCATION_MODE));
        list.append(new DefaultTest(device,DefaultTest::SPEC_BLUETOOTH_OFF));
        list.append(new DefaultTest(device,DefaultTest::SPEC_ROTATION_OFF));
        list.append(new PackageTest(device,"com.android.wallpaper.livepicker",false));
        list.append(new PackageTest(device,"com.android.facelock",false));
    }
  //  list.append(new DefaultTest(device,DefaultTest::SPEC_OEM_UNLOCK));
    return list;
}
