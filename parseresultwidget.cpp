#include "parseresultwidget.h"
#include "ui_parseresultwidget.h"
#include "QDomDocument"
#include "QFile"
#include "QDebug"

void ParseResultWidget::parseXml(QDomNode node)
{
    if(node.hasChildNodes()){
        QDomNodeList childNodes=node.childNodes();
        for(int i=0;i<childNodes.size();i++){
            parseXml(childNodes.at(i));
        }
    }else{
       if(node.attributes().namedItem("result").nodeValue()=="pass"){
            //qDebug()<<node.attributes().namedItem("name").nodeValue();
           QString caseName= node.parentNode().attributes().namedItem("name").nodeValue();
           QString testName=node.attributes().namedItem("name").nodeValue();
           QString moduleName=node.parentNode().parentNode().attributes().namedItem("name").nodeValue();
           if( !moduleToCaseMap.contains(moduleName)){
              moduleToCaseMap.insert(moduleName,QStringList());

             }
           moduleToCaseMap.value(moduleName).append(caseName);

           if( !caseToTestMap.contains(caseName)){
               caseToTestMap.insert(caseName,QStringList());
           }
            caseToTestMap.value(caseName).append(testName);
          }
    }
}

void ParseResultWidget::showResult(QString xmlPath)
{
    QDomDocument doc;
    doc.setContent(new QFile(xmlPath));
    moduleToCaseMap.clear();
    caseToTestMap.clear();
    parseXml(doc.namedItem("Result"));
    updateTreeWidget();
}

ParseResultWidget::ParseResultWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParseResultWidget)
{
    ui->setupUi(this);
}

ParseResultWidget::~ParseResultWidget()
{
    delete ui;
}

void ParseResultWidget::updateTreeWidget()
{



//    QStringList moduleList=failedMap.uniqueKeys();
//    qDebug()<<moduleList;
//    for(int i=0;i<moduleList.size();i++){

//        QTreeWidgetItem*moduleItem=new QTreeWidgetItem(moduleList.at(i));
//        ui->result_tree_widget->addTopLevelItem(moduleItem);
//        QStringList caseList=failedMap.value(moduleList.at(i)).
//        foreach(QMultiMap map,failedMap.values(moduleList.at(i))){
//            moduleItem->addChild();
//        }
//    }
 //    for(int i=0;i<failedList.size();i++)
//    {
//        if( !moduleList.contains(failedList.at(i).value("module")) )
//            moduleList.append(failedList.at(i).value("module"));

//    }
//   // ui->result_tree_widget->

}
