# Rmemcached

## Installation

This is how I installed it, its ugly for now, but it works.

`R CMD INSTALL ./rmemcached`

You might need to provide the path to libmemcached like so:

`R CMD INSTALL ./rmemcached --configure-args=--with-libmemcached-path=/usr/local/`

## Examples
  library(rmemcached)
  server <- cache.connect("127.0.0.1", 11211)
  cache.set(server, "foo", "1000")
  cache.set(server, "bar", "2000")
  cache.get(server, "foo")
  cache.get(server, "baz")