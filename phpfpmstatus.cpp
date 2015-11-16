#include "phpfpmstatus.h"
#include <QtXml/QDomDocument>

phpFpmStatus::phpFpmStatus(QObject *parent)
    : QObject(parent)
    , m_restClient(new RestClient(this))
{
    connect(m_restClient,SIGNAL(receivedResponse(QByteArray,int)),
            this, SLOT(receivedResponse(QByteArray,int)));
}

void phpFpmStatus::refresh()
{    
    m_restClient->makeGetRequestXML(m_url);
}

void phpFpmStatus::receivedResponse(QByteArray data, int code)
{
    qDebug() << "status code: " << code;
    qDebug() << "data: " << data;
    parseData(QString(data));
}

void phpFpmStatus::parseData(const QString & data)
{
    QDomDocument doc;
    if (!doc.setContent(data)){
        qDebug() << "NOT VALID XML! str: " << data;
        return;
    }
    QDomElement docElement = doc.documentElement();

    QString rootTag = docElement.tagName();
    qDebug() << "root: " << rootTag;

    if (rootTag != "status"){
        qDebug() << "incorrect xml: " << rootTag;
        return;
    }

    m_poolStatus.clear();
    m_processes.clear();

    QDomNode pEntries = docElement.firstChild();
    while (!pEntries.isNull()){
        QDomElement peData = pEntries.toElement();
        QString tagName = peData.tagName();
        QString value = peData.text();

        if(tagName != "processes"){if(value != "")m_poolStatus.insert(tagName,value);}
        else{
            qWarning() << "processes needs to be read differently " << tagName;
            QDomNode procEntries = peData.firstChild();
            while (!procEntries.isNull()){
                QDomElement procData = procEntries.toElement();
                QString procTagName = procData.tagName();

                if (procTagName == "process"){
                    QDomNode procStatEntries = procData.firstChild();

                    QVariantMap * m = new QVariantMap();
                    while (!procStatEntries.isNull()){
                        QDomElement procStatData = procStatEntries.toElement();
                        QString statName = procStatData.tagName();
                        QString statVal = procStatData.text();
                        qDebug() << "name: " << statName;
                        m->insert(statName,statVal);

                        procStatEntries= procStatEntries.nextSibling();
                    }
                    m_processes.append(m);
                }
                procEntries = procEntries.nextSibling();
            }

        }
        //next element
        pEntries = pEntries.nextSibling();
    }
    emit gotStatusData();
}
