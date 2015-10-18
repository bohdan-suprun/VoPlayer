#include "request.h"
#include <QNetworkInterface>
#include <QCryptographicHash>
#include <QMessageBox>
#define TIME_OUT 5000

Request::Request(QObject *parent) :
    QObject(parent),rep(0)
{

}
void Request::send(QString req){
    rep=manager.get(QNetworkRequest(QUrl(req)));
    connect(rep,SIGNAL(finished()),this,SLOT(readData()));
    connect(rep,SIGNAL(error(QNetworkReply::NetworkError)),this,SLOT(smError(QNetworkReply::NetworkError)));
    QTimer::singleShot(TIME_OUT,this,SLOT(onTimeOut()));
}
void Request::readData(){
    QString res=rep->readAll();
    QMap<QString,QString> result;

    result.insert("message",getContent(res,"<message>"));

    if(!result["message"].isEmpty()){
        emit error(result["message"]);
        return;
    }
    result.insert("name",getContent(res,"<name>"));
    result.insert("hash",getContent(res,"<hash>"));
    result.insert("en",getContent(res,"<en>"));
    result.insert("ru",getContent(res,"<ru>"));
    result.insert("info",getContent(res,"<info>"));
    result.insert("translate",getContent(res,"<text>"));
    Action action;
    QString str=getContent(res,"<action>");
    if(!str.isEmpty())
        action=(Action)str.toInt();
    else
        if(!result["translate"].isEmpty())
            action=TRANSLATION;

    emit dataReciev(result,action);


}
void Request::smError(QNetworkReply::NetworkError er){
    if(er!=QNetworkReply::OperationCanceledError)
        emit error("You have some problems with your internet connection. "+ QString().number(er));

}
QStringList Request::pasAndHash(QString email,QString pass){
   QStringList result;
   pass=QCryptographicHash::hash(QByteArray().append(pass),QCryptographicHash::Md5).toBase64(QByteArray::Base64UrlEncoding);
   email=QCryptographicHash::hash(QByteArray().append(email),QCryptographicHash::Md5).toBase64(QByteArray::Base64UrlEncoding);
   result.append(email);
   result.append(pass);
   return result;
}
QString Request::getContent(QString res,QString tag){
    QString result="";
    QString endTag=tag;
    endTag.insert(1,"/");
    if(res.contains(tag))
       result=res.mid(res.indexOf(tag)+tag.length(),res.indexOf(endTag)-res.indexOf(tag)-tag.length());
    return result;
}
void Request::onTimeOut(){
    if(rep->isRunning()){
        rep->close();
        emit error("The request had been aborted because of timeout.");
    }

}
