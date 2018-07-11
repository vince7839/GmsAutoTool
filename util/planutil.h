#ifndef PLANUTIL_H
#define PLANUTIL_H

#include<QObject>
class PlanUtil
{
public:
    PlanUtil();
    static bool isPlanExists(QString,QString);
    static void createPlan(QString,QString,QSet<QString>);
    static void execPlan(QString,QString);
};

#endif // PLANUTIL_H
