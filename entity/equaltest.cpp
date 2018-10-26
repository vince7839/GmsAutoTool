#include "equaltest.h"

EqualTest::EqualTest(QString name, QString a, QString b, bool expect)
{
    this->expect = expect;
    result = QString("%1 %2 %3").arg(a).arg(QString::fromUtf8(a ==b ? "等于":"不等于")).arg(b);
    status = a == b ? PASS : FAIL;
    this->name = name;    
}

QString EqualTest::getName()
{
    return name;
}

QString EqualTest::getResult()
{
    return result;
}

QString EqualTest::getExpect()
{
    return expect ? "Yes" : "No";
}

SpecTest::status EqualTest::getStatus()
{
    return status;
}

void EqualTest::run()
{

}
