#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QObject>
#include <manager/servicemanager.h>
#include <datatypes/listmodel.h>
#include <datatypes/audioitem.h>
#include <datatypes/friend.h>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#define HTTP_BUFFER_SIZE (512 * 1024)
#define HTTP_BUFFER_MIN (4 * 1024)

class AudioManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ServiceManager* sm READ sm WRITE setServiceManager)
    Q_PROPERTY(ListModel* model READ model NOTIFY modelChanged)
    Q_PROPERTY(QString selected READ selected WRITE setSelected NOTIFY selectedChanged)
    Q_PROPERTY(QVariant owner READ owner WRITE setOwner NOTIFY ownerChanged)
    Q_PROPERTY(QString ownerIcon READ ownerIcon NOTIFY ownerIconChanged)
    Q_PROPERTY(QString ownerName READ ownerName NOTIFY ownerNameChanged)
    Q_PROPERTY(QString currentUrl READ currentUrl NOTIFY urlChanged)
    Q_PROPERTY(QString currentArtist READ currentArtist NOTIFY artistChanged)
    Q_PROPERTY(QString currentTrack READ currentTrack NOTIFY trackChanged)
    Q_PROPERTY(QString currentFile READ currentFile NOTIFY fileChanged)
    Q_PROPERTY(QString currentId READ currentId WRITE setCurrentId NOTIFY idChanged)
    Q_PROPERTY(bool isOwnerChanged READ isOwnerChanged NOTIFY isOwnerChangedSignal)
    Q_PROPERTY(int currentIndex READ currentIndex NOTIFY indexChanged)
    Q_PROPERTY(int loadingProgress READ loadingProgress NOTIFY progressChanged)
    Q_PROPERTY(bool isAudioUpdate READ isAudioUpdate NOTIFY isAudioUpdateChanged)
    Q_PROPERTY(bool isAudioLoading READ isAudioLoading NOTIFY loadingChanged)

public:
    explicit AudioManager(QObject *parent = 0);
    ~AudioManager();

    ServiceManager* sm();
    void setServiceManager(ServiceManager *sm);
    ListModel* model();

    const QString& selected() const;
    void setSelected(const QString& selectedId);

    const QString& ownerIcon() const;
    const QString& ownerName() const;
    const QString& currentUrl() const;
    const QString& currentArtist() const;
    const QString& currentTrack() const;
    const QString& currentFile() const;
    const QString& currentId() const;
    bool isOwnerChanged() const;
    int currentIndex() const;
    void setCurrentId(QString id);
    int loadingProgress() const;
    bool isAudioLoading() const;

    QVariant owner();
    void setOwner(QVariant owner);

    bool isAudioUpdate() const;

signals:
    void modelChanged();
    void selectedChanged();
    void ownerChanged();
    void isAudioUpdateChanged();
    void ownerIconChanged();
    void ownerNameChanged();
    void urlChanged();
    void artistChanged();
    void trackChanged();
    void fileChanged();
    void idChanged();
    void progressChanged();
    void loadingChanged();
    void indexChanged();
    void isOwnerChangedSignal();

    void httpErrorOccured(QString error);
    void fileLoaded(QString file);

public slots:
    void updateAccounts(QString accountName, AccountList list);
    void updateAudioList(Friend owner,AudioList list, bool isLastUpdate);

    void current();
    void next();
    void previous();

    void refreshAudio();
    void refreshAudio(bool isNeedUpdate);

    QString getAudioSource();
    void setAudioModel();

private slots:
    void dataAvailable();
    void httpMetaDataChanged();
    void httpError(QNetworkReply::NetworkError code);
    void httpProgress(qint64 current, qint64 total);

    void updateProxy();

private:
    AudioList m_audioList;
    AudioList m_playingList;
    QString m_selectedId;
    ListModel* m_model;
    ServiceManager *m_sm;
    Friend m_owner;
    QString m_ownerName;
    QString m_ownerIcon;
    QString m_currentUrl;
    QString m_currentArtist;
    QString m_currentTrack;
    QString m_currentFile;
    QString m_currentId;
    QString m_currentOwnerId;
    bool m_isCurrentPlayingList;
    int m_currentIndex;
    int m_progress;

    QNetworkAccessManager m_nam;
    QNetworkReply* m_reply;

    QFile* m_cacheFile; // cache file for audio

    bool m_isAudioUpdate;
    bool isLoaded;
    bool m_isAudioLoading;
    bool m_isError;
    bool m_isOwnerChanged;

    void loadFile(const QString& url);
    void httpFinished();
    void closeCache();
};

#endif // AUDIOMANAGER_H
