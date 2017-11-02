#include "parseresultwidget.h"
#include "ui_parseresultwidget.h"
#include "QDomDocument"
#include "QFile"
#include "QDebug"

void ParseResultWidget::treeCheckedChange(QTreeWidgetItem*item,int)
{
    Qt::CheckState state=item->checkState(0);//必须要先取出来 因为后面改变子节点的时候会反过来修改父节点的状态,实时获取会导致只能勾选一个子节点

    if(state!= Qt::PartiallyChecked)
    {
        for(int i=0;i<item->childCount();i++)
        {
           item->child(i)->setCheckState(0,state);
        }
    }

   if( item->parent()!=NULL ) changeState(item->parent());
    // qDebug()<<"checked change";
}



void ParseResultWidget::parseXml(QDomNode node)  //递归解析xml文件,对我们感兴趣的节点进行处理
{
    parseNode(node);  //Test标签也可能有子节点,所以写在判断之外
    if(node.hasChildNodes()){

        QDomNodeList childNodes=node.childNodes();
        for(int i=0;i<childNodes.size();i++){
            parseXml(childNodes.at(i));
        }
    }else{ //对没有子节点的标签进行处理

    }
}

void ParseResultWidget::parseNode(QDomNode node)
{
    if(node.nodeName()=="Test"){
        if(node.attributes().namedItem("result").nodeValue()=="fail"){
           // qDebug()<<node.attributes().namedItem("name").nodeValue();
            QString caseName= node.parentNode().attributes().namedItem("name").nodeValue();
            QString testName=node.attributes().namedItem("name").nodeValue();
            QString moduleName=node.parentNode().parentNode().attributes().namedItem("name").nodeValue();
            if( !moduleToCaseMap.values(moduleName).contains(caseName))
                    moduleToCaseMap.insert(moduleName,caseName);
            if( !caseToTestMap.values(caseName).contains(testName))
                    caseToTestMap.insert(caseName,testName);
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
    show();
}

void ParseResultWidget::changeState(QTreeWidgetItem*item)
{
    Qt::CheckState preItemState=item->child(0)->checkState(0);
    Qt::CheckState newState;
    for(int j=0;j<item->childCount();j++)
    {
       if(item->child(j)->checkState(0) == preItemState)
           {
            if(j == item->childCount()-1 )
                newState=preItemState;
             else
                continue;
            }
        else{
            newState = Qt::PartiallyChecked;
            break;
         }

    }
    item->setCheckState(0,newState);
   //每次改变只需要知会自己的父节点,因为父节点自己会触发信号执行这个函数,不需要层层往上
    // if(item->parent() !=NULL )changeState(item->parent());
}

ParseResultWidget::ParseResultWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParseResultWidget)
{
    ui->setupUi(this);
    connect(ui->result_tree_widget,SIGNAL(itemChanged(QTreeWidgetItem*,int))
            ,this,SLOT(treeCheckedChange(QTreeWidgetItem*,int)));

}

ParseResultWidget::~ParseResultWidget()
{
    delete ui;
}

void ParseResultWidget::updateTreeWidget()
{

   // qDebug()<<moduleToCaseMap;
    ui->result_tree_widget->clear();
    foreach(QString moduleName,moduleToCaseMap.uniqueKeys())
    {
        QTreeWidgetItem*moduleItem=new QTreeWidgetItem(QStringList()<<moduleName);
        ui->result_tree_widget->addTopLevelItem(moduleItem);
        moduleItem->setCheckState(0,Qt::Unchecked);

        foreach(QString caseName,moduleToCaseMap.values(moduleName))
        {
            QTreeWidgetItem* caseItem=new QTreeWidgetItem(QStringList()<<caseName);
            caseItem->setCheckState(0,Qt::Unchecked);
            moduleItem->addChild(caseItem);

            foreach (QString testName,caseToTestMap.values(caseName))
            {
                QTreeWidgetItem* testItem=new QTreeWidgetItem(QStringList()<<testName);
                testItem->setCheckState(0,Qt::Unchecked);
                caseItem->addChild(testItem);
            }

        }

    }
}
