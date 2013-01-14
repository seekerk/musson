INSTALL_PATH=/tmp/kulakov

QT_DEBUG='ENABLE_DEBUG=true'
#QT_DEBUG=

#prepare to install all packages
export PKG_CONFIG_PATH=$INSTALL_PATH/lib/pkgconfig:$PKG_CONFIG_PATH
export LD_LIBRARY_PATH=$INSTALL_PATH/lib:$LD_LIBRARY_PATH
cd ../../../

# install plugins
cd plugins
make distclean
echo qmake PREFIX=$INSTALL_PATH $QT_DEBUG
qmake PREFIX=$INSTALL_PATH $QT_DEBUG
make || exit
make install
cd ..

# install webauth-lib
cd plugins/webauth-lib
make distclean
echo qmake PREFIX=$INSTALL_PATH $QT_DEBUG
qmake PREFIX=$INSTALL_PATH $QT_DEBUG
make || exit
make install
cd ../..

# install core
cd core
make distclean
echo qmake PREFIX=$INSTALL_PATH $QT_DEBUG
qmake PREFIX=$INSTALL_PATH $QT_DEBUG
make || exit
make install
cd ..

# install components
cd components
make distclean
echo qmake PREFIX=$INSTALL_PATH $QT_DEBUG
qmake PREFIX=$INSTALL_PATH $QT_DEBUG
make || exit
make install
cd ..

# install application
cd applications/gallery
make distclean
echo qmake PREFIX=$INSTALL_PATH $QT_DEBUG
qmake PREFIX=$INSTALL_PATH $QT_DEBUG
make || exit
make install
cd ../..
