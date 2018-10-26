#ifndef FEATURETEST_H
#define FEATURETEST_H

#include<entity/spectest.h>

class FeatureTest : public SpecTest
{
public:
    FeatureTest(QString device,QString feature,QString name,QString expect = "");
    QString getName();
    QString getResult();
    QString getExpect();
    status getStatus();
    void run();
private:
    QString device;
    QString feature;
    QString expect;
    QString result;
    status status;
    QString name;
};

#endif // FEATURETEST_H
