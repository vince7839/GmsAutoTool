#include "packagetest.h"
#include<util/specutil.h>
#include<util/specutil.h>
 const QString PackageTest::ASSITANT_GO = "com.google.android.apps.assistant";
 const QString PackageTest::CHROME = "com.android.chrome";
  const QString PackageTest::PLAY_STORE = "com.android.vending";
 const QString PackageTest::MAPS_GO = "com.google.android.apps.mapslite";
 const QString PackageTest::GMAIL_GO = "com.google.android.gm.lite";
 const QString PackageTest::GOOGLE_GO = "com.google.android.apps.searchlite";
 const QString PackageTest::YOUTUBE_GO = "com.google.android.apps.youtube.mango";
 const QString PackageTest::MAPS = "com.google.android.apps.maps";
 const QString PackageTest::GMAIL = "com.google.android.gm";
 const QString PackageTest::GOOGLE = "com.google.android.googlequicksearchbox";
 const QString PackageTest::YOUTUBE = "com.google.android.youtube";
 const QString PackageTest::DRIVE = "com.google.android.apps.docs";
 const QString PackageTest::PLAY_MUSIC = "com.google.android.music";
 const QString PackageTest::PLAY_MOVIES = "com.google.android.videos";
 const QString PackageTest::PHOTOS = "com.google.android.apps.photos";
 const QString PackageTest::DUO = "com.google.android.apps.tachyon";
  const QString PackageTest::LIVE_PAPER = "com.android.wallpaper.livepicker";
  const QString PackageTest::FACE_LOCK = "com.android.facelock";
  const QString PackageTest::LOVELY_FONT = "com.valmedia.fdelux";
  const QString PackageTest::NAV_GO="com.google.android.apps.navlite";

PackageTest::PackageTest(QString device, QString package, bool expect)
{
    this->package = package;
    this->expect = expect;
    this->device = device;
    util = SpecUtil::getInstance(device);
    names.insert(ASSITANT_GO,QString::fromUtf8("Assistant Go检测"));
    names.insert(CHROME,QString::fromUtf8("Chrome检测"));
    names.insert(PLAY_STORE,QString::fromUtf8("Play Store检测"));
    names.insert(MAPS_GO,QString::fromUtf8("Maps Go检测"));
    names.insert(GMAIL_GO,QString::fromUtf8("Gmail Go检测"));
    names.insert(GOOGLE,QString::fromUtf8("Google检测"));
    names.insert(GOOGLE_GO,QString::fromUtf8("Google Go检测"));
    names.insert(YOUTUBE_GO,QString::fromUtf8("Youtube Go检测"));
    names.insert(MAPS,QString::fromUtf8("Maps检测"));
    names.insert(GMAIL,QString::fromUtf8("Gmail检测"));
    names.insert(YOUTUBE,QString::fromUtf8("Youtube检测"));
    names.insert(DRIVE,QString::fromUtf8("Drive检测"));
    names.insert(PLAY_MUSIC,QString::fromUtf8("Play Music检测"));
    names.insert(PLAY_MOVIES,QString::fromUtf8("Play Movies检测"));
    names.insert(PHOTOS,QString::fromUtf8("Photo检测"));
    names.insert(DUO,QString::fromUtf8("Duo检测"));
    names.insert(LIVE_PAPER,QString::fromUtf8("Live Paper检测"));
    names.insert(FACE_LOCK,QString::fromUtf8("Face Lock检测"));
    names.insert(LOVELY_FONT,QString::fromUtf8("Lovely Font检测"));
    names.insert(NAV_GO,QString::fromUtf8("Nav Go检测"));
    name = names.value(package);
}

void PackageTest::run()
{
    bool exist = util->hasPackage(package) ;
    result = QString::fromUtf8(exist ? "已预置" : "未预置");
    status = exist == expect ? PASS : FAIL;
}

QString PackageTest::getName()
{
    return name;
}

QString PackageTest::getExpect()
{
    return QString::fromUtf8(expect ? "预置" : "未预置");;
}

QString PackageTest::getResult()
{
    return result;
}

SpecTest::status PackageTest::getStatus()
{
    return status;
}
