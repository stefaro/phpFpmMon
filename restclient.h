#ifndef RESTCLIENT_H
#define RESTCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QAuthenticator>
#include <QList>

/*ACCEPT*/
static const QString ACCEPT_HEADER_XML  = "application/*+xml";
/*CONTENT TYPE*/
static const QString CONTENT_TYPE_XML  = "application/*+xml";

/*METHODS*/
enum HTTP_METHOD{
        HTTP_GET = 0,
        HTTP_POST,
        HTTP_PUT,
        HTTP_DELETE
};

enum HTTP_STATUS_CODES{
    HTTP_STATUS_NOT_FOUND = 404,
    HTTP_STATUS_FORBIDDEN = 413,
    HTTP_STATUS_OK = 200,
    HTTP_STATUS_INTERNAL_ERROR = 500
};

class RestClient : public QObject
{
    Q_OBJECT

public:
    explicit RestClient(QObject *parent = 0);
    void makeAsynchronousRequest(QUrl &, int , const QString &contentType);
    void setCredentials(const QString &,const QString &);
    void storeData(bool value){m_storeData=value;}
    QByteArray replyData(){return m_replyData;}
    int statusCode(){return m_statusCode;}

    void makeGetRequest(QUrl &url){makeAsynchronousRequest(url,HTTP_GET,NULL);}
    void makeGetRequestXML(QUrl &url){setAcceptHeader(ACCEPT_HEADER_XML);makeAsynchronousRequest(url,HTTP_GET,CONTENT_TYPE_XML);}   

    void setAcceptHeader(const QString &header){acceptHeader=new QString();acceptHeader->append(header);}

private:
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QString *username;
    QString *password;
    QString *acceptHeader;
    QByteArray m_replyData; // data only if storeData is set to true...
    bool m_storeData;
    bool authFailed;
    int m_statusCode;

    void init();
    QString generateUserAgent();
    QByteArray generateAuthHeader();

signals:
    void sentRequest();
    void requestFinished();
    void receivedResponse(QByteArray,int);
    void invalidCredentials();

public slots:
    void replyFinished(QNetworkReply *);
    void authenticationRequested(QNetworkReply *, QAuthenticator *);
};

#endif // RESTCLIENT_H
