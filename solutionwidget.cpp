#include "solutionwidget.h"
#include "ui_solutionwidget.h"
#include<QLabel>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QPushButton>
#include<QGroupBox>
#include<QFile>
#include<QDebug>
#include<QClipboard>
#include<addsolutionwidget.h>
#include<QSpacerItem>

void SolutionWidget::copyText()
{
    QClipboard*board=QApplication::clipboard();
    board->setText(copyMap.value((QPushButton*)QObject::sender()));
}

void SolutionWidget::showSolution(QMap<QString, QString> map)
{
    scrollWidget=new QWidget;
    vLayoutScrool=new QVBoxLayout;
    testInfo=map;
    copyMap.insert(ui->btn_copy_module,map.value("module"));
    copyMap.insert(ui->btn_copy_case,map.value("case"));
    copyMap.insert(ui->btn_copy_test,map.value("test"));

    ui->label_module->setText(QString::fromUtf8("Module：").append(map.value("module")));
    ui->label_case->setText(QString::fromUtf8(  "Case  ：").append(map.value("case")));
    ui->label_test->setText(QString::fromUtf8(  "Test  ：").append(map.value("test")));

    QDomDocument doc;

    doc.setContent(new QFile("/home/liaowenxing/solution.xml"));
    parseXml(doc.namedItem("Solution"));

    scrollWidget->setLayout(vLayoutScrool);

    ui->scrollArea->setWidget(scrollWidget);
    show();
}

void SolutionWidget::addAnswer(QMap<QString, QString> map)
{
    QLabel *label_project=new QLabel(QString::fromUtf8("项目：").append(map.value("project")));
    QLabel *label_tool=new QLabel(QString::fromUtf8("工具：").append(map.value("tool")));
    QLabel *label_time=new QLabel(QString::fromUtf8("时间：").append(map.value("time")));
    QLabel *label_editor=new QLabel(QString::fromUtf8("编辑者：").append(map.value("editor")));

    QLabel *label_analy=new QLabel(map.value("analysis"));
    label_analy->setWordWrap(true);

    QLabel *label_solution=new QLabel(map.value("solution"));
    label_solution->setWordWrap(true);
    QPalette p;
    p.setColor(QPalette::WindowText,Qt::red);
    label_solution->setPalette(p);

    QLabel *label_title_analy=new QLabel(QString::fromUtf8("分析："));
    QLabel *label_title_solution=new QLabel(QString::fromUtf8("解决方案："));
    QPushButton*btn_copy_analy=new QPushButton(QString::fromUtf8("复制"));
    QPushButton*btn_copy_solution=new QPushButton(QString::fromUtf8("复制"));

    QGroupBox *groupBox=new QGroupBox;

    QHBoxLayout *hLayout_1=new QHBoxLayout;
    QHBoxLayout *hLayout_2=new QHBoxLayout;

    QVBoxLayout *vLayout=new QVBoxLayout;

    hLayout_1->addWidget(label_title_analy);
    hLayout_1->addStretch();
    hLayout_1->addWidget(btn_copy_analy);

    hLayout_2->addWidget(label_title_solution);
    hLayout_2->addStretch();
    hLayout_2->addWidget(btn_copy_solution);

    vLayout->addWidget(label_project);
    vLayout->addWidget(label_tool);
    vLayout->addWidget(label_time);
    vLayout->addWidget(label_editor);
    vLayout->addItem(hLayout_1);
    vLayout->addWidget(label_analy);
    vLayout->addItem(hLayout_2);
    vLayout->addWidget(label_solution);

    groupBox->setLayout(vLayout);

    vLayoutScrool->addWidget(groupBox);

    copyMap.insert(btn_copy_analy,map.value("analysis"));
    copyMap.insert(btn_copy_solution,map.value("solution"));
    connect(btn_copy_analy,SIGNAL(clicked(bool)),this,SLOT(copyText()));
    connect(btn_copy_solution,SIGNAL(clicked(bool)),this,SLOT(copyText()));

}

void SolutionWidget::parseNode(QDomNode node)
{
    if(node.nodeName()=="Test"
            && node.attributes().namedItem("name").nodeValue()==testInfo.value("test"))
    {
       for(int j=0;j<node.childNodes().size();j++)
       {
           QDomNode child=node.childNodes().at(j);
           QMap<QString,QString> map;
            for(int i=0;i<child.attributes().size();i++)
            {
                map.insert(child.attributes().item(i).nodeName()
                      ,child.attributes().item(i).nodeValue());
            }
           addAnswer(map);
        }
    }

}

void SolutionWidget::addSolution()
{
    AddSolutionWidget*w=new AddSolutionWidget;
    w->showDetail(testInfo);
}

void SolutionWidget::parseXml(QDomNode node)
{
    parseNode(node);
    if(node.hasChildNodes()){

        QDomNodeList childNodes=node.childNodes();
        for(int i=0;i<childNodes.size();i++){
            parseXml(childNodes.at(i));
        }
    }else{

    }
}

SolutionWidget::SolutionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SolutionWidget)
{
    ui->setupUi(this);
    setWindowTitle(QString::fromUtf8("解决方案"));
    connect(ui->btn_copy_module,SIGNAL(clicked(bool)),this,SLOT(copyText()));
    connect(ui->btn_copy_case,SIGNAL(clicked(bool)),this,SLOT(copyText()));
    connect(ui->btn_copy_test,SIGNAL(clicked(bool)),this,SLOT(copyText()));
    connect(ui->btn_add_solution,SIGNAL(clicked(bool)),this,SLOT(addSolution()));

}

SolutionWidget::~SolutionWidget()
{
    delete ui;
}
