#ifndef XMLUTIL_H
#define XMLUTIL_H
#include<QDomDocument>
#include<QDomNode>
#include<QString>
#include<QMap>

class XmlUtil
{
public:
    XmlUtil(QString);
    QString getNodeText(QString nodeName,QMap<QString,QString>selection);
    QString getNodeText(QString nodeName,QString[],QString[]);
    void parseNode(QDomNode);
    void parseChildren(QDomNode);
    static QDomNode getChildNode(QDomNode,QString,QMap<QString,QString> map = QMap<QString,QString>());
    static QList<QDomNode> getNodes(QDomNode root,QString name,QMap<QString,QString>attrs);
private:
    QDomDocument doc;
    QString mAttrs[];
    QString mAttrValues[];
    QString mResult;
};

#endif // XMLUTIL_H
