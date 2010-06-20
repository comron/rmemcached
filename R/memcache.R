cache.connect <- function(host, port) {
  .Call("memcache_connect", host, as.integer(port))
}

#.Call("sha1_object", bytes, skip)
cache.set <- function(conn, key, value) {
  v <- rawToChar(serialize(value, connection=NULL, ascii=TRUE))
  .Call("memcache_set", conn, key, v)
}

cache.get <- function(conn, key) {
  v <- .Call("memcache_get", conn, key)
  if(!is.null(v)) unserialize(charToRaw(v))
  else v
}
