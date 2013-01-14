#ifndef _INTERFACE_H_
#define _INTERFACE_H_

class QString;
class DriverInfo;

namespace Driver {
    enum Action {
        checkConnection,
        authorize
    };
}

class DriverInterface {
public:

    virtual ~DriverInterface() {}

    // driver name
    virtual QString getDriverName() = 0;

    // display name
    virtual QString getDriverDisplayName() = 0;

    // driver icon
    virtual QString getDriverIcon() = 0;

    // driver initialization
    virtual void init() = 0;

    // check connection
    virtual void checkConnection() = 0;

    virtual DriverInfo getInfo() = 0;

};

Q_DECLARE_INTERFACE(DriverInterface, "org.fruct.oss.musson.driverinterface/0.1")

#endif /* _INTERFACE_H_ */
