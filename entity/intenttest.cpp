#include "intenttest.h"
#include<util/executor.h>
#include<QDebug>
#include<entity/packagetest.h>
#include<util/specutil.h>
IntentTest::IntentTest(QString device,IntentTest::IntentType type)
{
    this->device = device;
    this->type = type;
    util = SpecUtil::getInstance(device);
}

void IntentTest::run()
{
    QString intent;
    QString package;
    switch(type){
    case Browser_1:
        expect = "1";
        name = QString::fromUtf8("Browser测试1");
        package = PackageTest::CHROME;
        intent = QString("adb -s %1 shell am start -W -a android.intent.action.VIEW -d 'https://www.wikipedia.org/'|grep -c %2").arg(device).arg(package);
        break;
    case Browser_2:
        expect = "1";
        name = QString::fromUtf8("Browser测试2");
        package = PackageTest::CHROME;
        intent = QString("adb -s %1 shell am start -W -a android.intent.action.VIEW -d 'http://www.wikipedia.org/'|grep -c %2").arg(device).arg(package);
        break;
    case Gallery_1:
        expect = "1";
        name = QString::fromUtf8("Gallery测试1");
        package = PackageTest::PHOTOS;
        intent = QString("adb -s %1 shell am start -W -t image/* file://tmp/cute.jpg|grep -c %2").arg(device).arg(package);
        break;
    case Gallery_2:
        expect = "1";
        name = QString::fromUtf8("Gallery测试2");
        package = PackageTest::PHOTOS;
        intent = QString("adb -s %1 shell am start -W -t image/* file://tmp/cute.mp4|grep -c %2").arg(device).arg(package);
        break;
    case Gallery_3:
        expect = "1";
        name = QString::fromUtf8("Gallery测试3");
        package = PackageTest::PHOTOS;
        intent = QString("adb -s %1 shell am start -W -a android.intent.action.PICK -t image/*|grep -c %2").arg(device).arg(package);
        break;
    case Gallery_4:
        expect = "1";
        name = QString::fromUtf8("Gallery测试4");
        package = PackageTest::PHOTOS;
        intent = QString("adb -s %1 shell am start -W -a com.android.camera.action.REVIEW -t image/*|grep -c %2").arg(device).arg(package);
        break;
    case Messaging_1:
        expect = "1";
        name = QString::fromUtf8("Messaging测试1");
        package = "com.google.android.apps.messaging";
       intent = QString("adb -s %1 shell am start -W -a android.intent.action.SENDTO -d sms:CCXXXXXXXXXX|grep -c %2").arg(device).arg(package);
        break;
    case Messaging_2:
        expect = "1";
        name = QString::fromUtf8("Messaging测试2");
        package = "com.google.android.apps.messaging";
        intent = QString("adb -s %1 shell am start -W -a android.intent.action.SENDTO -d smsto:CCXXXXXXXXXX|grep -c %2").arg(device).arg(package);
        break;
    case Email_1:
        name = QString::fromUtf8("Email测试1");
        expect = "1";
        package = util->isGoVersion() ? PackageTest::GMAIL_GO : PackageTest::GMAIL;
        intent  = QString("adb -s %1 shell am start -W -a android.intent.action.SENDTO -d mailto:someone@gmail.com|grep -c %2").arg(device).arg(package);
        break;
    case Email_2:
        name = QString::fromUtf8("Email测试2");
        expect = "1";
        package = util->isGoVersion() ? PackageTest::GMAIL_GO : PackageTest::GMAIL;
        intent  = QString("adb -s %1 shell dumpsys package %2 | grep -c 'android.permission.READ_CALENDAR: granted=true'").arg(device).arg(package);
        break;
    case Email_3:
        name = QString::fromUtf8("Email测试3");
        expect = "1";
        package = util->isGoVersion() ? PackageTest::GMAIL_GO : PackageTest::GMAIL;
        intent  = QString("adb -s %1 shell dumpsys package %2 | grep -c 'android.permission.WRITE_CALENDAR: granted=true'").arg(device).arg(package);
        break;
    case Email_4:
        name = QString::fromUtf8("Email测试4");
        expect = "1";
        package = util->isGoVersion() ? PackageTest::GMAIL_GO : PackageTest::GMAIL;
        intent  = QString("adb -s %1 shell dumpsys package %2 | grep -c 'android.permission.READ_CONTACTS: granted=true'").arg(device).arg(package);
        break;
    case Email_5:
        name = QString::fromUtf8("Email测试5");
        package = util->isGoVersion() ? PackageTest::GMAIL_GO : PackageTest::GMAIL;
        expect = "1";
        intent  = QString("adb -s %1 shell dumpsys package %2 | grep -c 'android.permission.WRITE_CONTACTS: granted=true'").arg(device).arg(package);
        break;
    case Keyboard_1:
        expect = "1";
        name = QString::fromUtf8("keyboard测试1");
        intent  = QString("adb -s %1 shell settings get secure default_input_method|grep -c com.google.android.inputmethod").arg(device);
        break;
    case Keyboard_2:
        expect = "0";
        name = QString::fromUtf8("keyboard测试2");
        intent  = QString("adb -s %1 shell ime list -a | grep mId | grep -v -c mId=com.google.android").arg(device);
        break;
    case Voice_Assistant:
        expect = "1";
        name = QString::fromUtf8("Voice Assistant测试");
        package  = "com.google.android.googlequicksearchbox";
        intent  = QString("adb -s 1% shell am start -W -a android.intent.action.VOICE_COMMAND|grep -c %2").arg(device).arg(package);
        break;
    case Voice_Assistant_Go:
        expect = "1";
        name = QString::fromUtf8("Voice Assistant测试");
        package = "com.google.android.apps.assistant";
        intent  = QString("adb -s %1 shell am start -W -a android.intent.action.ASSIST|grep -c %2").arg(device).arg(package);
        break;
    case Search:
        name = QString::fromUtf8("Search测试");
        expect = "1";
        package = util->isGoVersion() ? "com.google.android.apps.searchlite" : "com.google.android.googlequicksearchbox";
        intent  = QString("adb -s %1 shell am start -W -a android.intent.action.WEB_SEARCH -e query wikipedia|grep -c %2").arg(device).arg(package);
        break;
      case Calendar:
        expect = "1";
        name = QString::fromUtf8("Calendar测试");
        package = "com.google.android.calendar";
        intent = QString("adb -s %1 shell am start -W -a android.intent.action.VIEW -d content://com.android.calendar/time/1410665898789|grep -c %2").arg(device).arg(package);
    }
    if(!package.isEmpty()){
        Executor::waitFinish(QString("adb shell am force-stop %1").arg(package));
    }
    result = Executor::waitFinish(intent).simplified();
    status = result == expect ? PASS : FAIL;
    qDebug()<<"IntentTest::run:"<<result;
}

QString IntentTest::getName()
{
    return name;
}

QString IntentTest::getResult()
{
    return result;
}

QString IntentTest::getExpect()
{
    return expect;
}

SpecTest::status IntentTest::getStatus()
{
    return status;
}
