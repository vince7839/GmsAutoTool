#ifndef PLANUTIL_H
#define PLANUTIL_H

#include<QObject>
class PlanUtil
{
public:
    PlanUtil();
    static bool isPlanExists(QString toolPath,QString planName);
    static void createPlan(QString toolPath,QString planName,QSet<QString>items);
    static QString createAutoPlan(QString toolPath,QSet<QString>items);
    static void execPlan(QString toolPath,QString planName);
    static void copyPlan(QString toolPath,QString xmlPath);
    static QString getPlanDirPath(QString toolPath);
};

#endif // PLANUTIL_H
