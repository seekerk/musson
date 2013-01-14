#ifndef QATTACHMENT_H
#define QATTACHMENT_H

#include <QObject>
#include <QDomDocument>
#include <QSharedData>
#include <QMetaType>

#include "musson_global.h"
#include "datatypes/baseobject.h"

#define NODE_ATTACH_ROOT "attach"

#define NODE_ATTACH_ID "id"

#define NODE_ATTACH_OWNERID "ownerId"

#define NODE_ATTACH_TYPE "type"

#define NODE_ATTACH_NAME "name"

#define NODE_ATTACH_ALBUMID "albumId"

#define NODE_ATTACH_ICONURL "iconUrl"

#define NODE_ATTACH_ICON "icon"

#define NODE_ATTACH_OBJECT "object"

#define NODE_ATTACH_IMAGE "image"

#define NODE_ATTACH_DURATION "duration"

class QAttachmentData;

/**
  \brief Store attachment data of event or message.
  */
class musson_EXPORT QAttachment : public BaseObject
{
    QSharedDataPointer<QAttachmentData> d;

public:
    QAttachment();

    /**
      \brief The copy constructor.

      \param src The source instance of QAttachment class.
      */
    QAttachment(const QAttachment& src);

    ~QAttachment();

    enum AttachmentType {
        ImageAttach,
        VideoAttach,
        AudioAttach,
        LinkAttach,
        NoteAttach
    };

    QAttachment& operator=(const QAttachment& src);

    /*!
     \brief Store attachment data to XML format.

     \param doc root XML document

     \return XML structure
     */
    QDomElement toQDomElement(QDomDocument& doc) const;

    static QAttachment fromQDomElement(const QDomElement& fr, const QString& srvId, const QString& accountId);

    const QString icon() const;

    // Getters
    const QString id() const;
    AttachmentType type() const;
    const QString albumId() const;
    const QString name() const;
    const QString objectUrl() const;
    const QString image() const;
    const QString duration() const;

    // Setters
    void setId(const QString& v);
    void setType(AttachmentType v);
    void setName(const QString& v);
    void setAlbumId(const QString& v);
    void setObjectUrl(const QString& v);
    void setDuration(const QString& v);
};

Q_DECLARE_METATYPE(QAttachment)

#endif // QATTACHMENT_H
