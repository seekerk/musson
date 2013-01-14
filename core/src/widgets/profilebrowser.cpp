#include "profilebrowser.h"

ProfileBrowser::ProfileBrowser(QObject *parent) :
    QObject(parent)
{
    m_isProfileUpdate = false;
    serviceIds["facebook"] = "fb";
    serviceIds["vkontakte"] = "vk";
    serviceIds["mymail"] = "mm";
    serviceIds["flickr"] = "fl";
    serviceIds["photobucket"] = "pb";
}

void ProfileBrowser::setProfile(const QVariant profile)
{
    services.clear();
    if (profile.canConvert<Friend>()) {
        m_profile = profile.value<Friend>();
        updateProfile(m_profile);

        m_sm->getProfile(m_profile.accountId(), m_profile.ownerId(), false, true, !m_profile.hasFullProfile());

        services.append(serviceIds[m_profile.serviceId()]);
        FriendList profiles = m_profile.getProfileList();
        for (int i = 0; i < profiles.length(); i++) {
            QString service = profiles.at(i).serviceId();
            services.append(serviceIds[service]);
        }

        emit servicesModelChanged();
        emit profileChanged();
    }
}

void ProfileBrowser::updateProfile(Friend profile)
{
    if (m_profile.ownerId() == profile.ownerId() &&
            m_profile.accountId() == profile.accountId()) {
        m_profile = profile;
        this->setName(m_profile.name());
        this->setNick(m_profile.nickName());
        this->setIcon(m_profile.icon());
        this->setGender(m_profile.gender());
        this->setBirthday(m_profile.birthday());
        this->setOnline(m_profile.online());
        this->setHomePhone(m_profile.homePhone());
        this->setMobilePhone(m_profile.mobilePhone());
        this->setCountry(m_profile.country());
        this->setCity(m_profile.city());
    }

    m_isProfileUpdate = false;
    emit isProfileUpdateChanged();
}

const QVariant ProfileBrowser::profile() const
{
    QVariant v;
    v.setValue(m_profile);
    return v;
}

QVariant ProfileBrowser::servicesModel()
{
    QVariant v = QVariant::fromValue(services);
    return v;
}

const QString& ProfileBrowser::name() const
{
    return m_name;
}

void ProfileBrowser::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;
        emit nameChanged();
    }
}

const QString& ProfileBrowser::icon() const
{
    return m_icon;
}

void ProfileBrowser::setIcon(const QString &icon)
{
    if (m_icon != icon) {
        m_icon = icon;
        emit iconChanged();
    }
}

const QString& ProfileBrowser::nick() const
{
    return m_nick;
}

void ProfileBrowser::setNick(const QString &nick)
{
    if (m_nick != nick) {
        m_nick = nick;
        emit nickChanged();
    }
}

const QString& ProfileBrowser::gender() const
{
    return m_gender;
}

void ProfileBrowser::setGender(const QString &gender)
{
    if (m_gender != gender) {
        m_gender = gender;
        emit genderChanged();
    }
}

const QString& ProfileBrowser::birthday() const
{
    return m_birthday;
}

void ProfileBrowser::setBirthday(const QString &birthday)
{
    if (m_birthday != birthday) {
        m_birthday = birthday;
        emit birthdayChanged();
    }
}

bool ProfileBrowser::online() const
{
    return m_online;
}

void ProfileBrowser::setOnline(const bool &online)
{
    if (m_online != online) {
        m_online = online;
        emit onlineChanged();
    }
}

const QString& ProfileBrowser::homePhone() const
{
    return m_homePhone;
}

void ProfileBrowser::setHomePhone(const QString &number)
{
    if (m_homePhone != number) {
        m_homePhone = number;
        emit homePhoneChanged();
    }
}

const QString& ProfileBrowser::mobilePhone() const
{
    return m_mobilePhone;
}

void ProfileBrowser::setMobilePhone(const QString &number)
{
    if (m_mobilePhone != number) {
        m_mobilePhone = number;
        emit mobilePhoneChanged();
    }
}

const QString& ProfileBrowser::country() const
{
    return m_country;
}

void ProfileBrowser::setCountry(const QString &country)
{
    if (m_country != country) {
        m_country = country;
        emit countryChanged();
    }
}

const QString& ProfileBrowser::city() const
{
    return m_city;
}

void ProfileBrowser::setCity(const QString &city)
{
    if (m_city != city) {
        m_city = city;
        emit cityChanged();
    }
}

bool ProfileBrowser::isProfileUpdate() const
{
    return m_isProfileUpdate;
}

void ProfileBrowser::refreshProfile()
{
    if (!m_isProfileUpdate) {
        m_isProfileUpdate = true;
        emit isProfileUpdateChanged();

        m_sm->getProfile(m_profile.accountId(), m_profile.ownerId(), true, true, true);
    }
}

void ProfileBrowser::setServiceManager(ServiceManager *sm)
{
    m_sm = sm;
    connect(m_sm, SIGNAL(updateProfile(Friend)), this, SLOT(updateProfile(Friend)));
}

ServiceManager* ProfileBrowser::sm()
{
    return m_sm;
}
