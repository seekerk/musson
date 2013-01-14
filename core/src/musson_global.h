#ifndef MUSSON_GLOBAL_H
#define MUSSON_GLOBAL_H

#include <QtCore/qglobal.h>

//Q_INIT_RESOURCE_EXTERN(res);

#if defined(musson_LIBRARY)
#  define musson_EXPORT Q_DECL_EXPORT
#else
#  define musson_EXPORT Q_DECL_IMPORT
#endif

#endif  /* MUSSON_GLOBAL_H */

