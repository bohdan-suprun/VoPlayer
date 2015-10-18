#include <QNetworkInterface>
#include <QSettings>
#include "requestmanager.h"
#define ADRESS "http://"+host+":8080/Voplayer/reg?"

RequestManager::RequestManager(QObject *parent) :
    QObject(parent)
{
    request=new Request();
    QNetworkInterface net;
    foreach (QNetworkInterface value, net.allInterfaces())
       if(value.humanReadableName()!="lo"){
          MAC=value.hardwareAddress();
          break;
        }
    connect(request,SIGNAL(dataReciev(QMap<QString,QString>,Action)),this,SLOT(receivData(QMap<QString,QString>,Action)));
    connect(request,SIGNAL(error(QString)),this,SLOT(smError(QString)));
    readConfig();
}
void RequestManager::readConfig(){
    QSettings set("4th group","VoPlayer");
    set.beginGroup("Network");
    hash=set.value("Hash","-1").toString().trimmed();
    host=set.value("HostName","109.87.34.136").toString();
}

void RequestManager::writeConfig(){
    QSettings set("4th group","VoPlayer");
    set.beginGroup("Network");
    set.setValue("Hash",hash);
    set.setValue("HostName",host);
}
void RequestManager:: registration(QString name, QString email, QString pas){
    action=REGISTRATION;

    email=QByteArray().append(email).toBase64(QByteArray::Base64UrlEncoding);
    name = QByteArray().append(name).toBase64(QByteArray::Base64UrlEncoding);
    QStringList ls=request->pasAndHash(email,pas);
    request->send(ADRESS+"action=0&email="+email+"&pass="+ls[1]+"&name="+name+"&mac="+MAC);
}

void RequestManager:: logIn(QString e, QString pas){
    action=LOG_IN;

    if(hash!="-1")
        logOut();

    QString email= QByteArray().append(e).toBase64(QByteArray::Base64UrlEncoding);
    QStringList ls=request->pasAndHash(e,pas);
    request->send(ADRESS+"action=1&email="+email+"&pass="+ls[1]+"&mac="+MAC);
}

void RequestManager:: write(QString ru,QString en){
    readConfig();

    ru= QByteArray().append(ru).toBase64(QByteArray::Base64UrlEncoding);
    en= QByteArray().append(en).toBase64(QByteArray::Base64UrlEncoding);
    request->send(ADRESS+"action=2&hash="+hash+"&ru="+ru+"&en="+en+"&mac="+MAC);
}

void RequestManager::logOut(){

    readConfig();
    request->send(ADRESS+"action=3&hash="+request->pasAndHash(hash)[0]+"&mac="+MAC);
    hash="-1";
    writeConfig();

}

void RequestManager:: read(unsigned int number){

    readConfig();
    request->send(ADRESS+"action=4&hash="+hash+"&number="+QString().number(number)+"&mac="+MAC);

}

void RequestManager:: viaHash(){
    readConfig();
    request->send(ADRESS+"action=5&hash="+hash+"&mac="+MAC);

}

void RequestManager:: info(){
    readConfig();
    request->send(ADRESS+"action=6&hash="+hash+"&mac="+MAC);

}

void RequestManager:: translate(QString ru){

    originalText=ru;
    const QString apiKey=tr("trnsl.1.1.20140914T211042Z.23946adf3b5d4140.b103b344bc140037557d45d3f7192a5b51b85d36");
    QString requestStr=tr("https://translate.yandex.net/api/v1.5/tr/translate?key=");
    requestStr+=apiKey+tr("&text=");
    requestStr+=ru+tr("&lang=en-ru");
    requestStr+=tr("&format=plain&options=1");
    request->send(requestStr);
}
void RequestManager::receivData(QMap<QString,QString> data, Action ac){

    if(ac==LOG_IN){
          hash=data["hash"];
          writeConfig();
    }

    if(ac==TRANSLATION)
        data.insert("original",originalText);

    emit finish(data,ac);
}
void RequestManager::smError(QString data){
    emit error(data);
}
