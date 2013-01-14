#ifndef PROFILEBROWSER_H
#define PROFILEBROWSER_H

#include <QObject>
#include <QVariant>
#include <manager/servicemanager.h>
#include <datatypes/friend.h>
#include "musson_global.h"

class musson_EXPORT ProfileBrowser : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ServiceManager* sm READ sm WRITE setServiceManager)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString nick READ nick WRITE setNick NOTIFY nickChanged)
    Q_PROPERTY(QString icon READ icon WRITE setIcon NOTIFY iconChanged)
    Q_PROPERTY(QString birthday READ birthday WRITE setBirthday NOTIFY birthdayChanged)
    Q_PROPERTY(QString gender READ gender WRITE setGender NOTIFY genderChanged)
    Q_PROPERTY(QString mobilePhone READ mobilePhone WRITE setMobilePhone NOTIFY mobilePhoneChanged)
    Q_PROPERTY(QString homePhone READ homePhone WRITE setHomePhone NOTIFY homePhoneChanged)
    Q_PROPERTY(QString country READ country WRITE setCountry NOTIFY countryChanged)
    Q_PROPERTY(QString city READ city WRITE setCity NOTIFY cityChanged)
    Q_PROPERTY(QVariant profile READ profile WRITE setProfile NOTIFY profileChanged)
    Q_PROPERTY(bool online READ online WRITE setOnline NOTIFY onlineChanged)
    Q_PROPERTY(bool isProfileUpdate READ isProfileUpdate NOTIFY isProfileUpdateChanged)
    Q_PROPERTY(QVariant servicesModel READ servicesModel NOTIFY servicesModelChanged)
public:
    explicit ProfileBrowser(QObject *parent = 0);

    const QVariant profile() const;

    const QString& name() const;
    void setName(const QString& name);

    const QString& icon() const;
    void setIcon(const QString& icon);

    const QString& nick() const;
    void setNick(const QString& nick);

    const QString& birthday() const;
    void setBirthday(const QString& birthday);

    const QString& gender() const;
    void setGender(const QString& gender);

    const QString& mobilePhone() const;
    void setMobilePhone(const QString& number);

    const QString& homePhone() const;
    void setHomePhone(const QString& number);

    const QString& country() const;
    void setCountry(const QString& country);

    const QString& city() const;
    void setCity(const QString& city);

    bool online() const;
    void setOnline(const bool& online);

    ServiceManager* sm();
    void setServiceManager(ServiceManager *sm);

    QVariant servicesModel();

    bool isProfileUpdate() const;

signals:
    void nameChanged();
    void iconChanged();
    void nickChanged();
    void birthdayChanged();
    void genderChanged();
    void onlineChanged();
    void homePhoneChanged();
    void mobilePhoneChanged();
    void countryChanged();
    void cityChanged();
    void profileChanged();
    void isProfileUpdateChanged();
    void servicesModelChanged();

public slots:
    void setProfile(const QVariant profile);
    void updateProfile(Friend profile);
    void refreshProfile();

private:
    Friend m_profile;
    QString m_name;
    QString m_icon;
    QString m_nick;
    QString m_birthday;
    QString m_gender;
    bool m_online;
    QString m_mobilePhone;
    QString m_homePhone;
    QString m_country;
    QString m_city;

    ServiceManager *m_sm;
    bool m_isProfileUpdate;
    QStringList services;
    QHash<QString, QString> serviceIds;
};

#endif // PROFILEBROWSER_H
