#include "audiomanager.h"

AudioManager::AudioManager(QObject *parent) :
    QObject(parent),
    m_reply(0),
    m_cacheFile(0)
{
    m_model = new ListModel(new AudioItem, this);
    m_isAudioUpdate = false;
    m_isAudioLoading = false;
    isLoaded = false;
    m_progress = 0;
    m_isError = false;
    m_isCurrentPlayingList = true;
    m_currentIndex = -1;

    QDir dir;
    if (!dir.exists(CoreSettings::getAudioDir()))
        dir.mkpath(CoreSettings::getAudioDir());
}

AudioManager::~AudioManager()
{
    delete m_model;
}

void AudioManager::setServiceManager(ServiceManager *sm)
{
    m_sm = sm;

    connect(m_sm, SIGNAL(updateAccounts(QString,AccountList)),
            this, SLOT(updateAccounts(QString,AccountList)));
    connect(m_sm, SIGNAL(updateAudioList(Friend,AudioList,bool)),
            this, SLOT(updateAudioList(Friend,AudioList,bool)));
    connect(m_sm, SIGNAL(settingsChanged()), SLOT(updateProxy()));
    updateProxy();

    if (m_sm->getAccounts().length() != 0)
        this->refreshAudio(false);
}

ServiceManager* AudioManager::sm()
{
    return m_sm;
}

void AudioManager::updateProxy()
{
    m_nam.setProxy(CoreSettings::getProxy());
}

ListModel* AudioManager::model()
{
    return m_model;
}

const QString& AudioManager::selected() const
{
    return m_selectedId;
}

void AudioManager::setSelected(const QString &selectedId)
{
    if (m_selectedId != selectedId) {
        m_selectedId = selectedId;
        emit selectedChanged();
    }
}

const QString& AudioManager::currentUrl() const
{
    return m_currentUrl;
}

const QString& AudioManager::currentTrack() const
{
    return m_currentTrack;
}

const QString& AudioManager::currentArtist() const
{
    return m_currentArtist;
}

const QString& AudioManager::currentFile() const
{
    return m_currentFile;
}

const QString& AudioManager::currentId() const
{
    return m_currentId;
}

bool AudioManager::isOwnerChanged() const
{
    return m_isOwnerChanged;
}

int AudioManager::currentIndex() const
{
    return m_currentIndex;
}

void AudioManager::setCurrentId(QString id)
{
    if (m_currentId != id) {
        m_currentId = id;
        emit idChanged();
    }
}

int AudioManager::loadingProgress() const
{
    return m_progress;
}

bool AudioManager::isAudioLoading() const
{
    return m_isAudioLoading;
}

QVariant AudioManager::owner()
{
    QVariant v;
    v.setValue(m_owner);
    return v;
}

void AudioManager::setOwner(QVariant owner)
{
    m_isOwnerChanged = false;
    if (owner.canConvert<Friend>()) {
        Friend newFd = owner.value<Friend>();
        if (newFd.ownerId() != m_owner.ownerId()) {
            m_owner = newFd;
            emit ownerChanged();
            m_isOwnerChanged = true;

            if (!m_owner.isEmpty()) {
                m_ownerName = m_owner.name();
                m_ownerIcon = m_owner.icon();
                m_sm->getAudio(m_owner, false, true);
            } else {
                Friend fd = m_sm->getAccounts().first().getProfile(false);
                m_ownerIcon = fd.icon();
                m_ownerName = fd.name();
                m_sm->getAudio(false, true);
            }

            emit ownerIconChanged();
            emit ownerNameChanged();
        }
    } else {
        if ((m_owner.isEmpty() && m_ownerName.isEmpty()) || !m_owner.isEmpty()) {
            Friend fd;
            m_owner = fd;
            emit ownerChanged();
            m_isOwnerChanged = true;
            fd = m_sm->getAccounts().first().getProfile(false);
            m_ownerIcon = fd.icon();
            emit ownerIconChanged();
            m_ownerName = fd.name();
            emit ownerNameChanged();

            m_sm->getAudio(false, true);
        }
    }
}

const QString& AudioManager::ownerIcon() const
{
    return m_ownerIcon;
}

const QString& AudioManager::ownerName() const
{
    return m_ownerName;
}

bool AudioManager::isAudioUpdate() const
{
    return m_isAudioUpdate;
}

void AudioManager::updateAccounts(QString, AccountList list)
{
    if (list.count() != 0)
        this->refreshAudio(false);
}

void AudioManager::refreshAudio()
{
    this->refreshAudio(true);
}

void AudioManager::refreshAudio(bool isNeedUpdate)
{
    if (!m_isAudioUpdate) {
        m_isAudioUpdate = true;
        emit isAudioUpdateChanged();
        if (!m_owner.isEmpty()) {
            m_sm->getAudio(m_owner, isNeedUpdate, true);
        } else {
            m_sm->getAudio(isNeedUpdate, true);
        }
    }
}

void AudioManager::setAudioModel()
{
    if (m_isOwnerChanged) {
        m_isOwnerChanged = false;
        m_playingList = m_audioList;
        qDebug() << __FILE__ << ":" << __FUNCTION__ << " playing list changed";
    }
}

void AudioManager::current()
{
    bool isPlayingList = false;
    if (m_playingList.first().audioId() == m_audioList.first().audioId())
        isPlayingList = true;

    for (int i = 0; i < m_playingList.length(); i++) {
        if (m_playingList.at(i).audioId() == m_currentId) {
            if (isPlayingList) {
                m_currentIndex = i;
            } else {
                m_currentIndex = -1;
            }
            emit indexChanged();

            Audio a = m_playingList.at(i);
            m_currentUrl = a.url();
            emit urlChanged();
            m_currentArtist = a.artist();
            emit artistChanged();
            m_currentTrack = a.title();
            emit trackChanged();
            m_currentFile = a.file();
            emit fileChanged();
            break;
        }
    }
}

void AudioManager::next()
{
    Audio a;
    for (int i = 0; i < m_playingList.length(); i++) {
        if (m_playingList.at(i).audioId() == m_currentId) {
            if (i == m_playingList.length() - 1)
                a = m_playingList.first();
            else
                a = m_playingList.at(i + 1);
            break;
        }
    }

    m_currentId = a.audioId();
    emit idChanged();
}

void AudioManager::previous()
{
    Audio a;
    for (int i = 0; i < m_playingList.length(); i++) {
        if (m_playingList.at(i).audioId() == m_currentId) {
            if (i == 0)
                a = m_playingList.last();
            else
                a = m_playingList.at(i - 1);
            break;
        }
    }

    m_currentId = a.audioId();
    emit idChanged();
}

void AudioManager::updateAudioList(Friend, AudioList list, bool isLastUpdate)
{
    if (isLastUpdate)
        m_isAudioUpdate = false;
    else
        m_isAudioUpdate = true;
    emit isAudioUpdateChanged();

    m_audioList = list;
    if (!m_isOwnerChanged) {
        qDebug() << __FUNCTION__ << " updated playing list";
        m_playingList = m_audioList;
    }
    m_model->clear();

    for (int i = 0; i < list.length(); i++)
        m_model->appendRow(new AudioItem(list.at(i)));

    emit modelChanged();
}

QString AudioManager::getAudioSource()
{
    if (m_nam.proxy().type() != QNetworkProxy::NoProxy) {
        if (!QFile::exists(m_currentFile)) {
            loadFile(m_currentUrl);
            return QString::null;
        } else {
            return m_currentFile;
        }
    } else {
        return m_currentUrl;
    }
}

void AudioManager::loadFile(const QString &url)
{
    if (m_reply) {
        m_reply->close();
        m_reply->deleteLater();
        m_reply = 0;
    }

    closeCache();
    m_progress = 0;
    emit progressChanged();
    isLoaded = false;
    m_isError = false;
    m_isAudioLoading = true;
    emit loadingChanged();

    m_reply = m_nam.get(QNetworkRequest(QUrl(url)));
    connect(m_reply, SIGNAL(finished()), this, SLOT(dataAvailable()));
    connect(m_reply, SIGNAL(metaDataChanged()), this, SLOT(httpMetaDataChanged()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(httpError(QNetworkReply::NetworkError)));
    connect(m_reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(httpProgress(qint64,qint64)));
}

void AudioManager::dataAvailable()
{
    if (!m_cacheFile) {
        m_cacheFile = new QFile(m_currentFile);
        m_cacheFile->open(QIODevice::WriteOnly);
        qDebug() << __FILE__ << ":" << __FUNCTION__ << "create cache for: " << m_cacheFile->fileName();
    }

    if (m_reply && m_cacheFile) {
        m_cacheFile->write(m_reply->readAll());
        m_cacheFile->flush();
    }
    httpFinished();
}

void AudioManager::httpMetaDataChanged()
{
    qDebug() << "Response: " << m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()
             << " " << m_reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
    qDebug() << "Redirect: " << m_reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toString();
    foreach (QByteArray h, m_reply->rawHeaderList())
        qDebug() << h << ": " << m_reply->rawHeader(h);
}

void AudioManager::httpError(QNetworkReply::NetworkError)
{
#ifdef DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "Http error: " << m_reply->errorString();
#endif

    emit httpErrorOccured(m_reply->errorString());
    m_isError = true;

    // delete cache file if error occured
    if (QFile::exists(m_currentFile)) {
        if (m_cacheFile->isOpen())
            m_cacheFile->close();
        m_cacheFile->remove();
    }
}

void AudioManager::httpProgress(qint64 current, qint64 total)
{
    qreal pr = ((qreal)current/total) * 100;
    m_progress = (int)pr;
    emit progressChanged();
    if (!isLoaded && (current > HTTP_BUFFER_MIN))
        isLoaded = true;
}

void AudioManager::httpFinished()
{
    if (m_isError)
        return;

#ifdef DEBUG
    qDebug() << __PRETTY_FUNCTION__ << ":" << __LINE__ << "download completed";
#endif

    if (m_nam.proxy().type() != QNetworkProxy::NoProxy) {
        m_isAudioLoading = false;
        emit loadingChanged();
        emit fileLoaded(m_currentFile);
    }
    isLoaded = false;
}

void AudioManager::closeCache()
{
    if (m_cacheFile) {
        if (m_cacheFile->isOpen())
            m_cacheFile->close();
        delete m_cacheFile;
        m_cacheFile = 0;
    }
}
