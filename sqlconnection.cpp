#include "sqlconnection.h"
#include<QSqlDatabase>
#include<QList>
#include<QMap>
#include<QMessageBox>
#include<QSqlQuery>
#include<QSqlRecord>
#include<QSqlField>
#include<QDebug>
SqlConnection*SqlConnection::mConn;
SqlConnection::SqlConnection()
{
    //QString dbPath="/media/sf_虚拟机共享/AutoTool.db";
    QString dbPath="/home/liaowenxing/QtProject/GmsAutoTool.db";
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

}

bool SqlConnection::connect()
{
    if(!db.open())
     {
        return false;
     }
    return true;
}

void SqlConnection::close(){
    if(db.isOpen()) db.close();
}

SqlConnection* SqlConnection::getInstance()
{
    if(mConn == NULL)
    {
        mConn = new SqlConnection;
    }

    return mConn;
}

SqlConnection::~SqlConnection()
{
   close();
}

QList<QMap<QString,QString> > SqlConnection::execSql(QString str)
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
