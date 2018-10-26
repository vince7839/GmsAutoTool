#include "cmdtest.h"
#include<util/executor.h>
CmdTest::CmdTest(QString name, QString cmd, QString expect)
{
    this->name = name;
    this->cmd = cmd;
    this->expect = expect;
}

QString CmdTest::getName()
{
    return name;
}

QString CmdTest::getResult()
{
    return result;
}

QString CmdTest::getExpect()
{
    return expect;
}

SpecTest::status CmdTest::getStatus()
{
    return status;
}

void CmdTest::run()
{
    result = Executor::waitFinish(cmd).trimmed();
    status = result == expect ? PASS : FAIL;
}
