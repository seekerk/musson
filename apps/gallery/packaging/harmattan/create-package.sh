INSTALL_PATH=/tmp/musson-gallery

#QT_DEBUG='ENABLE_DEBUG=true'
QT_DEBUG=

#LIB_DEBUG='--enable-debug=yes'
LIB_DEBUG=

#prepare to install all packages
export PKG_CONFIG_PATH=$INSTALL_PATH/lib/pkgconfig:$PKG_CONFIG_PATH
export LD_LIBRARY_PATH=$INSTALL_PATH/lib:$LD_LIBRARY_PATH
rm -rf $INSTALL_PATH
cd ../../../../

# install plugins
cd plugins
#make distclean
echo qmake PREFIX=$INSTALL_PATH $QT_DEBUG DRIVERSDIR=$INSTALL_PATH/drivers
qmake PREFIX=$INSTALL_PATH $QT_DEBUG DRIVERSDIR=$INSTALL_PATH/drivers
make || exit
make install || exit
cd ..

# install webauth-lib
cd plugins/webauth-lib
#make distclean
echo qmake PREFIX=$INSTALL_PATH $QT_DEBUG
qmake PREFIX=$INSTALL_PATH $QT_DEBUG DEFINES+=QML_TO_RESOURCES
make || exit
make install || exit
cd ../..

# install core
cd core
#make distclean
echo qmake PREFIX=$INSTALL_PATH $QT_DEBUG
qmake PREFIX=$INSTALL_PATH $QT_DEBUG TRANSLATIONS_PATH=$INSTALL_PATH/translations DRIVERSDIR=$INSTALL_PATH/drivers
make || exit
make install
cd ..

# install components
cd components
make distclean
echo qmake PREFIX=$INSTALL_PATH $QT_DEBUG
qmake PREFIX=$INSTALL_PATH $QT_DEBUG TRANSLATIONS_PATH=$INSTALL_PATH/translations
make || exit
make install
cd ..

# install application
cd applications/gallery
make distclean
echo qmake PREFIX=$INSTALL_PATH $QT_DEBUG DEFINES+=QML_TO_RESOURCES
qmake PREFIX=$INSTALL_PATH $QT_DEBUG DEFINES+=QML_TO_RESOURCES
make || exit
make install
cd ../..

exit

#package all
cd applications/gallery/packaging/harmattan
dpkg-buildpackage -rfakeroot
