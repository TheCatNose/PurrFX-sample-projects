#ifndef DSTRING_H
#define DSTRING_H

#ifdef Q_OS_WINDOWS
#define QPATHSTR(x) x.toStdWString()
#else
#define QPATHSTR(x) x.toStdString()
#endif

#endif
