#include "featuretest.h"
#include<util/specutil.h>

FeatureTest::FeatureTest(QString device, QString feature, QString name, QString expect)
{

    this->device = device;
    this->feature = feature;
    this->expect = expect;
    this->name = name;
}

QString FeatureTest::getName()
{
    return name;
}

QString FeatureTest::getResult()
{
    return result;
}

QString FeatureTest::getExpect()
{
    return expect;
}

SpecTest::status FeatureTest::getStatus()
{
    return UNKNOWN;
}

void FeatureTest::run()
{
    SpecUtil* util = SpecUtil::getInstance(device);
    result = QString::fromUtf8(util->hasFeature(feature) ? "已声明" : "未声明");
}
