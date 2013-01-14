#ifndef PROGRAMSETTINGS_H
#define PROGRAMSETTINGS_H

#include <QObject>
#include <manager/servicemanager.h>
#include "musson_global.h"

class musson_EXPORT ProgramSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ServiceManager* sm READ sm WRITE setServiceManager)
    Q_PROPERTY(bool lockOrientation READ lockOrientation WRITE setLockOrientation NOTIFY lockOrientationChanged)
    Q_PROPERTY(bool lightTheme READ lightTheme WRITE setLightTheme NOTIFY lightThemeChanged)
    Q_PROPERTY(bool hideStatusBar READ hideStatusBar WRITE setHideStatusBar NOTIFY hideStatusBarChanged)
public:
    explicit ProgramSettings(QObject *parent = 0);
    ServiceManager* sm();
    void setServiceManager(ServiceManager* sm);

    bool lockOrientation() const;
    void setLockOrientation(bool locked);

    bool lightTheme() const;
    void setLightTheme(bool light);

    bool hideStatusBar() const;
    void setHideStatusBar(bool hide);

signals:
    void lockOrientationChanged();
    void lightThemeChanged();
    void hideStatusBarChanged();

public slots:
    void clearCache();

private:
    ServiceManager* m_sm;
    bool m_lockOrientation;
    bool m_lightTheme;
    bool m_hideStatusBar;
};

#endif // PROGRAMSETTINGS_H
