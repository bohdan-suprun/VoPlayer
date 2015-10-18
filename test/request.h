#ifndef REQUEST_H
#define REQUEST_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>

enum Action{
    REGISTRATION=0,
    LOG_IN,
    WRITE,
    LOG_OUT,
    READ,
    LOG_IN_VIA_HASH,
    INFO,
    TRANSLATION,
};

class Request : public QObject
{
    Q_OBJECT
public:
    explicit Request(QObject *parent = 0);
    void send(QString);
    QStringList pasAndHash(QString email,QString pas=QString());
signals:
    void error(QString);
    void dataReciev(QMap<QString,QString>,Action);
public slots:
private slots:
    void onTimeOut();
    void readData();
    void smError(QNetworkReply::NetworkError);
private:
    QString key(int, QString email="");
    QString getContent(QString content, QString tag);
    QString MAC;
    QNetworkAccessManager manager;
    QNetworkReply* rep;
    QTimer timer;

};

#endif // REQUEST_H
