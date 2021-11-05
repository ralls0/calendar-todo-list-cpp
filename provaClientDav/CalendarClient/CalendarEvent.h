/**
 * @author      Created by Marco Manco on 16/10/21.
 *
 * @brief         Class for Calendar events
 */

#ifndef PROVACLIENTDAV_CALENDAREVENT_H
#define PROVACLIENTDAV_CALENDAREVENT_H

/**
 * Library
 */

#include <QDateTime>
#include <QObject>
#include <QString>

/**
 * Class
 *
 * @brief CalendarEvent represents an event within an ICS or CalDAV calendar.
 *
 * When owned by a CalendarClient object, the start date and end date of the
 * CalendarEvent object are stored as specified by the CalDAV server or ICS
 * file. Thus the event might apply to several dates if a RRULE value is set.
 *
 */

class CalendarEvent : public QObject {
Q_OBJECT

public:
    // Display name of the calendar where this event originates from
    Q_PROPERTY(QString calendarName READ calendarName WRITE setCalendarName NOTIFY
                       calendarNameChanged)

    // Color of the calendar where this event originates from
    Q_PROPERTY(QString color READ getColor WRITE setColor NOTIFY colorChanged)

    // Summary or title of the event
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

    // Location, where the event occurs
    Q_PROPERTY(
            QString location READ location WRITE setLocation NOTIFY locationChanged)

    // Detailed description of the event
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY
                       descriptionChanged)

    // Start date and time; event applies since this point in time
    Q_PROPERTY(QDateTime startDateTime READ getStartDateTime WRITE
                       setStartDateTime NOTIFY startDateTimeChanged)

    // End date and time; event applies until this point in time
    Q_PROPERTY(QDateTime endDateTime READ getEndDateTime WRITE setEndDateTime
                       NOTIFY endDateTimeChanged)

    // iCalendar RRULE value as string
    Q_PROPERTY(QString rrule READ getRRULE WRITE setRRULE NOTIFY rruleChanged)

    // iCalendar EXDATE value as string
    Q_PROPERTY(
            QString exdates READ getExdates WRITE setExdates NOTIFY exdatesChanged)

    // Event categories, separated by ","
    Q_PROPERTY(QString categories READ getCategories WRITE setCategories NOTIFY
                       categoriesChanged)

    // Unique ID of event
    Q_PROPERTY(QString uid READ getUID WRITE setUID NOTIFY uidChanged)

    // URL of calendar file
    Q_PROPERTY(QString href READ getHREF WRITE setHREF NOTIFY hrefChanged)

    // Read-only boolean property which is set, if the event represents a specific
    // occurrence and an EXDATE value applies
    Q_PROPERTY(bool isCanceled READ getIsCanceled NOTIFY isCanceledChanged)

    /***** End of: Q_PROPERTY Properties ************************************/ /*@}*/

    /******************************************************************************/
    /* Constructors */
    /******************************************************************************/
public:
    explicit CalendarEvent(QObject *parent);
    CalendarEvent(const CalendarEvent &other);

    /******************************************************************************/
    /* Destructor */
    /******************************************************************************/
public:
    /******************************************************************************/
    /* Public function prototypes */
    /******************************************************************************/
public:
    CalendarEvent &operator=(const CalendarEvent &other);

    bool operator<(const CalendarEvent &other) const;

    void copyFrom(const CalendarEvent &other);

    /******************************************************************************/
    /* Protected function prototypes */
    /******************************************************************************/
protected:
    /******************************************************************************/
    /* Private function prototypes */
    /******************************************************************************/
private:
    /******************************************************************************/
    /* Signals */
    /******************************************************************************/
signals:
    void colorChanged(QString &color);
    void calendarNameChanged(const QString &calendarName);
    void nameChanged(const QString &name);
    void locationChanged(const QString &location);
    void descriptionChanged(const QString &description);
    void startDateTimeChanged(const QDateTime &startDateTime);
    void endDateTimeChanged(const QDateTime &endDateTime);
    void rruleChanged(const QString &rrule);
    void exdatesChanged(const QString &exdates);
    void categoriesChanged(const QString &categories);
    void uidChanged(const QString &uid);
    void hrefChanged(const QString &href);
    void isCanceledChanged(const bool &isCanceled);

    /******************************************************************************/
    /* Public slots */
    /******************************************************************************/
public slots:
    QString getColor(void) const;
    void setColor(const QString &color);

    QString calendarName() const;
    void setCalendarName(const QString &calendarName);

    QString name() const;
    void setName(const QString &name);

    QString location() const;
    void setLocation(const QString &location);

    QString description() const;
    void setDescription(const QString &description);

    QDateTime getStartDateTime() const;
    void setStartDateTime(const QDateTime &startDateTime);

    QDateTime getEndDateTime(void) const;
    void setEndDateTime(const QDateTime &endDateTime);

    QString getRRULE() const;
    void setRRULE(const QString &rRule);

    QString getExdates() const;
    void setExdates(const QString &exdates);

    QString getCategories() const;
    void setCategories(const QString &categories);

    bool getIsCanceled() const;
    void setIsCanceled(const bool &isCanceled);

    QString getUID(void) const;
    void setUID(const QString &uid);

    QString getHREF(void) const;
    void setHREF(const QString &href);

    // to edit an event and upload it back to the CalDAV server we need to know
    // the CalendarClient object where this event originates from
    QObject *getCalendarPointer(void) const;

    void setCalendarPointer(QObject *ptr);

protected:
    QString m_Color;
    QString m_calendarName;
    QString m_Name;
    QString m_Location;
    QString m_Description;
    QDateTime m_StartDateTime;
    QDateTime m_EndDateTime;
    QString m_RRULE;
    QString m_Exdates;
    QString m_Categories;
    QString m_UID;
    QString m_HREF;
    bool m_IsCanceled;
    QObject *m_CalendarPointer;
};

#endif // PROVACLIENTDAV_CALENDAREVENT_H
