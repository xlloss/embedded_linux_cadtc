dnl $Id: config0.m4 291371 2009-11-28 00:38:05Z jani $
dnl config.m4 for date extension

sinclude(ext/date/lib/timelib.m4)
sinclude(lib/timelib.m4)

PHP_DATE_CFLAGS="-I@ext_builddir@/lib"
timelib_sources="lib/astro.c lib/dow.c lib/parse_date.c lib/parse_tz.c 
                 lib/timelib.c lib/tm2unixtime.c lib/unixtime2tm.c"

PHP_NEW_EXTENSION(date, php_date.c $timelib_sources, no,, $PHP_DATE_CFLAGS)

PHP_ADD_BUILD_DIR([$ext_builddir/lib], 1)
PHP_ADD_INCLUDE([$ext_builddir/lib])
PHP_ADD_INCLUDE([$ext_srcdir/lib])

PHP_INSTALL_HEADERS([ext/date], [php_date.h lib/timelib.h lib/timelib_structs.h lib/timelib_config.h])

AC_CHECK_FUNCS([llabs])

cat > $ext_builddir/lib/timelib_config.h <<EOF
#ifdef PHP_WIN32
# include "config.w32.h"
#else
# include <php_config.h>
#endif
EOF
