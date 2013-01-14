#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include <QSharedDataPointer>
#include <QMetaType>

#include "utils/qrefreshlist.h"
#include "musson_global.h"

class BaseObjectPrivate;

class musson_EXPORT BaseObject
{
private:
    QSharedDataPointer<BaseObjectPrivate> d;

public:
    /**
      \brief Constructs an empty object.
      */
    BaseObject();

    /**
      \brief Constructs a copy of object.
      */
    BaseObject(const BaseObject &other);

    /**
      \brief A Destructor.
      */
    ~BaseObject();

    /**
      \brief Function checks that object is empty or not.

      \return True if object is empty (nothing changed after creation), otherwise - false.
      */
    bool isEmpty() const;

    /**
      \brief Assigment operator. Copies internal data.

      \param other The source object.

      \return This object.
      */
    BaseObject& operator=(const BaseObject& other);

    /**
      \brief Generates file name based on path and \a iconUrl.

      \param path The path to folder.

      \return Path with file name.
      */
    const QString generateFileName(const QString& path) const;

    /**
      \brief Generates file name based on path and link to object.

      \param path The path to folder.

      \param obj Link to object.

      \return Path with file name.
      */
    const QString generateFileName(const QString &path, const QString &obj) const;

    const QString title() const {return objectId();};

    inline bool operator<(const BaseObject &other) const {return title() < other.title();}

    //Getters
    const QString serviceId() const;
    const QString accountId() const;
    const QString ownerId() const;
    const QString objectId() const;
    const QString iconUrl() const;
    bool isPrivate() const;

    //Setters
    void setServiceId(const QString &v);
    void setAccountId(const QString &v);
    void setOwnerId(const QString &v);
    void setObjectId(const QString &v);
    void setIconUrl(const QString &v);
    void setPivate(const bool v = true);

};

/*! \typedef BaseObjectList
 List of BaseObject items.
 \sa QRefreshList
*/
typedef QRefreshList<BaseObject> BaseObjectList;

Q_DECLARE_METATYPE(BaseObject)
Q_DECLARE_METATYPE(BaseObjectList)

#endif // BASEOBJECT_H
