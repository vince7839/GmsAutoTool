#include "sqlconnection.h"
#include<QSqlDatabase>
#include<QList>
#include<QMap>
#include<QMessageBox>
#include<QSqlQuery>
#include<QSqlRecord>
#include<QSqlField>
#include<QDebug>
#include<QMessageBox>
SqlConnection*SqlConnection::sInstance;
const QString SqlConnection::CONFIG = "Config";
const QString SqlConnection::CONFIG_GRAB_SCREEN = "allow_grab_screen";
const QString SqlConnection::CONFIG_SEND_FILE = "allow_send_file";
SqlConnection::SqlConnection()
{   
    QString dbPath = "database/GmsAutoTool.db";
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);
}

bool SqlConnection::connect()
{
    if(!db.open())
    {
        QMessageBox::warning(NULL,QString::fromUtf8("警告"),QString::fromUtf8("连接数据库失败！"));
        return false;
    }
    return true;
}

void SqlConnection::close(){
    if(db.isOpen()) db.close();
}

SqlConnection* SqlConnection::getInstance()
{
    if(sInstance == NULL)
    {
        sInstance = new SqlConnection;
    }

    return sInstance;
}

SqlConnection::~SqlConnection()
{
   close();
}

QList<QMap<QString,QString> > SqlConnection::exec(QString str)
{
    QList<QMap<QString,QString> > list;
    QSqlQuery query(db);
     query.exec(str);
     while(query.next()){
         QSqlRecord record=query.record();
         QMap<QString,QString> map;
         for(int i=0;i<record.count();i++)
         {
             map.insert(record.field(i).name(),record.value(i).toString());
         }
     list.append(map);
     }

     return list;
}

void SqlConnection::putInt(QString table,QString projection,int value)
{
    QString query = QString("update %1 set %2 = %3 where id = %4")
                    .arg(table).arg(projection).arg(value).arg(0);
    exec(query);
}
