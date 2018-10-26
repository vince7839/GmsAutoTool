#ifndef EQUALTEST_H
#define EQUALTEST_H

#include<QObject>
#include<entity/spectest.h>

class EqualTest : public SpecTest
{
public:
    EqualTest(QString name,QString a,QString b,bool expect=true);
    QString getName();
    QString getResult();
    QString getExpect();
    status getStatus();
    void run();
private:
    bool expect;
    QString name;
    QString result;
    status status;
};

#endif // EQUALTEST_H
