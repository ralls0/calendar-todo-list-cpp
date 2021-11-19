/**
 * @author      Created by Marco Manco.
 * @file        ClientCalDAV.h
 *
 * @brief       ClientCalDAV class used for CalDAV
 */

#include "./ClientCalDAV.h"

#define DEBUG_CALENDARCLIENT_CALDAV 0
#if DEBUG_CALENDARCLIENT_CALDAV
#define QDEBUG qDebug()
#else
#define QDEBUG                                                                 \
  if (0)                                                                       \
  qDebug()
#endif

ClientCalDAV::ClientCalDAV(const QString &Username,
                           const QString &Password,
                           const QString &Color,
                           const QUrl &HostURL,
                           const QString &DisplayName):
                           _Username(Username),
                           _Password(Password),
                           _Color(Color),
                           _HostURL(HostURL),
                           _DisplayName(DisplayName),
                           _RequestTimeoutMS(32000),
                           _pUploadReply(nullptr){
    QObject::connect(this, &ClientCalDAV::accessTokenChanged,
                     this, &ClientCalDAV::saveEvent);

}

QString ClientCalDAV::encodeBase64(QString string) {
    QByteArray ba;
    ba.append(string.toUtf8());
    return ba.toBase64();
}

void ClientCalDAV::handleUploadFinished(void) {
    _UploadRequestTimeoutTimer.stop();

    QDEBUG << _DisplayName << ": "
           << "HTTP upload finished";

    if (NULL != _pUploadReply) {
        std::cout<<_DisplayName.toStdString() << ":\r\n"
                 << "received:\r\n"
                 << _pUploadReply->readAll().toStdString();
    }
}

void ClientCalDAV::setAccessToken(QString token) {
    _accessToken = token;
    std::cout<<"CalDAV Token: "<<_accessToken.toStdString()<<"\n";
    emit accessTokenChanged();
}

void ClientCalDAV::saveEvent() {/*QString uid, QString filename,
                                      QString summary, QString location,
                                      QString description, QString rrule,
                                      QString exdate, QDateTime startDateTime,
                                      QDateTime endDateTime) {*/

    QString uid = "";
    QString filename = "";
    QString summary = "Prova";
    QString location = "Casa Mia";
    QString description = "Messappunto Google OAuth";
    QString rrule = "";
    QString exdate = "";
    QDateTime startDateTime = QDateTime();
    QDateTime endDateTime = QDateTime();

    QDate sdate = QDate(2021, 12, 22);
    QDate edate = QDate(2022, 1, 6);

    startDateTime.setDate(sdate);
    endDateTime.setDate(edate);

    QDEBUG << _DisplayName << ": "
           << "saving event" << summary;

    if (NULL != _pUploadReply) {
        QDEBUG << _DisplayName << ": "
               << "cleaning up _pUploadReply";
        _pUploadReply->abort();
        _pUploadReply->disconnect();
    }

    std::cout<<"non sono abbortito\n";
    QString authorization = "Bearer ";
    authorization.append(_accessToken);

    QBuffer *buffer = new QBuffer();

    buffer->open(QIODevice::ReadWrite);

    if (uid.isEmpty()) {
        std::cout<<"uid empty\n";
        uid = QDateTime::currentDateTime().toString("yyyyMMdd-HHMM-00ss") +
              "-0000-" + startDateTime.toString("yyyyMMddHHMM");
    }

    if (filename.isEmpty()) {
        std::cout<<"filename empty\n";
        filename = uid + ".ics";
    }

    std::cout<<"creo stringa di richiesta\n";
    QString requestString =
            "BEGIN:VCALENDAR\r\n"
            "BEGIN:VEVENT\r\n"
            "UID:" +
            uid +
            "\r\n"
            "VERSION:2.0\r\n"
            "DTSTAMP:" +
            QDateTime::currentDateTime().toString("yyyyMMddTHHmmssZ") +
            "\r\n"
            "SUMMARY:" +
            summary +
            "\r\n"
            "DTSTART:" +
            startDateTime.toString("yyyyMMddTHHmmss") +
            "\r\n"
            "DTEND:" +
            endDateTime.toString("yyyyMMddTHHmmss") +
            "\r\n"
            "LOCATION:" +
            location +
            "\r\n"
            "DESCRIPTION:" +
            description +
            "\r\n"
            "TRANSP:OPAQUE\r\n";

    if (!rrule.isEmpty()) {
        std::cout<<"rrule non e' empty\n";
        requestString.append("RRULE:" + rrule + "\r\n");
    }

    if (!exdate.isEmpty()) {
        std::cout<<"exdata non e' empty\n";
        requestString.append("EXDATE:" + exdate + "\r\n");
    }

    std::cout<<"appendo la fine\n";
    requestString.append("END:VEVENT\r\nEND:VCALENDAR");

    std::cout<<"prendo la dime del buffer\n";
    int buffersize = buffer->write(requestString.toUtf8());
    buffer->seek(0);
    buffer->size();

    QByteArray contentlength;
    contentlength.append(QString::number(buffersize).toUtf8());

    std::cout<<"creo richiesta header\n";
    QNetworkRequest request;
    request.setUrl(QUrl(_HostURL.toString() + filename));
    request.setRawHeader("User-Agent", "CalendarClient_CalDAV");
    request.setRawHeader("Authorization", authorization.toUtf8());
    request.setRawHeader("Depth", "0");
    request.setRawHeader("Prefer", "return-minimal");
    request.setRawHeader("Content-Type", "text/calendar; charset=utf-8");
    request.setRawHeader("Content-Length", contentlength);

    std::cout<<"start ssl config\n";
    QSslConfiguration conf = request.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(conf);

    std::cout<<"put _pUploadReply\n";
    _pUploadReply = _UploadNetworkManager.put(request, buffer);

    if (NULL != _pUploadReply) {
//    connect(_pUploadReply, SIGNAL(error(QNetworkReply::NetworkError)), this,
//            SLOT(handleUploadHTTPError()));

        connect(_pUploadReply, SIGNAL(finished()), this,
                SLOT(handleUploadFinished()));

        _UploadRequestTimeoutTimer.start(_RequestTimeoutMS);
    } else {
        QDEBUG << _DisplayName << ": "
               << "ERROR: Invalid reply pointer when requesting URL.";
    }
}