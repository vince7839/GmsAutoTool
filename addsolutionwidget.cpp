#include "addsolutionwidget.h"
#include "ui_addsolutionwidget.h"
#include<QDomDocument>
#include<QDomNode>
#include<QDebug>

AddSolutionWidget::AddSolutionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddSolutionWidget)
{
    ui->setupUi(this);
    setWindowModality(Qt::ApplicationModal);
    connect(ui->btn_ok,SIGNAL(clicked(bool)),this,SLOT(writeXml()));
    connect(ui->btn_cancel,SIGNAL(clicked(bool)),this,SLOT(close()));
}

AddSolutionWidget::~AddSolutionWidget()
{
    delete ui;
}

void AddSolutionWidget::showDetail(QMap<QString, QString> map)
{
    testInfo=map;
    show();
}

QDomNode AddSolutionWidget::getNodeByAttr(QDomNode node,QString attrValue)
{
    QDomNode childNode;
    for(int i=0;i<node.childNodes().size();i++)
    {
        if(node.attributes().namedItem("name").nodeValue() == attrValue)
            childNode=node.childNodes().at(i);
    }

    return childNode;
}

void AddSolutionWidget::writeXml()
{


    QDomDocument doc;
    QFile file("/home/liaowenxing/solution.xml");
    if( file.open(QIODevice::ReadWrite) )
        doc.setContent(&file);
    else
        qDebug()<<"file open fail";
    QDomNode rootNode=doc.namedItem("Solution");
    QDomElement moduleNode=getNodeByAttr(rootNode,testInfo.value("module")).toElement();

    if( moduleNode.isNull() )
    {
         moduleNode=doc.createElement("Module");
         moduleNode.setAttribute("name",testInfo.value("module"));

         rootNode.appendChild(moduleNode);
    }

    QDomElement caseNode=getNodeByAttr(moduleNode,testInfo.value("case")).toElement();

    if( caseNode.isNull() )
    {
        caseNode=doc.createElement("TestCase");
        caseNode.setAttribute("name",testInfo.value("case"));
        moduleNode.appendChild(caseNode);
    }

    QDomElement testNode=getNodeByAttr(caseNode,testInfo.value("test")).toElement();

    if(testNode.isNull())
    {
        testNode=doc.createElement("Test");
        testNode.setAttribute("name",testInfo.value("test"));
        caseNode.appendChild(testNode);
    }

    testNode.setAttribute("project",ui->lineEdit_project->text());
    testNode.setAttribute("tool",ui->lineEdit_tool->text());
    testNode.setAttribute("time",ui->dateTimeEdit->text());
    testNode.setAttribute("editor",ui->lineEdit_editor->text());
    testNode.setAttribute("analysis",ui->plainTextEdit_analysis->document()->toPlainText());
    testNode.setAttribute("solution",ui->plainTextEdit_solution->document()->toPlainText());

    QTextStream out(&file);

    doc.save(out,4);
    file.close();

}

void AddSolutionWidget::parseXml(QDomNode node)
{

//    parseNode(node);
//    if(node.hasChildNodes()){

//        QDomNodeList childNodes=node.childNodes();
//        for(int i=0;i<childNodes.size();i++){
//            parseXml(childNodes.at(i));
//        }
//    }else{

//    }
}

void AddSolutionWidget::parseNode(QDomNode node)
{

}
