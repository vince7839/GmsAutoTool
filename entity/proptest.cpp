#include "proptest.h"
#include<util/specutil.h>
#include<QDebug>
const QString PropTest::PROP_LOW_RAM = "ro.config.low_ram";
const QString PropTest::PROP_GMS_VERSION = "ro.com.google.gmsversion";
const QString PropTest::PROP_BOARD = "ro.product.board";
const QString PropTest::PROP_BRAND = "ro.product.brand";
const QString PropTest::PROP_DEVICE = "ro.product.device";
const QString PropTest::PROP_MANUFACTURER = "ro.product.manufacturer";
const QString PropTest::PROP_MODEL = "ro.product.model";
const QString PropTest::PROP_NAME = "ro.product.name";
const QString PropTest::PROP_FINGERPRINT_1 = "ro.bootimage.build.fingerprint";
const QString PropTest::PROP_FINGERPRINT_2 = "ro.build.fingerprint";
const QString PropTest::PROP_FINGERPRINT_3 = "ro.vendor.build.fingerprint";
const QString PropTest::PROP_CLIENTID = "ro.com.google.clientidbase";
const QString PropTest::PROP_CLIENTID_AM = "ro.com.google.clientidbase.am";
const QString PropTest::PROP_CLIENTID_YT = "ro.com.google.clientidbase.yt";
const QString PropTest::PROP_API_LEVEL = "ro.product.first_api_level";
const QString PropTest::PROP_SECURITY = "ro.build.version.security_patch";
const QString PropTest::PROP_RELEASE = "ro.build.version.release";
const QString PropTest::PROP_DENSITY = "ro.sf.lcd_density";
const QString PropTest::PROP_INCREMENTAL = "ro.build.version.incremental";
const QString PropTest::PROP_VENDOR_MODEL = "ro.vendor.product.model";
const QString PropTest::PROP_VENDOR_DEVICE = "ro.vendor.product.device";
const QString PropTest::PROP_VENDOR_NAME = "ro.vendor.product.name";
const QString PropTest::PROP_VENDOR_MANUFACTURER = "ro.vendor.product.manufacturer";
const QString PropTest::PROP_VENDOR_BRAND = "ro.vendor.product.brand";

PropTest::PropTest(QString device, QString prop, QString name, QString expect)
{
    this->name = name;
    this->prop = prop;
    this->expect = expect;
    this->device = device;
    util = SpecUtil::getInstance(device);
    if(this->name.isEmpty()){
        this->name = prop;
    }
}

void PropTest::run()
{
    result = util->getProp(prop);
  if(result == expect && !expect.isEmpty() ){
        status = PASS;
    }else if(result != expect && !expect.isEmpty()){
        status = FAIL;
    }else{
      status = UNKNOWN;
  }
}

QString PropTest::getName()
{
    return name;
}

QString PropTest::getExpect()
{
    return expect;
}

QString PropTest::getResult()
{
    return result;
}

SpecTest::status PropTest::getStatus()
{
    return status;
}

