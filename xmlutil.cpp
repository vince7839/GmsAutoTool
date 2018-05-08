#include "xmlutil.h"
#include<QString>
#include<QDomDocument>
#include<QDomNode>
#include<QFile>
#include<QDebug>


XmlUtil::XmlUtil(QString path)
{
    doc.setContent(new QFile("config/Config.xml"));
}

QString XmlUtil::getNodeText(QString nodeName,QString attrs[], QString attrValues[])
{
    parseChildren(doc.documentElement());
    return mResult;
}

void XmlUtil::parseNode(QDomNode node)
{
  for(int i=0;i<sizeof(mAttrs);i++)
  {
      if(node.attributes().namedItem(mAttrs[i]).nodeValue()
              == mAttrValues[i] )
          mResult=node.toElement().text();
  }
}

void XmlUtil::parseChildren(QDomNode node)
{
    parseNode(node);
    if( node.hasChildNodes())
    {
        for(int i=0;i<node.childNodes().size();i++)
        {
            parseChildren(node.childNodes().at(i));
        }
    }
}

QDomNode XmlUtil::getChildNode(QDomNode root, QString name, QMap<QString, QString> attr)
{
    QStringList attrNames = attr.keys();
    for(int i=0;i<root.childNodes().size();i++)
    {
        QDomNode child = root.childNodes().at(i);
        if(child.nodeName() == name)
        {
            for(int j=0;j<attrNames.size();j++)
            {
                QString key = attrNames.at(j);
                if(child.attributes().namedItem(key).nodeValue()
                    != attr.value(key))
                    break;
                if(j == attrNames.size() - 1)
                    return child;
            }
         }
    }
    return QDomNode();
}
