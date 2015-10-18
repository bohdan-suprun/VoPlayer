#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include <QObject>
#include "request.h"


class RequestManager : public QObject
{
    Q_OBJECT
public:
    explicit RequestManager(QObject *parent = 0);
    RequestManager(Request*,QObject *parent = 0);
    Request * getRequest() const{return request;}

    void registration(QString name, QString email, QString pas);
    void logIn(QString email, QString pas);
    void write(QString ru,QString en);
    void logOut();
    void read(unsigned int number);
    void viaHash();
    void info();
    void translate(QString ru);
signals:
    void finish(QMap<QString,QString>,Action);
    void error(QString);
public slots:
private slots:
    void receivData(QMap<QString,QString>,Action);
    void smError(QString);
private:
    QString hash;
    QString originalText;
    QString host;
    QString MAC;
    void readConfig();
    void writeConfig();
    Request* request;
    Action action;

};

#endif // REQUESTMANAGER_H
