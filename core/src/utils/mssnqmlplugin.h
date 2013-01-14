#include <QtDeclarative/QDeclarativeExtensionPlugin>

class MssnQmlPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT
public:
    void registerTypes(const char *uri);
};
