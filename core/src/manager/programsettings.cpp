#include "programsettings.h"

ProgramSettings::ProgramSettings(QObject *parent) :
    QObject(parent)
{
}

void ProgramSettings::setServiceManager(ServiceManager *sm)
{
    m_sm = sm;
    m_lockOrientation = !m_sm->settings()->value(SETTINGS_AUTOROTATE).toBool();
    m_lightTheme = m_sm->settings()->value(SETTINGS_LIGHT_THEME).toBool();
    m_hideStatusBar = m_sm->settings()->value(SETTINGS_HIDE_SB).toBool();
}

ServiceManager* ProgramSettings::sm()
{
    return m_sm;
}

bool ProgramSettings::lockOrientation() const
{
    return m_lockOrientation;
}

void ProgramSettings::setLockOrientation(bool locked)
{
    if (m_lockOrientation != locked) {
        m_lockOrientation = locked;
        m_sm->settings()->setValue(SETTINGS_AUTOROTATE, !m_lockOrientation);
        emit lockOrientationChanged();
    }
}

bool ProgramSettings::lightTheme() const
{
    return m_lightTheme;
}

void ProgramSettings::setLightTheme(bool light)
{
    if (m_lightTheme != light) {
        m_lightTheme = light;
        m_sm->settings()->setValue(SETTINGS_LIGHT_THEME, m_lightTheme);
        emit lightThemeChanged();
    }
}

bool ProgramSettings::hideStatusBar() const
{
    return m_hideStatusBar;
}

void ProgramSettings::setHideStatusBar(bool hide)
{
    if (m_hideStatusBar != hide) {
        m_hideStatusBar = hide;
        m_sm->settings()->setValue(SETTINGS_HIDE_SB, m_hideStatusBar);
        emit hideStatusBarChanged();
    }
}

void ProgramSettings::clearCache()
{
    m_sm->clearCache();
}
