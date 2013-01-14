#include "albumsmanager.h"

AlbumsManager::AlbumsManager(QObject *parent) :
    QObject(parent)
{
    m_model = new ListModel(new AlbumItem, this);
    m_isAlbumsUpdate = false;
    m_showMyAlbums = false;
}

AlbumsManager::~AlbumsManager()
{
    delete m_model;
}

void AlbumsManager::setServiceManager(ServiceManager *sm)
{
    m_sm = sm;

    connect(m_sm, SIGNAL(updateAccounts(QString,AccountList)),
            this, SLOT(updateAccounts(QString,AccountList)));
    connect(m_sm, SIGNAL(updateAlbumList(Friend,AlbumList,bool)),
            this, SLOT(updateAlbums(Friend,AlbumList,bool)));
}

void AlbumsManager::initAlbums()
{
    AccountList acc = m_sm->getAccounts();
    this->updateAccounts(QString::null, acc);

    if (this->m_sm->getAccounts().length() != 0)
        this->refreshAlbums(false);
}

ServiceManager* AlbumsManager::sm()
{
    return m_sm;
}

ListModel* AlbumsManager::model()
{
    return m_model;
}

const QString& AlbumsManager::selected() const
{
    return m_selectedId;
}

void AlbumsManager::setSelected(const QString &selectedId)
{
    if (m_selectedId != selectedId) {
        m_selectedId = selectedId;
        emit selectedChanged();
    }
}

const QString& AlbumsManager::profileIcon() const
{
    return m_profileIcon;
}

QVariant AlbumsManager::owner()
{
    QVariant v;
    v.setValue(m_owner);
    return v;
}

void AlbumsManager::setOwner(QVariant owner)
{
    if (owner.canConvert<Friend>()) {
        m_owner = owner.value<Friend>();

        if (!m_owner.isEmpty()) {
            m_sm->getAlbums(m_owner, false, true);
        } else {
            if (m_sm->getAccounts().length() > 0)
            {
                m_profileIcon = m_sm->getAccounts().first().getProfile(false).icon();
            } else {
                m_profileIcon = "";
            }
            emit profileIconChanged();
            m_sm->getAlbums(false, true);
        }
        emit ownerChanged();
    } else {
        Friend fd;
        m_owner = fd;
        if (m_sm->getAccounts().length() > 0)
        {
            m_profileIcon = m_sm->getAccounts().first().getProfile(false).icon();
        } else {
            m_profileIcon = "";
        }
        emit profileIconChanged();
        m_sm->getAlbums(false, true);
    }
}

bool AlbumsManager::isAlbumsUpdate() const
{
    return m_isAlbumsUpdate;
}

bool AlbumsManager::showMyAlbums() const
{
    return m_showMyAlbums;
}

void AlbumsManager::setShowMyAlbums(bool show)
{
    if (m_showMyAlbums != show) {
        m_showMyAlbums = show;
        emit showMyAlbumsChanged();
    }
}

void AlbumsManager::updateAccounts(QString, AccountList list)
{
    if (list.count() != 0)
        this->refreshAlbums(false);
}

void AlbumsManager::refreshAlbums()
{
    this->refreshAlbums(true);
}

void AlbumsManager::refreshAlbums(bool isNeedUpdate)
{
    if (!m_isAlbumsUpdate) {
        m_isAlbumsUpdate = true;
        emit isAlbumsUpdateChanged();
        if (!m_owner.isEmpty()) {
            m_sm->getAlbums(m_owner, isNeedUpdate, true);
        } else {
            m_sm->getAlbums(isNeedUpdate, true);
        }
    }
}

void AlbumsManager::updateAlbums(Friend, AlbumList list, bool isLastUpdate)
{
    if (isLastUpdate)
        m_isAlbumsUpdate = false;
    else
        m_isAlbumsUpdate = true;
    emit isAlbumsUpdateChanged();

    m_albumList = list;

    ListModel* m = new ListModel(new AlbumItem);
    delete m_model;
    m_model = m;

    for (int i = 0; i < list.size(); i++) {
        m_model->appendRow(new AlbumItem(list.at(i)));
    }
    emit modelChanged();
}

void AlbumsManager::createAlbum(QVariant profile, QString name, QString description, QString privacy)
{
    if (profile.canConvert<Friend>()) {
        Friend fr = profile.value<Friend>();
        m_sm->createAlbum(fr.accountId(), name, description, privacy);
    }
}
