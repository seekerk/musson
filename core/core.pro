TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS = src

OTHER_FILES += \
    debian/rules \
    debian/manifest.aegis \
    debian/copyright \
    debian/control \
    debian/compat \
    debian/changelog
