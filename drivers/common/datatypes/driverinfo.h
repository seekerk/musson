#ifndef DRIVERINFO_H
#define DRIVERINFO_H

#include <QObject>
#include <QMetaType>

/**
  \brief Class for store service information

  This structure store driver information: display name, driver name,
  service icon and path to library.
  */
class DriverInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString serviceName READ serviceName WRITE setServiceName)
    Q_PROPERTY(QString icon READ icon WRITE setIcon)
    Q_PROPERTY(QString library READ library WRITE setLibrary)

private:
    QString mName;
    QString mServiceName;
    QString mIcon;
    QString mLibrary;

public:
    /**
      \brief An constructor.

      \param parent The pointer to parent object.
      */
    explicit DriverInfo(QObject *parent = 0);

    /**
      \brief Copy constructor.

      \param src The DriverInfo instance.
      */
    DriverInfo(const DriverInfo &src);

    bool isEmpty() const;

    /**
      \brief Service dispay name.

      Contains origin name of service which can be displayed.
      */
    QString name() const;

    void setName(const QString &v);

    QString serviceName() const;

    void setServiceName(const QString &v);

    /**
      \brief Service icon.

      Contains origin icon of service.
      */
    QString icon() const;

    void setIcon(const QString &v);

    /**
      \brief Path to library.

      Contains path to library driver.
      */
    QString library() const;

    void setLibrary(const QString &v);

    /*!
      \brief Overloads assignment operator for DriverInfo classes.

      \param DriverInfo source DriverInfo instance
    */
    DriverInfo& operator=(const DriverInfo&);
};

Q_DECLARE_METATYPE(DriverInfo)

#endif // DRIVERINFO_H
