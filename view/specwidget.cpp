#include "specwidget.h"
#include "ui_specwidget.h"
#include<entity/spectest.h>
#include<entity/proptest.h>
#include<entity/packagetest.h>
#include<util/specutil.h>
#include<QDebug>
#include<QFile>
#include<QFileDialog>
#include<QMessageBox>
#include<QDomDocument>

SpecWidget::SpecWidget(QWidget *parent) :
    PopWidget(parent),
    ui(new Ui::SpecWidget)
{
    ui->setupUi(this);
    ui->label->setText(QString::fromUtf8("<font color='green'>绿色PASS</font>，<font color='red'>红色FAIL</font>，其他视项目而定，需要自己check"));
    setWindowTitle(QString::fromUtf8("规范测试结果"));
    ui->tableWidget->setColumnCount(3);
    QStringList header = QStringList()<<QString::fromUtf8("name")
                                     <<QString::fromUtf8("期望结果")<<QString::fromUtf8("实际结果");
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    moveToCenter();
    connect(ui->btn_export,&QPushButton::clicked,this,&SpecWidget::saveReport);
}

SpecWidget::~SpecWidget()
{
    delete ui;
}

QString SpecWidget::getStatusText(SpecTest::status status)
{
    if(status == SpecTest::PASS){
        return "PASS";
    }else if(status == SpecTest::FAIL){
        return "FAIL";
    }else{
        return "UNKNOWN";
    }
}

void SpecWidget::showResult(QList<SpecTest *> list)
{
    this->list = list;
    qDebug()<<"SpecWidget::showResult:"<<list.size();
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(list.size());
    for(int i = 0;i < list.size() ; i++){
        SpecTest* test = list.at(i);
        QTableWidgetItem*name = new QTableWidgetItem;
        name->setText(test->getName());
        QTableWidgetItem*expect = new QTableWidgetItem;
        expect->setText(test->getExpect());
        QTableWidgetItem*result = new QTableWidgetItem;
        result->setText(test->getResult());
        if(test->getStatus() == SpecTest::PASS){
            result->setBackgroundColor(Qt::green);
        }else if(test->getStatus() == SpecTest::FAIL){
            result->setBackgroundColor(Qt::red);
        }
        ui->tableWidget->setItem(i,0,name);
        ui->tableWidget->setItem(i,1,expect);
        ui->tableWidget->setItem(i,2,result);
    }
    showMaximized();
}

void SpecWidget::saveReport()
{
    qDebug()<<"[SpecWidget::saveReport]";
    QString filename = QFileDialog::getExistingDirectory(this,QString::fromUtf8(""),QDir::homePath())
            .append("/SpecReport.xml");
    QFile file(filename);
    if(file.open(QIODevice::WriteOnly|QIODevice::Truncate)){
        QTextStream out(&file);
        QDomDocument doc;
        QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
        doc.appendChild(instruction);
        QDomElement tests = doc.createElement("Tests");
        foreach(SpecTest* test,list){
            QDomElement testElement = doc.createElement("Test");
            QDomElement nameElement = doc.createElement("Name");
            QDomElement expectElement = doc.createElement("Expect");
            QDomElement resultElement = doc.createElement("Result");
            QDomElement statusElement = doc.createElement("Status");
            nameElement.appendChild(doc.createTextNode(test->getName()));
            expectElement.appendChild(doc.createTextNode(test->getExpect()));
            resultElement.appendChild(doc.createTextNode(test->getResult()));
            statusElement.appendChild(doc.createTextNode(getStatusText(test->getStatus())));
            testElement.appendChild(nameElement);
            testElement.appendChild(expectElement);
            testElement.appendChild(resultElement);
            testElement.appendChild(statusElement);
            tests.appendChild(testElement);
        }
        doc.appendChild(tests);
        doc.save(out,2);
        file.close();
        QMessageBox::information(this,QString::fromUtf8("文件保存至"),QString::fromUtf8("报告已保存至%1").arg(filename));
    }
}

