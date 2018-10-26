#ifndef INTENTTEST_H
#define INTENTTEST_H

#include <QObject>
#include<entity/spectest.h>
#include<util/specutil.h>
class IntentTest : public SpecTest
{
public:
    enum IntentType{
        Browser_1,
        Browser_2,
        Gallery_1,
        Gallery_2,
        Gallery_3,
        Gallery_4,
        Messaging_1,
        Messaging_2,
        Email_1,
        Email_2,
        Email_3,
        Email_4,
        Email_5,
        Keyboard_1,
        Keyboard_2,
        Voice_Assistant,
        Voice_Assistant_Go,
        Search,
        Calendar
    };
    IntentTest(QString device,IntentType type);
    void run();
    QString getName();
    QString getResult();
    QString getExpect();
    SpecTest::status getStatus();
private:
    QString name;
    QString expect;
    QString result;
    status status;
    QString device;
    IntentType type;
    SpecUtil*util;
};

#endif // INTENTTEST_H
