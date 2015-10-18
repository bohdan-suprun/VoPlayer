#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QDialog>
#include "requestmanager.h"

namespace Ui {
class Registration;
}

class Registration : public QDialog
{
    Q_OBJECT

public:
    Registration(QWidget *parent);
    ~Registration();
signals:
    void autorisOk(QString,QString);
private slots:
    void data(QMap<QString,QString>,Action);
    void displayError(QString);
    void on_label_8_linkActivated(const QString &link);
    void on_signInBtn_clicked();
    void on_regBtn_clicked();
    void on_pushButton_clicked();

private:
    RequestManager* req;
    Ui::Registration *ui;
};

#endif // REGISTRATION_H
