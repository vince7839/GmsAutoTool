#ifndef SQLCONNECTION_H
#define SQLCONNECTION_H
#include<QSqlDatabase>

class SqlConnection
{

public:
    static SqlConnection* getInstance();
    ~SqlConnection();
    bool connect();
    void close();
    QList<QMap<QString,QString> > execSql(QString);
    static QList<QMap<QString,QString> > RemoteExec(QString);
private:
    QSqlDatabase db;
    SqlConnection();
    static SqlConnection* mConn;
};

#endif // SQLCONNECTION_H
