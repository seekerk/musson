#include "accountmanager.h"
#include <QDebug>
#include <QDir>
#include "datatypes/account.h"

AccountManager::AccountManager(QObject *parent) :
    QObject(parent)
{
    m_model = new ListModel(new AccountItem, this);
    m_profilesModel = new ListModel(new ProfileItem, this);
    m_isDisabled = false;
}

AccountManager::~AccountManager()
{
    delete m_model;
    delete m_profilesModel;
}

void AccountManager::setServiceManager(ServiceManager *sm)
{
    m_sm = sm;
    AccountList acc = m_sm->getAccounts();
    this->updateAccounts(QString::null, acc);

    connect(m_sm, SIGNAL(updateAccounts(QString,AccountList)),
            this, SLOT(updateAccounts(QString,AccountList)));
    connect(m_sm, SIGNAL(updateProfile(Friend)),
            this, SLOT(gotProfile(Friend)));
}

ServiceManager *AccountManager::sm()
{
    return m_sm;
}

ListModel* AccountManager::model()
{
    return m_model;
}

ListModel* AccountManager::profilesModel()
{
    return m_profilesModel;
}

QVariant AccountManager::myProfile()
{
    QVariant ret;

    Friend profile = m_sm->getMyProfile(false);
    ret.setValue(profile);
    return ret;
}

QString AccountManager::myProfileIcon()
{
    Friend profile = m_sm->getMyProfile(false);
    return profile.icon();
}

QString AccountManager::myProfileName()
{
    Friend profile = m_sm->getMyProfile(false);
    return profile.name();
}

const QString& AccountManager::selected() const
{
    return m_selectedId;
}

void AccountManager::setSelected(const QString &selectedId)
{
    if (m_selectedId != selectedId) {
        m_selectedId = selectedId;
        emit selectedChanged();
    }
}

void AccountManager::updateAccounts(QString, AccountList list)
{
    m_model->clear();
    m_profilesModel->clear();

    Friend profile = m_sm->getMyProfile(false);
    m_profilesList = profile.getProfileList();

    for (int i = 0; i < m_profilesList.length(); i++) {
        if (m_profilesList.at(i).name().isEmpty())
            m_profilesList.removeAt(i);
    }

    for (int i = 0; i < m_profilesList.length(); i++) {
        QString service;
        for (int j = 0; j < list.length(); j++) {
            if (m_profilesList.at(i).accountId() == list.at(j).accountId()) {
                DriverInfo info = list.at(j).getDriverInfo();
                service = info.name();
                break;
            }
        }
        m_profilesModel->appendRow(new ProfileItem(m_profilesList.at(i), service));
    }

    QString service;
    for (int j = 0; j < list.length(); j++) {
        if (profile.accountId() == list.at(j).accountId()) {
            DriverInfo info = list.at(j).getDriverInfo();
            service = info.name();
            break;
        }
    }

    if (!profile.isEmpty() && !profile.name().isEmpty()) {
        m_profilesModel->appendRow(new ProfileItem(profile, service));
        m_profilesList.append(profile);
    }

    emit profilesModelChanged();

    for (int i = 0; i < list.length(); i++) {
        m_model->appendRow(new AccountItem(list.at(i)));
        if (list.at(i).accountId() == m_selectedId) {
            m_isDisabled = list.at(i).isDisabled();
        }
    }

    emit modelChanged();
    emit disabledChanged();
}

void AccountManager::refreshProfile()
{
    m_sm->getMyProfile(true);
}

void AccountManager::gotProfile(Friend profile)
{
    QString service;
    AccountList list = m_sm->getAccounts();
    for (int j = 0; j < list.length(); j++) {
        if (profile.accountId() == list.at(j).accountId()) {
            DriverInfo info = list.at(j).getDriverInfo();
            service = info.name();
            break;
        }
    }

    for (int i = 0; i < m_profilesList.length(); i++) {
        if (m_profilesList.at(i).ownerId() == profile.ownerId()) {
            m_profilesModel->removeRow(i);
            m_profilesModel->insertRow(i, new ProfileItem(profile, service));
        }
    }
}

void AccountManager::addAccount(QString accountName, DriverInfo* driver)
{
    QString accName = accountName;

    if (!accountName.isEmpty()) {
        QDir dir(CoreSettings::getAccountDir(accountName));
        if (dir.exists())
            accName = Account::generateNewName(accountName);
    }

    m_sm->addAccount(accName, *driver);
    delete driver;
}

void AccountManager::addAccount(DriverInfo *driver)
{
    m_sm->addAccount(QString::null, *driver);
    delete driver;
}

void AccountManager::reconnect()
{
    ListItem* item = m_model->find(m_selectedId);
    if (!item)
        return;

    QVariant v = item->data(AccountItem::DataRole);
    if (v.canConvert<Account>()) {
        Account acc = v.value<Account>();
        acc.getProfile(false);
    }
}

void AccountManager::deleteAccount()
{
    ListItem* item = m_model->find(m_selectedId);
    if (!item)
        return;

    QVariant v = item->data(AccountItem::DataRole);
    if (v.canConvert<Account>()) {
        Account acc = v.value<Account>();
        m_sm->deleteAccount(acc);
    }
}

void AccountManager::toggleAccount()
{
    ListItem* item = m_model->find(m_selectedId);
    if (!item)
        return;

    QVariant v = item->data(AccountItem::DataRole);
    if (v.canConvert<Account>()) {
        Account acc = v.value<Account>();
        acc.setDisabled(!acc.isDisabled());
    }
}

bool AccountManager::isDisabled() const
{
    return m_isDisabled;
}

void AccountManager::setDisabled(bool disabled)
{
    if (m_isDisabled != disabled) {
        m_isDisabled = disabled;
        emit disabledChanged();
    }
}

bool AccountManager::hasAccounts() const
{
    if (m_sm->getAccounts().length() > 0) return true;

    return false;
}
