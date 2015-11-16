#ifndef PHPFPMSTATUS_H
#define PHPFPMSTATUS_H

#include <QObject>
#include <QUrl>
#include <QVariantMap>
#include <QList>
#include "restclient.h"

class phpFpmStatus : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl statusUrl READ statusUrl WRITE setStatusUrl NOTIFY statusUrlChanged)
public:
    explicit phpFpmStatus(QObject *parent = 0);
    QUrl statusUrl() const {return m_url;}
signals:
    void statusUrlChanged();
    void gotStatusData();

public slots:
    void refresh();
    void setStatusUrl(QUrl val){m_url=val;emit statusUrlChanged();}
    QList<QVariantMap *> processes(){return m_processes;}
    QVariantMap poolStatus(){return m_poolStatus;}

private slots:
    void receivedResponse(QByteArray, int);
    void parseData(const QString &data);

private:
    RestClient *m_restClient;
    QUrl m_url;

    QVariantMap m_poolStatus;
    QList<QVariantMap *> m_processes;
};

#endif // PHPFPMSTATUS_H
