/**
 * @author      Created by Marco Manco on 16/10/21.
 * @file        CalendarClient.h
 *
 * @brief       Classe comune per tutti i calendari.
 *              Contine il parsing per il formato iCalendar
 */

#ifndef PROVACLIENTDAV_CALENDARCLIENT_H
#define PROVACLIENTDAV_CALENDARCLIENT_H

/**
 * Library
 */

#include <QDate>
#include <QList>
#include <QObject>
#include <QTextStream>
#include <QTimer>
#include <QUrl>
#include <QtNetwork>

#include "CalendarEvent.h"

/**
 * Constants
 */

/*
 * Indica se viene usato il colore indicato dal calendario
 * o quello specificato dall'utente
 */
#define CALENDAR_OVERWRITE_COLOR 0

/**
 * Class
 *
 * @brief       Classe astratta per tutti i calendari.
 *              Consente di ottenere un oggetto CalendarEvent da una
 *              risorsa locale o remota.
 */

class CalendarClient : public QObject {
Q_OBJECT

    /**
     * Public
     */

public:
    typedef enum {
        E_STATE_IDLE, // Calendario in attesa della prossima sincronizzazione
        E_STATE_BUSY, // Calendario sincronizzato
        E_STATE_ERROR // Errore nel calendario
    } E_CalendarState;

    typedef enum {
        E_CALENDAR_ICS,   // event source è un file iCalendar locale or remoto
        E_CALENDAR_CALDAV // event source è un server CalDAV
    } E_CalendarType;

    Q_ENUM(E_CalendarState)
    Q_ENUM(E_CalendarType)

    // Colore del CalendarClient per la GUI
    Q_PROPERTY(QString color READ getColor WRITE setColor NOTIFY colorChanged)

    // Sincronizzazione dello stato del CalendarClient
    Q_PROPERTY(
            E_CalendarState syncState READ getSyncState NOTIFY syncStateChanged)

    // Tipo di calendario; i.e. quale è la fonte degli eventi del calendario
    Q_PROPERTY(E_CalendarType calendarType READ getCalendarType CONSTANT)

    // URL della da cui vengono recuperati gli eventi del calendario
    // (percorso del file iCalendar, URL del server Caldav, ...)
    Q_PROPERTY(
            QUrl hostURL READ getHostURL WRITE setHostURL NOTIFY hostURLChanged)

    // Nome visualizzato dell'istanza Calendarclient;
    // potrebbe essere sovrascritto dalle informazioni del calendario ricevute
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY
                       displayNameChanged)

    // Richiesta di timeout in ms per l'accesso alle sorgenti del calendario
    // remoto (default: 2000ms) FIXME firefox usa 30s -> 30_000ms
    Q_PROPERTY(int requestTimeoutMS READ getRequestTimeoutMS WRITE
                       setRequestTimeoutMS NOTIFY requestTimeoutMSChanged)

    CalendarClient(QObject *parent = NULL);
    ~CalendarClient();

    /**
     * @brief   Restituisce un elenco di eventi che si verificano nella data
     * inserita
     */
    Q_INVOKABLE QList<QObject *> eventsForDate(const QDate &date);

    /**
     * @brief   Restituisce l'elenco completo degli eventi gestiti da
     * Calendarclient
     */
    Q_INVOKABLE QList<QObject *> allEvents(void);

    /**
     * Protected
     */

protected:
    QString m_Color;
    E_CalendarState m_State;
    E_CalendarType m_CalendarType;
    QUrl m_HostURL;
    QString m_DisplayName;

    // Timer per risincronizzare automaticamente con la sorgente dell'evento
    QTimer m_SynchronizationTimer;

    // Flusso di prova per gestire il contenuto del file iCalendar
    QTextStream *m_DataStream;

    // Elenco degli eventi gestiti Calendarevent
    QList<CalendarEvent> m_EventList;

    QNetworkAccessManager m_NetworkManager;
    QNetworkReply *m_pReply;

    QTimer m_RequestTimeoutTimer;
    int m_RequestTimeoutMS;

    /**
     * @brief   Helper function per codificare le richieste di autorizzazione di
     * rete
     */
    QString encodeBase64(QString string);

    /**
     * @brief   Helper function per decodificare le stringhe ricevute
     */
    QString ascii2utf8(QString str);

    /**
     * @brief   Parser per i campi del calendario da un iCalendar file.
     */
    void parseCALENDAR(QString href);

    /**
     * @brief   Parser per i campi VEVENT di un iCalendar file.
     */
    void parseVEVENT(QString href);

    /**
     * @brief   Aggiunge un singolo evento a m_EventList
     *          se l'evento si verifica entro un determinato Qdatetime.
     * @note    Evento singolo = evento senza valore RRULE.
     * @note    Gli eventi con un valore RRULE sono gestiti con parseVEVENT()
     */
    QList<QObject *> handleSingleEvent(CalendarEvent &evt,
                                       const QDateTime &startOfQuestionedDate,
                                       const QDateTime &endOfQuestionedDate);

    /**
     * @brief Restituisce true, se una data passata è esclusa dal valore EXDATE
     * dell'evento.
     */
    bool isDateExcluded(const QString strExdates,
                        const QDate &questionedDate) const;

    /**
     * Private attributes
     */

private:
    /**
     * Signals
     */

signals:

    void colorChanged(QString color);
    // emesso quando il calendario è entrato in un nuovo stato di sincronizzazione
    void syncStateChanged(E_CalendarState syncState);
    void calendarTypeChanged(E_CalendarType calendarType);
    void hostURLChanged(QString hostURL);
    void displayNameChanged(QString hostURL);

    // emesso quando l'evento in m_EventList ha dei cambiamenti
    void eventsUpdated(void);

    // emesso quando vi è un errore
    void error(QString errorMsg);

    // emesso quando lo stato di errore viene recuperato
    void recoverSignal(void);

    void requestTimeoutMSChanged(int);

    // emesso per forzare la sincronizzazione con la sorgente del calendario (file
    // o server)
    void forceSynchronization(void);

    /**
     * Public slots
     */

public slots:

    /**
     * @brief Slot per avviare la sincronizzazione con la sorgente dell'evento
     */
    virtual void startSynchronization(void) = 0;

    /**
     * @brief Slot per interrompere la sincronizzazione con la sorgente
     * dell'evento
     */
    virtual void stopSynchronization(void) = 0;

    /**
     * @brief Slot per recuperare dallo stato di errore
     */
    virtual void recover(void) = 0;

    QString getColor(void) const;
    void setColor(const QString &color);

    E_CalendarState getSyncState(void);
    E_CalendarType getCalendarType(void);

    virtual bool setHostURL(const QUrl hostURL);
    QString getHostURL(void) const;

    QString getDisplayName(void) const;
    virtual void setDisplayName(QString name);

    int getRequestTimeoutMS(void) const;
    void setRequestTimeoutMS(const int requestTimeoutMS);
};

#endif // PROVACLIENTDAV_CALENDARCLIENT_H
