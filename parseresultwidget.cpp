#include "parseresultwidget.h"
#include "ui_parseresultwidget.h"
#include "QDomDocument"
#include "QFile"
#include "QDebug"
#include "plandialog.h"
#include "solutionwidget.h"
#include "QDir"
#include"QProcess"

void ParseResultWidget::treeCheckedChange(QTreeWidgetItem*item,int)
{
    Qt::CheckState state=item->checkState(COLUMN_INDEX_NAME);//必须要先取出来 因为后面改变子节点的时候会反过来修改父节点的状态,实时获取会导致只能勾选一个子节点

    if(state!= Qt::PartiallyChecked)
    {
        for(int i=0;i<item->childCount();i++)
        {
           item->child(i)->setCheckState(COLUMN_INDEX_NAME,state);
        }
    }

   if( item->parent()!=NULL ) changeState(item->parent());

   if(item->childCount() == 0)
   {
       if(state == Qt::Checked)
           checkedList.append(item->text(COLUMN_INDEX_NAME));
       else
           checkedList.removeAll(item->text(COLUMN_INDEX_NAME));

       ui->label_checked_count->setText(QString("已勾选:%1").arg(checkedList.size()));
       ui->btn_new_plan->setEnabled( !checkedList.isEmpty() );
   }

}

void ParseResultWidget::selectAll(int state)
{
    for(int i=0;i<ui->result_tree_widget->topLevelItemCount();i++)
    {
        ui->result_tree_widget->topLevelItem(i)->setCheckState(COLUMN_INDEX_NAME
                                                    ,(Qt::CheckState)state);
    }
}

void ParseResultWidget::newPlan()
{
    qDebug()<<checkedList;
    PlanDialog* w=new PlanDialog;
    w->setModal(true);

    QDir dir(mResultPath+"/../../../subplans");
    QString planDirPath=dir.absolutePath();
    w->setPlanInfo(checkedList,planDirPath);

    if(w->exec())
    {
       writePlanXml(w->getPlanName());
       execPlan(w->getPlanName());
    }
    }

void ParseResultWidget::enableSolutionBtn()
{
    ui->btn_solution->setDisabled(ui->result_tree_widget->selectedItems().isEmpty()
                                  || ui->result_tree_widget->selectedItems().at(0)->childCount() != 0 );
}

void ParseResultWidget::showSolution()
{
   QString testName=ui->result_tree_widget->currentItem()->text(COLUMN_INDEX_NAME);
   QString caseName=caseToTestMap.key(testName);
   QString moduleName=moduleToCaseMap.key(caseName);

   QMap<QString,QString> map;
   map.insert("module",moduleName);
   map.insert("case",caseName);
   map.insert("test",testName);

   if(solutionWidget == NULL)
            solutionWidget=new SolutionWidget;

   solutionWidget->showSolution(map);
   solutionWidget->activateWindow();


}

void ParseResultWidget::expandTree()
{
    static bool isExpand=false;

    if( !isExpand )
    {
        ui->result_tree_widget->expandAll();
        ui->btn_expand->setText(QString::fromUtf8("收起▲"));
    }else{
        ui->result_tree_widget->collapseAll();
        ui->btn_expand->setText(QString::fromUtf8("展开▼"));
    }
         isExpand = !isExpand;

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
    mResultPath=xmlPath;
    QDomDocument doc;
    doc.setContent(new QFile(xmlPath));
    moduleToCaseMap.clear();
    caseToTestMap.clear();
    checkedList.clear();
    parseXml(doc.namedItem("Result"));
    updateTreeWidget();
    ui->result_tree_widget->collapseAll();
    ui->btn_expand->setText(QString::fromUtf8("展开▼"));
    ui->result_tree_widget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    show();
}

void ParseResultWidget::changeState(QTreeWidgetItem*item)
{
    Qt::CheckState preItemState=item->child(0)->checkState(COLUMN_INDEX_NAME);
    Qt::CheckState newState;
    for(int j=0;j<item->childCount();j++)
    {
       if(item->child(j)->checkState(COLUMN_INDEX_NAME) == preItemState)
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
    item->setCheckState(COLUMN_INDEX_NAME,newState);
   //每次改变只需要知会自己的父节点,因为父节点自己会触发信号执行这个函数,不需要层层递归往上

}

void ParseResultWidget::writePlanXml(QString planName)
{
    QDir dir(mResultPath+"/../../../subplans"); //有.的路径是相对路径,可以转化为绝对路径,这种方式更方便不用自己组装字符串
    if( !dir.exists())
    {
        dir.mkdir(dir.absolutePath());
     //   qDebug()<<"mkdir "<<dir.absolutePath();
    }
    QFile file(dir.absolutePath()+"/"+planName+".xml");
    QDomDocument doc;
    QDomElement rootNode = doc.createElement("SubPlan");
    rootNode.setAttribute("version","2.0");

    for(QString test:checkedList)
    {
      QString caseName=caseToTestMap.key(test);
      QString moduleName=moduleToCaseMap.key(caseName);
      QDomElement entryNode=doc.createElement("Entry");
      entryNode.setAttribute("include",moduleName+" "+caseName+"#"+test);
      rootNode.appendChild(entryNode);
    }
     doc.appendChild(rootNode);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out<<"<?xml version='1.0' encoding='UTF-8' standalone='no' ?>\n";
    doc.save(out,4);
    file.close();
    //qDebug()<<dir.absolutePath();
}

void ParseResultWidget::execPlan(QString planName)
{
    QProcess *p=new QProcess(this);
    QStringList arg;
    arg<<"-x"<<"bash"<<"-c"<<"-v"<<QString("/home/liaowenxing/plan.exp run cts --plan %1").arg(planName);
    p->execute(QString("gnome-terminal"),arg);
   // qDebug()<<planPath;
}


ParseResultWidget::ParseResultWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParseResultWidget)
{
    ui->setupUi(this);
    connect(ui->result_tree_widget,SIGNAL(itemChanged(QTreeWidgetItem*,int))
            ,this,SLOT(treeCheckedChange(QTreeWidgetItem*,int)));
    connect(ui->cbox_select_all,SIGNAL(stateChanged(int)),this,SLOT(selectAll(int)));

    connect(ui->btn_new_plan,SIGNAL(clicked(bool)),this,SLOT(newPlan()));

    connect(ui->btn_solution,SIGNAL(clicked(bool)),this,SLOT(showSolution()));

    connect(ui->result_tree_widget,SIGNAL(itemSelectionChanged()),this,SLOT(enableSolutionBtn()));

    connect(ui->btn_expand,SIGNAL(clicked(bool)),this,SLOT(expandTree()));

    ui->result_tree_widget->setColumnCount(2);
    ui->result_tree_widget->setHeaderLabels(QStringList()<<QString::fromUtf8("名称")
                                            <<QString::fromUtf8("数量"));
    ui->btn_solution->setDisabled(true);
   // ui->result_tree_widget->collapseAll();
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
        int testCountInModule=0;
        QTreeWidgetItem*moduleItem=new QTreeWidgetItem(QStringList()<<moduleName);
        ui->result_tree_widget->addTopLevelItem(moduleItem);
        moduleItem->setCheckState(COLUMN_INDEX_NAME,Qt::Unchecked);

        foreach(QString caseName,moduleToCaseMap.values(moduleName))
        {
            QTreeWidgetItem* caseItem=new QTreeWidgetItem(QStringList()<<caseName);
            caseItem->setCheckState(COLUMN_INDEX_NAME,Qt::Unchecked);
            moduleItem->addChild(caseItem);

            foreach (QString testName,caseToTestMap.values(caseName))
            {
                QTreeWidgetItem* testItem=new QTreeWidgetItem(QStringList()<<testName);
                testItem->setCheckState(COLUMN_INDEX_NAME,Qt::Unchecked);
                caseItem->addChild(testItem);
            }

            int testCountInCase=caseItem->childCount();
            testCountInModule += testCountInCase;
            caseItem->setText(COLUMN_INDEX_COUNT,QString("%1").arg(testCountInCase));
        }
        moduleItem->setText(COLUMN_INDEX_COUNT,QString("%1").arg(testCountInModule));
    }

}
