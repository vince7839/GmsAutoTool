#ifndef SQLCONNECTION_H
#define SQLCONNECTION_H
#include<QSqlDatabase>
#include<QMap>

class SqlConnection
{

public:
    static SqlConnection* getInstance();
    ~SqlConnection();
    bool isConnect();
    void close();
    QList<QMap<QString,QString> > exec(QString);
    void putInt(QString, QString, int);
    static const QString CONFIG;
    static const QString CONFIG_GRAB_SCREEN;
    static const QString CONFIG_SEND_FILE;
private:
    QSqlDatabase db;
    SqlConnection();
    static SqlConnection* sInstance;
};

#endif // SQLCONNECTION_H
