#include "networkmanager.h"

NetworkManager::NetworkManager(QObject *parent) :
    QObject(parent)
{
}

void NetworkManager::setType(int type)
{
    if (type != m_type) {
        m_type = type;
        m_sm->settings()->setValue(SETTINGS_PROXY, type);
        emit typeChanged();
    }
}

int NetworkManager::type() {
    return m_type;
}

void NetworkManager::setHost(const QString &host)
{
    if (host.compare(m_host) != 0) {
        m_host = host;
        m_sm->settings()->setValue(SETTINGS_PROXYHOST, host);
        emit hostChanged();
    }
}

const QString& NetworkManager::host() const
{
    return m_host;
}

void NetworkManager::setPort(const QString &port)
{
    if (port.compare(m_port) != 0) {
        m_port = port;
        m_sm->settings()->setValue(SETTINGS_PROXYPORT, port);
        emit portChanged();
    }
}

const QString& NetworkManager::port() const
{
    return m_port;
}

void NetworkManager::setServiceManager(ServiceManager *sm)
{
    m_sm = sm;
    m_type = (ProxyType::ProxyType)m_sm->settings()->value(SETTINGS_PROXY).toInt();
    m_host = m_sm->settings()->value(SETTINGS_PROXYHOST).toString();
    m_port = m_sm->settings()->value(SETTINGS_PROXYPORT).toString();
}

ServiceManager* NetworkManager::sm()
{
    return m_sm;
}
