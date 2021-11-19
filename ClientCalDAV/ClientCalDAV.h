/**
 * @author      Created by Marco Manco.
 * @file        ClientCalDAV.h
 *
 * @brief       ClientCalDAV class used for CalDAV
 */


#ifndef QTNETWOAUTH2_CLIENTCALDAV_H
#define QTNETWOAUTH2_CLIENTCALDAV_H

#include <iostream>
#include <QtNetwork>
//#include <>

class ClientCalDAV: public QObject {

    Q_OBJECT
private:
    QNetworkAccessManager _UploadNetworkManager;
    QNetworkReply *_pUploadReply;
    QTimer _UploadRequestTimeoutTimer;
    int _RequestTimeoutMS;

    QString _Username;
    QString _Password;
    QString _Color;
    QUrl _HostURL;
    QString _DisplayName;
    QString _accessToken;

    QString encodeBase64(QString string);

public:

    ClientCalDAV(const QString &Username,
                 const QString &Password,
                 const QString &Color,
                 const QUrl &HostURL,
                 const QString &DisplayName);

    void saveEvent();/*QString uid, QString filename,
                                 QString summary, QString location,
                                 QString description, QString rrule,
                                 QString exdate, QDateTime startDateTime,
                                 QDateTime endDateTime);*/

signals:
    void accessTokenChanged(void);

protected slots:
    void handleUploadFinished(void);

public slots:
    void setAccessToken(QString token);

};

#endif //QTNETWOAUTH2_CLIENTCALDAV_H
