#include "tasklistener.h"
#include<QDebug>
#include<QFile>

TaskListener::TaskListener(QObject *parent) : QObject(parent)
{

}

TaskListener::TaskListener(TaskParam *taskParam)
{
    mFileWatcher = new QFileSystemWatcher;
    connect(mFileWatcher,&QFileSystemWatcher::fileChanged,this,&TaskListener::onFileChanged);
}

void TaskListener::parseOutput(QString output)
{
    //  qDebug()<<"output:"<<output;
      QRegExp expectTimeReg(".*running ([0-9]+) .*modules, expected to complete in ([0-9]+)h ([0-9]+)m ([0-9]+)s.*");
      QRegExp moduleStartReg(".*Starting .* (.*) with ([0-9]+) test.*");   //more than 1 test will show tests
      QRegExp testFinishReg(".*([0-9]+:[0-9]+:[0-9]+).*([0-9]+)/([0-9]+) .* (.*) .* .*#(.*) (pass|fail).*");
      QRegExp nameReg("\\[GmsAutoTool\\]test name:(.*)");

      if(expectTimeReg.exactMatch(output))
      {
          qDebug()<<"[TaskWidget]output matches 'expect time'";
          QString moduleNum = expectTimeReg.capturedTexts().at(1);
          int num = moduleNum.toInt();

          qDebug()<<"[TaskWidget]output parse module num "<<moduleNum;
          QString hour = expectTimeReg.capturedTexts().at(2);
          QString minute = expectTimeReg.capturedTexts().at(3);
          QString second = expectTimeReg.capturedTexts().at(4);
          qDebug()<<hour<<minute<<second;
          emit timeExpected();
      }else if(moduleStartReg.exactMatch(output)){
          qDebug()<<"[TaskWidget]start a new module";
          QString moduleName = moduleStartReg.cap(1);
            emit moduleStart();
      }else if(testFinishReg.exactMatch(output)){
         // qDebug()<<"test finish:"<<output;
          QString finishTime = testFinishReg.cap(1);
          QString testIndex = testFinishReg.cap(2);
          QString testCount = testFinishReg.cap(3);
          QString moduleName = testFinishReg.cap(4);
          QString testName = testFinishReg.cap(5);
          QString testResult = testFinishReg.cap(6);
          emit moduleFinished();
      }else if(nameReg.exactMatch(output)){
          qDebug()<<"find test name:"<<nameReg.cap(1);
      }
}

void TaskListener::onFileChanged(QString filePath)
{
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly))
    {
        QStringList list = QString(file.readAll()).split("\n");
        list.removeLast();
        if(!list.isEmpty())
        {
            /*for(int i = view->rowIndex;i < list.size();i++)
            {
                QString output = list.at(i);
                if(!output.isEmpty()){
                    parseOutput(output);
                }
            }*/
        }
        file.close();
    }
}
