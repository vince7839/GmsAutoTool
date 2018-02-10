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
    void putInt(QString, QString, int);
    static const QString CONFIG;
    static const QString CONFIG_GRAB_SCREEN;
    static const QString CONFIG_SEND_FILE;
private:
    QSqlDatabase db;
    SqlConnection();
    static SqlConnection* mConn;
};

#endif // SQLCONNECTION_H
