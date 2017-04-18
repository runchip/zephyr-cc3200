#ifndef __ERRNO_COMPAT__
#define __ERRNO_COMPAT__

#include <errno.h>

#ifdef EBADF
#undef EBADF
#endif

#ifdef EAGAIN
#undef EAGAIN
#endif

#ifdef ENOMEM
#undef ENOMEM
#endif

#ifdef EACCES
#undef EACCES
#endif

#ifdef EFAULT
#undef EFAULT
#endif

#ifdef EINVAL
#undef EINVAL
#endif

#ifdef EDESTADDRREQ
#undef EDESTADDRREQ
#endif

#ifdef EPROTOTYPE
#undef EPROTOTYPE
#endif

#ifdef ENOPROTOOPT
#undef ENOPROTOOPT
#endif

#ifdef EPROTONOSUPPORT
#undef EPROTONOSUPPORT
#endif

#ifdef ESOCKTNOSUPPORT
#undef ESOCKTNOSUPPORT
#endif

#ifdef EOPNOTSUPP
#undef EOPNOTSUPP
#endif

#ifdef EAFNOSUPPORT
#undef EAFNOSUPPORT
#endif

#ifdef EADDRINUSE
#undef EADDRINUSE
#endif

#ifdef EADDRNOTAVAIL
#undef EADDRNOTAVAIL
#endif

#ifdef ENETUNREACH
#undef ENETUNREACH
#endif

#ifdef ENOBUFS
#undef ENOBUFS
#endif

#ifdef EISCONN
#undef EISCONN
#endif

#ifdef ENOTCONN
#undef ENOTCONN
#endif

#ifdef ETIMEDOUT
#undef ETIMEDOUT
#endif

#ifdef ECONNREFUSED
#undef ECONNREFUSED
#endif

#ifdef EWOULDBLOCK
#undef EWOULDBLOCK
#endif

#endif // __ERRNO_COMPAT__
