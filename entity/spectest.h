#ifndef SPECTEST_H
#define SPECTEST_H
#include<QMetaType>
#include<QObject>

class SpecTest : public QObject{

public:
    enum status{
        PASS,
        FAIL,
        UNKNOWN
    };
    virtual void run() = 0;
    virtual QString getName() = 0;
    virtual QString getExpect() = 0;
    virtual QString getResult() = 0;
    virtual status getStatus() = 0;
};
Q_DECLARE_METATYPE(SpecTest*)
#endif // SPECTEST_H
