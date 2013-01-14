#ifndef ACCOUNTITEM_H
#define ACCOUNTITEM_H

/**
  \file accountitem.h
  \brief Account delegate for ListModel.

  Delegate for Account datatype from musson-core library.

  \author Musson team

  \date 2011-10-03
  */

#include "datatypes/listmodel.h"
#include <datatypes/account.h>
#include <datatypes/driverinfo.h>
#include <QIcon>
#include "musson_global.h"

/*!
 \brief A class for presenting account type from musson-core.
 */
class musson_EXPORT AccountItem : public ListItem
{
    Q_OBJECT
public:
    /*!
     \brief Empty constructor.

     \param parent object
     */
    AccountItem(QObject *parent = 0) : ListItem(parent) {};

    /*!
     \brief Creates an instance of AccountItem class based on Account.

     \param instance of Account class
     \param parent object
     */
    explicit AccountItem(Account account, QObject *parent = 0);
    ~AccountItem();

    /*!
      \brief Get data from delegate based on defined role.

      \param data role
      \return data based on role
     */
    QVariant data(int role) const;

    /*!
      \brief Get hashtable of role names.

      \return hashtable of role names
     */
    QHash<int, QByteArray> roleNames() const;

    /*!
      \brief Get name of account.

      \return name of account
     */
    const QString& name() const;

    /*!
      \brief Get avatar of account.

      \return avatar of account
     */
    const QString& icon() const;

    /*!
      \brief Get service icon for account.

      \return service icon
     */
    const QIcon& driverIcon() const;

    /*!
      \brief Get status of account.

      \return true if account is disabled, else false
     */
    const bool& isDisabled() const;
    const bool& isNetworkEnabled() const;

    /*!
      \brief Get instance of account, which is base for delegate.

      \return variant containing instance of account
     */
    const QVariant& account() const;

    /*!
      \brief Get key value.

      It is using for sorting AccountItem.

      \return variant with key value.
     */
    inline QVariant key() const { return m_name; };

    /*!
      \brief Get id of AccountItem.

      \return id of AccountItem
     */
    QString id() const;

    /*!
      \brief describes role names.
     */
    enum Roles {
        NameRole = Qt::UserRole + 1, /**< \brief Name of account. */
        IconRole, /**< \brief Avatar of account. */
        DriverIconRole, /**< \brief Service icon for account. */
        DataRole, /**< \brief Instance of Account class for account. */
        StatusRole, /**< \brief Status (enabled, disabled) of account. */
        NetworkRole,
        IDRole /**< \brief Id of delegate. */
    };

private:
    QVariant m_account;
    QString m_icon;
    QIcon m_driverIcon;
    QString m_name;
    bool m_isNetworkEnabled;
    bool m_isDisabled;
};

#endif // ACCOUNTITEM_H
