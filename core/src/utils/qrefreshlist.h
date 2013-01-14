#ifndef QREFRESHLIST_H
#define QREFRESHLIST_H

#include <QObject>
#include <QList>
#include <QDate>

/**
  \brief Extends QList with fixation time creation.
  */
template <class T> class QRefreshList : public QList<T>
{
private:
    QDateTime _timeCreate; /**< The list creation time. */
    bool _hasPrivate; /**< True if list or part of list has limited access and cann't be received from service, otherwise - false. */

public:
    /**
      \brief Default constructor.
      */
    QRefreshList() : _hasPrivate(false) {}

    /**
      \brief Destructor.
      */
    ~QRefreshList() {}

    /**
      \brief Get list time creation.

      \return List creation time.
      */
    QDateTime timeCreate() const {return this->_timeCreate;}

    /**
      \brief Sets list time creation.

      \param val New value of time creation.
      */
    void setTimeCreate(const QDateTime& val) {this->_timeCreate = val;}

    /**
      \brief Get privacy flag.

      \return True if list or part of this list has limited access and didn't received from service, otherwise - false.
      */
    bool hasPrivate() const {return this->_hasPrivate;}

    /**
      \brief Sets privacy flag.

      \param val True if list or part of this list has limited access and didn't received from service (default value), otherwise - false.
      */
    void setPrivate(const bool &val = true) {this->_hasPrivate = val;}

signals:

public slots:

};

#endif // QREFRESHLIST_H
