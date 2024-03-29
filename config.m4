PHP_ARG_ENABLE(vehicles,
    [Whether to enable the "vehicles" extension],
    [  --enable-vehicles      Enable "vehicles" extension support])

PHP_ADD_INCLUDE(/usr/include/cairo)
PHP_ADD_INCLUDE(/usr/include/pango)

if test $PHP_VEHICLES != "no"; then
    PHP_REQUIRE_CXX()
    PHP_SUBST(VEHICLES_SHARED_LIBADD)
    PHP_ADD_LIBRARY(stdc++, 1, VEHICLES_SHARED_LIBADD)
    PHP_ADD_LIBRARY(cairo,1, VEHICLES_SHARED_LIBADD)
    PHP_ADD_LIBRARY(pango,1, VEHICLES_SHARED_LIBADD)
    PHP_NEW_EXTENSION(vehicles, vehicles.cc car.cc, $ext_shared)
fi

