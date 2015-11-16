#include "restclient.h"
#include <qdebug.h>
#include <QHttpMultiPart>
#include <QFile>
#include <QFileInfo>

void RestClient::init()
{
    /*CONNECT ALL NECESSARY SIGNALS*/
    connect(manager,SIGNAL(finished(QNetworkReply *)),this,SLOT(replyFinished(QNetworkReply *)) );
    connect(manager,
            SIGNAL(authenticationRequired(QNetworkReply *, QAuthenticator *)),
            this,
            SLOT(authenticationRequested(QNetworkReply *, QAuthenticator *))
            );
}

RestClient::RestClient(QObject *parent)
    : QObject(parent)
    , manager(new QNetworkAccessManager(this))
    , reply(0)
    , username(0)
    , password(0)
    , acceptHeader(0)    
    , m_storeData(false)
    , authFailed(false)
{
    //manager = new QNetworkAccessManager(this);
    init();

    qDebug() << "SIGNAL connected";
}

QString RestClient::generateUserAgent()
{
    return QString("phpFpmMon/0.1");
}

QByteArray RestClient::generateAuthHeader()
{
    QString auth = QString("%1:%2").arg(*username).arg(*password);
    QByteArray data = "Basic " +  auth.toLocal8Bit().toBase64();
    return data;
}


void RestClient::makeAsynchronousRequest(QUrl &requestUrl, int requestMethod, const QString &contentType)
{
    qDebug() << "URL FOR RESTCLIENT " << requestUrl;
    QNetworkRequest request;
    request.setUrl(requestUrl);

    /*USER-AGENT*/
    request.setHeader(QNetworkRequest::UserAgentHeader,generateUserAgent());

    /*ACCEPT*/
    if (acceptHeader){
        request.setRawHeader("Accept",acceptHeader->toLatin1());
    }

    /*AUTH*/
    if (username && password){
        request.setRawHeader("Authorization",generateAuthHeader());
    }

    /*CONTENT TYPE*/
    if (contentType.compare(CONTENT_TYPE_XML,Qt::CaseInsensitive) == 0) {
        qDebug() << "Content-Type: XML";
        request.setHeader(QNetworkRequest::ContentTypeHeader,CONTENT_TYPE_XML);
    }
    else{
        qDebug() << "No content-type set" << contentType;
    }

    switch (requestMethod) {

    case HTTP_GET:
        qDebug() << "Making HTTP GET";
        reply = manager->get(request);
        break;

    case HTTP_PUT:
    case HTTP_POST:        
    case HTTP_DELETE:
    default:
        qDebug() << "NO METHOD SELECTED!";
        break;
    }

    emit sentRequest();
}

void RestClient::setCredentials(const QString &user,const QString &pass)
{
    if(username)delete username;
    if(password)delete password;
    username = new QString(user);
    password = new QString(pass);
}

void RestClient::authenticationRequested(QNetworkReply *reply, QAuthenticator *authenticator)
{
    qDebug() << "AUTH REQUESTED";
    if (username == 0 || password == 0){
        qWarning() << "Credentials are not set!!";
        return;
    }

    if (authenticator->user() == *username && authenticator->password() == *password)
    {
        qDebug() << "Aborting Request due to incorrect credentials" ;
        reply->abort();
        authFailed=true;
        emit invalidCredentials();
        return;
    }

    if (username && password)
    {
        authenticator->setUser(* username);
        authenticator->setPassword(* password);
    }
}

void RestClient::replyFinished(QNetworkReply *)
{
    m_statusCode = 0;
    QByteArray data;

    if (reply)
    {
        qDebug() << "Reply exists";
        m_statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (reply->error() == QNetworkReply::NoError) {
            data = reply->readAll();
        }
        reply->deleteLater();
    }
    else{
        qDebug() << "replyFinished called with nil reply" ;
    }

    // inform delegates
    if (m_storeData)m_replyData=data;
    emit requestFinished();
    emit receivedResponse(data,m_statusCode);
}


