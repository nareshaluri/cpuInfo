#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <qdebug.h>

namespace  {
    const char* ITEM_ROOT_WINDOW        = "rootWindow";
    const char* ITEM_LIST_VIEW          = "cpuInfoList";
    const char* PROPERTY_LIST_MODEL     = "model";

    const QString CPU_INFO_FILE_PATH    = "/proc/cpuinfo";
    const QString QML_TO_LOAD           = "qrc:///main.qml";
}


/**
 * @brief getCPUInfo
 * This function gets the cpuinfo located in /proc/cpuInfo file and format the each entry found in the file
 * @return
 * Returns list of QStrings. Each item in the list represent one entry from cpuInfo file
 */
QStringList getCPUInfo()
{
    qDebug()<<"getCPUInfo()";

    QStringList cpuEntriesList;

    QFile cpuInfoFile(CPU_INFO_FILE_PATH);

    bool canReadFile = cpuInfoFile.open(QIODevice::ReadOnly);
    if(canReadFile)
    {
        QTextStream fileStream(&cpuInfoFile);

        QString lineData;
        //Read file content and format it
        do
        {
            lineData = fileStream.readLine();
            //Each entry in cpuInfo file is ":" separated <key,Val> pair
            QStringList lineSplitData = lineData.split(":");
            QString keyPart;
            QString valuePart;
            //Some entries do not have value part for example "power management"
            //Some are empry entries
            //Filter out those entries
            if(lineSplitData.length() == 2)
            {
                keyPart = lineSplitData.at(0).trimmed();//Trim extra spaces
                valuePart = lineSplitData.at(1).trimmed();//Trim extra spaces
            }
            else if(lineSplitData.length() == 1)
            {
                keyPart = lineSplitData.at(0).trimmed();//Trim extra spaces
                valuePart = "";
            }
            if(!keyPart.isEmpty())
            {
                if(!valuePart.isEmpty())
                {
                    QString entry = keyPart+"\t"+":"+"\t"+valuePart;//Format the display string
                    cpuEntriesList << entry;
                }
                else
                {
                    QString entry = keyPart+"\t"+":"+"\t";
                    cpuEntriesList << entry;
                }
            }
        }while(!lineData.isNull());

        cpuInfoFile.close();
    }
    else
    {
        qDebug()<<"ERROR IN OPENING FILE" << cpuInfoFile.errorString();
    }
    return cpuEntriesList;
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QML_TO_LOAD));

    QList<QObject*> rootItems = engine.rootObjects();

    for(int i=0;i<rootItems.length();i++)
    {
        //Find root winddow
        if(QString::compare(rootItems.at(i)->objectName(),ITEM_ROOT_WINDOW) == 0)
        {
            QObject *rootWindow = rootItems.at(i);//Root window reference
            QStringList cpuEntriesList = getCPUInfo();
            QQuickItem *listView = rootWindow->findChild<QQuickItem*>(ITEM_LIST_VIEW);//Find ListView
            if(listView)
            {
                qDebug()<<"FOUND LIST VIEW";
                listView->setProperty(PROPERTY_LIST_MODEL,QVariant::fromValue(cpuEntriesList));
            }
            break;
        }
        else
        {
            qDebug()<<"No item found in qml with name 'rootWindow' ";
        }
    }
    return app.exec();
}
