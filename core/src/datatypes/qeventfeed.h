#ifndef QEVENTFEED_H
#define QEVENTFEED_H

#include <QObject>
#include <QDomDocument>
#include <QMetaType>
#include <QSharedData>

#include "musson_global.h"
#include "utils/qrefreshlist.h"
#include "datatypes/qattachment.h"
#include "datatypes/baseobject.h"

class QEventFeed;
class Photo;

/*! \typedef QEventFeedList
 Structure for stacking list of events from feed.
 \sa QRefreshList
*/
typedef QRefreshList<QEventFeed> QEventFeedList;

Q_DECLARE_METATYPE(QEventFeed)
Q_DECLARE_METATYPE(QEventFeedList)

#define FILE_FEED_DATA "feed_%1.xml"

#define NODE_FEED_ROOT "feed"

#define NODE_EVENT_ROOT "event"

#define NODE_EVENT_OWNERID "ownerId"

#define NODE_EVENT_OWNER_NAME "ownerName"

#define NODE_EVENT_TEXT "text"

#define NODE_EVENT_CREATED "created"

class QEventFeedData;

/**
  \brief Class of event from a feed.
  */
class musson_EXPORT QEventFeed : public BaseObject
{
    QSharedDataPointer<QEventFeedData> d;
public:
    /**
      \brief List of supported feeds.

      \sa getFeed()
      */
    enum FeedType
    {
        photoFeed, /**< \brief new images on service. */
        messageFeed, /**< \brief New messages (status, not, etc.) on service. */
        commentFeed, /**< \brief New comments on service. */
        videoFeed, /**< \brief New videos on service */
        audioFeed, /**< \brief New audios on service */
        likeFeed, /**< \brief New likes on service */
        tagFeed, /**< \brief New tags on images on service */
        friendFeed /**< \brief New friends and friends of friends on service */
    };

    /**
      \brief The default constructor.

      \param parent The pointer to parent object.
      */
    QEventFeed();

    /**
      \brief The copy constructor.

      \param src The source instance of QEventFeed class.
      */
    QEventFeed(const QEventFeed& src);

    /**
      \brief Destructor.
     */
    ~QEventFeed();

    QEventFeed& operator=(const QEventFeed& source);

    static QString getKey(const QString accountId, const FeedType type);

    /*!
     \brief Store feed data to XML format.

     \param doc root XML document

     \return XML structure
     */
    QDomElement toQDomElement(QDomDocument& doc) const;

    /*!
     \brief Loads feed from XML struct.

     \param fr XML struct with feed data

     \param accountId ID of account to which feed belongs

     \param type The type of feed.

     \return News feed.
     */
    static QEventFeed fromQDomElement(const QDomElement& fr, const QString& srvId, const QString& accountId, const FeedType type);

    Photo toPhoto() const;

    static QString typeToString(const FeedType type);

    // Getters
    const QString ownerName() const;
    const QString text() const;
    const QDateTime created() const;
    const QAttachment attach() const;

    // Setters
    void setOwnerName(const QString& v);
    void setText(const QString& v);
    void setCreated(const QDateTime& v);
    void setAttach(const QAttachment& v);
};

Q_DECLARE_METATYPE(QEventFeed::FeedType)

#endif // QEVENTFEED_H
