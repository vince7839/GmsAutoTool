#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "toolwidget.h"
#include "taskwidget.h"
#include "resultwidget.h"
#include <QTime>
#include <view/tabstyle.h>
#include <QMenuBar>
#include <util/config.h>
#include <QSettings>
#include <QProcess>
#include <QDebug>
#include <QMessageBox>
#include<QDesktopWidget>
#include<util/networkutil.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(QString::fromUtf8("GmsAutoTool"));
    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(updateTabContent(int)));

    mLinkLabel = new QLabel;
    mLinkLabel->setOpenExternalLinks(true);
    ui->statusbar->addPermanentWidget(mLinkLabel);
    ui->statusbar->setStyleSheet("QStatusBar::item{border: 0px}");

    toolWidget = new ToolWidget;
    testWidget = new TaskWidget;
    resultWidget = new ResultWidget;
    extendWidget = new ExtendWidget;
    onlineWidget = new OnlineWidget;
    settingsWidget = new SettingsWidget;

    ui->tabWidget->addTab(toolWidget,QString::fromUtf8("工具管理"));
   // ui->tabWidget->addTab(testWidget,QString::fromUtf8("开始测试"));
    ui->tabWidget->addTab(resultWidget,QString::fromUtf8("测试报告"));
    ui->tabWidget->addTab(extendWidget,QString::fromUtf8("扩展工具"));
    ui->tabWidget->addTab(onlineWidget,QString::fromUtf8("内网工具"));
    ui->tabWidget->addTab(settingsWidget,QString::fromUtf8("设置选项"));
    ui->tabWidget->tabBar()->setStyle(new TabStyle);
    int x = (QApplication::desktop()->width() - width()) /2;
    int y = (QApplication::desktop()->height() - height()) /2;
    move(x,y);
    initMenu();
    initLinks();
    startTimer(5000);

    checkProcess = new QProcess;
    QStringList arg = QStringList()<<"-v";
    connect(checkProcess,&QProcess::readyRead,this,&MainWindow::onProcessOutput);
    checkProcess->start("expect",arg);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initMenu()
{
    QMenu* optionsMenu = menuBar()->addMenu(QString::fromUtf8("关于"));
    menuBar()->show();
    QAction* aboutQt = optionsMenu->addAction(QString::fromUtf8("关于Qt"));
    connect(aboutQt,SIGNAL(triggered()),this,SLOT(aboutQt()));
    QAction* aboutMine = optionsMenu->addAction(QString::fromUtf8("关于本软件"));
    connect(aboutMine,&QAction::triggered,this,&MainWindow::aboutMine);
}

void MainWindow::initLinks()
{
    mLinkList.clear();
  /*  QDomDocument doc;
    doc.setContent(new QFile("config/Config.xml"));
   QDomNode linkNode =  doc.namedItem("Config").namedItem("Link");
   qDebug()<<"node size:"<<linkNode.childNodes().size();
   for(int i = 0;i<linkNode.childNodes().size();i++)
   {
        QDomNode child = linkNode.childNodes().at(i);
        QString title = child.attributes().namedItem("title").nodeValue();
        QString url = child.toElement().text();
        QMap<QString,QString> map;
        map.insert("title",title);
        map.insert("url",url);
        mLinkList.append(map);
   }*/
    NetworkUtil* netUtil = new NetworkUtil;
    connect(netUtil,&NetworkUtil::finished,this,&MainWindow::onRequestFinished);
    netUtil->get("http://192.168.3.77:8989/wcp/webtype/view402880cd5b65b8bb015bec3decbc0058/Pub1.html");
}

void MainWindow::aboutQt()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::aboutMine()
{
    QMessageBox::information(this,QString::fromUtf8("关于本软件")
    ,QString::fromUtf8("此软件用于CTS，GTS，VTS等工具的辅助测试，帮助测试者自动输入命令和建立Plan等操作，如果有建议，请向我反馈！liaowenxing@sagereal.com或者走过来找我。")
                             ,QMessageBox::Yes);
}

void MainWindow::updateLinkLabel()
{
    if(mLinkList.isEmpty()) return;
    QString prefix("http://192.168.3.77:8989/wcp/");
    static int index = 0;
    index = ++index%(mLinkList.size());
    QMap<QString,QString> map = mLinkList.at(index);
    QString url = prefix.append(map.value("url"));
    QString title = map.value("title");
    mLinkLabel->setToolTip(title);
    if(title.size() > 20)
    {        
        title = title.left(20).append("...");
    }
    //qDebug()<<"[MainWindow]real url:"<<url;
    QString link = QString::fromUtf8("<a href='%1'>WCP最新知识：%2</a>").arg(url).arg(title);
    mLinkLabel->setText(link);
}

void MainWindow::onRequestFinished(QNetworkReply *reply)
{
       QString src = QString::fromUtf8(reply->readAll());
       src = src.simplified();
       QRegExp rx("class=\"doc_node_title_min\".*href=\"(.+)\">(.+)<span");
       rx.setMinimal(true);
       int pos = 0;
       while ((pos = rx.indexIn(src, pos)) != -1)
       {
           QString url = rx.cap(1);
           QString title = rx.cap(2).simplified();
           QMap<QString,QString> map;
           map.insert("url",url);
           map.insert("title",title);
           mLinkList.append(map);
         //  qDebug()<<QString("[MainWindow]get wcp:%1:%2").arg(title).arg(url);
           pos += rx.matchedLength();
       }
       updateLinkLabel();
       qDebug()<<"[MainWindow]links size:"<<mLinkList.size();
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    updateLinkLabel();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    SocketUtil::getInstance()->sendMeOffline();
}

void MainWindow::updateTabContent(int index)
{
    qDebug()<<"[MainWindow] tab changed:"<<index;
    BaseWidget*tab = static_cast<BaseWidget*>(ui->tabWidget->currentWidget());
    tab->updateContent();
}

void MainWindow::onProcessOutput()
{
    qDebug()<<"onProcessOutput";
    QString output = checkProcess->readAll();
    if(output.contains("version")){
        qDebug()<<"[MainWindow] expect ok";

    }else{
        qDebug()<<"[MainWindow] expect not ok";
        QMessageBox::warning(this,QString::fromUtf8("提示")
                             ,QString::fromUtf8("即将安装expect，它是与命令行交互的必要组件，请在弹出的控制台中输入您的sudo密码"));
        QProcess* p = new QProcess;
        p->start("gnome-terminal",QStringList()<<"-x"<<"bash"<<"-c"
                 <<"cd plugin/tcl8.4.20/unix/;./configure;sudo make && sudo make install;cd ../../expect5.45.3/;./configure;sudo make && sudo make install;");
    }
}

