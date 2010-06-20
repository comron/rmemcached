#include <libmemcached/visibility.h>
#include <libmemcached/memcached.h>

#include <R.h>
#include <Rinternals.h>

// #define DEBUG

#ifdef DEBUG
  #define PRINT_DEBUG(format, args...) printf("DEBUG: "format, ##args);
#else
  #define PRINT_DEBUG(format, args...)
#endif

#define isExternalPointer(s)	(TYPEOF(s) == EXTPTRSXP)

static void memcache_connection_finalizer(SEXP ptr);
static void memcache_servers_finalizer(SEXP ptr);
SEXP memcache_connect(SEXP host, SEXP port);
