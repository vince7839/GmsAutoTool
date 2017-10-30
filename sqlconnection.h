#ifndef SQLCONNECTION_H
#define SQLCONNECTION_H
#include<QSqlDatabase>

class SqlConnection
{
public:
    SqlConnection();
    ~SqlConnection();
    bool connect();
    QList<QMap<QString,QString> > execSql(QString);
private:
    QSqlDatabase db;
};

#endif // SQLCONNECTION_H
