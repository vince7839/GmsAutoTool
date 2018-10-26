#include "addsolutionwidget.h"
#include "ui_addsolutionwidget.h"
#include<QDomDocument>
#include<QDomNode>
#include<QDebug>

AddSolutionWidget::AddSolutionWidget(QWidget *parent) :
    PopWidget(parent),
    ui(new Ui::AddSolutionWidget)
{
    ui->setupUi(this);
    setWindowTitle(QString::fromUtf8("提交解决方案"));
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
        if(node.childNodes().at(i).attributes().namedItem("name").nodeValue() == attrValue)
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
    qDebug()<<"new module";
    }

    QDomElement caseNode=getNodeByAttr(moduleNode,testInfo.value("case")).toElement();

    if( caseNode.isNull() )
    {
        caseNode=doc.createElement("TestCase");
        caseNode.setAttribute("name",testInfo.value("case"));
        moduleNode.appendChild(caseNode);
    qDebug()<<"new case";
    }

    QDomElement testNode=getNodeByAttr(caseNode,testInfo.value("test")).toElement();

    if(testNode.isNull())
    {
        testNode=doc.createElement("Test");
        testNode.setAttribute("name",testInfo.value("test"));
        caseNode.appendChild(testNode);
    qDebug()<<"new test";
    }

    QDomElement answerNode=doc.createElement("Answer");

    answerNode.setAttribute("project",ui->lineEdit_project->text());
    answerNode.setAttribute("tool",ui->lineEdit_tool->text());
    answerNode.setAttribute("time",ui->dateTimeEdit->text());
    answerNode.setAttribute("editor",ui->lineEdit_editor->text());
    answerNode.setAttribute("analysis",ui->plainTextEdit_analysis->document()->toPlainText());
    answerNode.setAttribute("solution",ui->plainTextEdit_solution->document()->toPlainText());

    testNode.appendChild(answerNode);
    file.resize(0);
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
