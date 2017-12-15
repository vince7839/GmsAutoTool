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

QDomNode XmlUtil::getChildNode(QDomNode node, QStringList attrs, QStringList attrValues)
{
    for(int i=0;i<node.childNodes().size();i++)
    {
        for(int j=0;j<attrs.size();j++)
        {
            if(node.childNodes().at(i).attributes().namedItem(attrs.at(j)).nodeValue()
                != attrValues.at(j))
                break;
            if(j == attrs.size()-1) return node.childNodes().at(i);
        }

    }

    return QDomNode();
}
