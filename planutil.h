#ifndef PLANUTIL_H
#define PLANUTIL_H

#include<QObject>
class PlanUtil
{
public:
    PlanUtil();
    static bool isPlanExists(QString,QString);
    static void CreatePlan(QString,QString,QSet<QString>);
};

#endif // PLANUTIL_H
