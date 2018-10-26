#ifndef CMDBUILDER_H
#define CMDBUILDER_H

#include <QObject>
#include<util/taskparam.h>
#include<QDomNode>
#include<QDebug>
class CmdBuilder;
class CmdBuilder
{
public:

    class Cmd{
    public:
        Cmd(QString c,QString p,QString v){
            cmd = c;
            version = v;
            platform = p;
        }
        QString getCmd(){
            return cmd;
        }
        QString getPlatform(){
            return platform;
        }
        QString getVersion(){
            return version;
        }

        bool beforeThan(QString p,QString v){
            return magicNumber(platform,version) <= magicNumber(p,v);
        }
    private:
        float magicNumber(QString p,QString v){
            bool ok = false;
            float platNum = p.toFloat(&ok);
            float magic = ok ? platNum : 0;
            QRegExp re("[RrCc]+([0-9]+)");
            if(re.exactMatch(v)){
                ok = false;
                float verNum = re.cap(1).toFloat(&ok);
                magic += ok ? (verNum/1000) : 0;
            }
          //  qDebug()<<"[CmdBuilder::Cmd::magicNumber]"<<p<<v<<magic;
            return magic;
        }
        QString platform;
        QString version;
        QString cmd;
    };
    CmdBuilder();
    CmdBuilder* buildTaskCmd(TaskParam* taskParam);
    CmdBuilder* buildShell();
    CmdBuilder *buildProp(QString prop);
    CmdBuilder*buildAdb(QString device);
    CmdBuilder* buildFeature();
    CmdBuilder* buildPackage();
    CmdBuilder* buildAmStart();
    CmdBuilder* buildAction(QString action);
    CmdBuilder* buildData(QString data);
    CmdBuilder* buildType(QString type,QString file);
    QString create();
    void test();
    static QList<Cmd*> actionCmds(QString type,QString action);
    static QList<Cmd*> sortCmds(QList<Cmd*> list);
    static QString getActionCmd(QString type,QString action,QString platform,QString version);
private:
    TaskParam* taskParam = NULL;
    QString cmd;
};

#endif // CMDBUILDER_H
