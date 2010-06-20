cache.connect <- function(host, port) {
  .Call("memcache_connect", host, as.integer(port))
}

#.Call("sha1_object", bytes, skip)
cache.set <- function(conn, key, value) {
  .Call("memcache_set", conn, key, value)
}

cache.get <- function(conn, key) {
  .Call("memcache_get", conn, key)
}
