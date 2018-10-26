#ifndef CMDTEST_H
#define CMDTEST_H

#include<entity/spectest.h>

class CmdTest : public SpecTest
{
public:
    CmdTest(QString name,QString cmd,QString expect = "");
    QString getName();
    QString getResult();
    QString getExpect();
    status getStatus();
    void run();
private:
    QString name;
    QString cmd;
    QString expect;
    QString result;
    status status;
};

#endif // CMDTEST_H
