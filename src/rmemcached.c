#include "rmemcached.h"

static void memcache_connection_finalizer(SEXP ptr) {
  if(!R_ExternalPtrAddr(ptr)) return;
  memcached_free(R_ExternalPtrAddr(ptr));
  R_ClearExternalPtr(ptr);
}

bool isMemcachedConnection(SEXP conn) {
  return (isExternalPointer(conn) && strcmp(CHAR(PRINTNAME(TAG(conn))), "Rmemcached_connection") == 0);
}

SEXP memcache_connect(SEXP host, SEXP port) {
  if(!isString(host)) error("'host' must be of type character");
  if(!isInteger(port)) error("'port' must be of type integer");

  SEXP memc_ptr;

  memcached_return_t rc;
  memcached_server_st *servers;
  memcached_st *memc = memcached_create(NULL);

  servers = memcached_server_list_append(NULL, CHAR(STRING_ELT(host, 0)), *INTEGER(port), &rc);
  rc = memcached_server_push(memc, servers);  
  memcached_server_free(servers);
  
  memc_ptr = R_MakeExternalPtr((void *)memc, install("Rmemcached_connection"), R_NilValue);

  PROTECT(memc_ptr);
  R_RegisterCFinalizerEx(memc_ptr, memcache_connection_finalizer, TRUE);
  UNPROTECT(1);
  
  return memc_ptr;
}

SEXP memcache_set(SEXP conn, SEXP key, SEXP value) {
  if(!isMemcachedConnection(conn)) error("'conn' must be a memcached connection");
  if(!isString(key)) error("'key' must be of type character");
  if(!isString(value)) error("'value' must be of type character");
  
  SEXP ans;
  memcached_return_t rc;
  memcached_st *memc = (memcached_st *)R_ExternalPtrAddr(conn);
  const char *c_key = CHAR(STRING_ELT(key, 0));
  const char *c_value = CHAR(STRING_ELT(value, 0));
  
  rc = memcached_set(memc, c_key, strlen(c_key), c_value, strlen(c_value), (time_t)0, (uint32_t)0);

  PROTECT(ans = allocVector(LGLSXP, 1));
  LOGICAL(ans)[0] = rc == MEMCACHED_SUCCESS ? TRUE : FALSE;;
  UNPROTECT(1);

  return ans;
}

SEXP memcache_get(SEXP conn, SEXP key) {
  if(!isMemcachedConnection(conn)) error("'conn' must be a memcached connection");
  if(!isString(key)) error("'key' must be of type character");
  
  SEXP ans;
  memcached_return_t rc;
  memcached_st *memc = (memcached_st *)R_ExternalPtrAddr(conn);
  const char* c_key = CHAR(STRING_ELT(key, 0));
  char* value = NULL;
  size_t value_length;
  uint32_t flags;

  value = memcached_get(memc, c_key, strlen(c_key), &value_length, &flags, &rc);

  if( value != NULL ) {
    PROTECT(ans = allocVector(STRSXP, 1));
    SET_STRING_ELT(ans, 0, mkChar(value));
    UNPROTECT(1);
    return ans;    
  } else {
    return R_NilValue;
  }
}