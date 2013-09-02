#ifndef QTN_QTINUUM_NAMESPACE_H
#define QTN_QTINUUM_NAMESPACE_H

// declare namespace
namespace Qtinuum {}

#ifdef DISABLE_QTINUUM_NAMESPACE
using namespace Qtinuum;
#endif // DISABLE_QTINUUM_NAMESPACE

#ifndef NO_QTINUUM_SHORT
// short namespace name
namespace qtn = Qtinuum;
#endif // NO_QTINUUM_SHORT

#ifdef QTINUUM_NAMESPACE_ALIAS
// alias for Qtinuum namespace
namespace QTINUUM_NAMESPACE_ALIAS = Qtinuum;
#endif // QTINUUM_NAMESPACE_ALIAS

#endif // QTN_QTINUUM_NAMESPACE_H
