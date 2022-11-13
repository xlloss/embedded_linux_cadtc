/*                                                                -*- C -*-
   +----------------------------------------------------------------------+
   | PHP Version 5                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2007 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Stig Sæther Bakken <ssb@php.net>                             |
   +----------------------------------------------------------------------+
*/

/* $Id: build-defs.h.in 292156 2009-12-15 11:17:47Z jani $ */

#define CONFIGURE_COMMAND " './configure'  '--host=arm-linux' '--target=arm' '--prefix=./install-arm' '--disable-short-tags' '--without-mysql' '--without-pear' '--disable-all' '--disable-short-tags' '--with-thttpd=../thttpd-2.21b'"
#define PHP_ADA_INCLUDE		""
#define PHP_ADA_LFLAGS		""
#define PHP_ADA_LIBS		""
#define PHP_APACHE_INCLUDE	""
#define PHP_APACHE_TARGET	""
#define PHP_FHTTPD_INCLUDE      ""
#define PHP_FHTTPD_LIB          ""
#define PHP_FHTTPD_TARGET       ""
#define PHP_CFLAGS		"$(CFLAGS_CLEAN) -prefer-non-pic -static"
#define PHP_DBASE_LIB		""
#define PHP_BUILD_DEBUG		""
#define PHP_GDBM_INCLUDE	""
#define PHP_IBASE_INCLUDE	""
#define PHP_IBASE_LFLAGS	""
#define PHP_IBASE_LIBS		""
#define PHP_IFX_INCLUDE		""
#define PHP_IFX_LFLAGS		""
#define PHP_IFX_LIBS		""
#define PHP_INSTALL_IT		"    echo 'PHP_LIBS = -L. -lphp5 $(PHP_LIBS) $(EXTRA_LIBS)' > /home/slash/work/tiny4412_src/tiny4412/experiment/day4/3-application-exerise/webserver/thttpd-2.21b/php_makefile;     echo 'PHP_LDFLAGS = $(NATIVE_RPATHS) $(PHP_LDFLAGS)' >> /home/slash/work/tiny4412_src/tiny4412/experiment/day4/3-application-exerise/webserver/thttpd-2.21b/php_makefile;     echo 'PHP_CFLAGS = $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(CPPFLAGS) $(EXTRA_CFLAGS)' >> /home/slash/work/tiny4412_src/tiny4412/experiment/day4/3-application-exerise/webserver/thttpd-2.21b/php_makefile;     rm -f /home/slash/work/tiny4412_src/tiny4412/experiment/day4/3-application-exerise/webserver/thttpd-2.21b/php_thttpd.c /home/slash/work/tiny4412_src/tiny4412/experiment/day4/3-application-exerise/webserver/thttpd-2.21b/php_thttpd.h /home/slash/work/tiny4412_src/tiny4412/experiment/day4/3-application-exerise/webserver/thttpd-2.21b/libphp5.a;     $(LN_S) /home/slash/work/tiny4412_src/tiny4412/experiment/day4/3-application-exerise/webserver/php-5.2.12/sapi/thttpd/thttpd.c /home/slash/work/tiny4412_src/tiny4412/experiment/day4/3-application-exerise/webserver/thttpd-2.21b/php_thttpd.c;     $(LN_S) /home/slash/work/tiny4412_src/tiny4412/experiment/day4/3-application-exerise/webserver/php-5.2.12/sapi/thttpd/php_thttpd.h /home/slash/work/tiny4412_src/tiny4412/experiment/day4/3-application-exerise/webserver/php-5.2.12/libs/libphp5.a /home/slash/work/tiny4412_src/tiny4412/experiment/day4/3-application-exerise/webserver/thttpd-2.21b/;    test -f /home/slash/work/tiny4412_src/tiny4412/experiment/day4/3-application-exerise/webserver/thttpd-2.21b/php_patched ||     (cd /home/slash/work/tiny4412_src/tiny4412/experiment/day4/3-application-exerise/webserver/thttpd-2.21b && patch -p1 < /home/slash/work/tiny4412_src/tiny4412/experiment/day4/3-application-exerise/webserver/php-5.2.12/sapi/thttpd/thttpd_patch && touch php_patched)"
#define PHP_IODBC_INCLUDE	""
#define PHP_IODBC_LFLAGS	""
#define PHP_IODBC_LIBS		""
#define PHP_MSQL_INCLUDE	""
#define PHP_MSQL_LFLAGS		""
#define PHP_MSQL_LIBS		""
#define PHP_MYSQL_INCLUDE	""
#define PHP_MYSQL_LIBS		""
#define PHP_MYSQL_TYPE		""
#define PHP_ODBC_INCLUDE	""
#define PHP_ODBC_LFLAGS		""
#define PHP_ODBC_LIBS		""
#define PHP_ODBC_TYPE		""
#define PHP_OCI8_SHARED_LIBADD 	""
#define PHP_OCI8_DIR			""
#define PHP_OCI8_VERSION		""
#define PHP_ORACLE_SHARED_LIBADD 	"@ORACLE_SHARED_LIBADD@"
#define PHP_ORACLE_DIR				"@ORACLE_DIR@"
#define PHP_ORACLE_VERSION			"@ORACLE_VERSION@"
#define PHP_PGSQL_INCLUDE	""
#define PHP_PGSQL_LFLAGS	""
#define PHP_PGSQL_LIBS		""
#define PHP_PROG_SENDMAIL	"/usr/sbin/sendmail"
#define PHP_SOLID_INCLUDE	""
#define PHP_SOLID_LIBS		""
#define PHP_EMPRESS_INCLUDE	""
#define PHP_EMPRESS_LIBS	""
#define PHP_SYBASE_INCLUDE	""
#define PHP_SYBASE_LFLAGS	""
#define PHP_SYBASE_LIBS		""
#define PHP_DBM_TYPE		""
#define PHP_DBM_LIB		""
#define PHP_LDAP_LFLAGS		""
#define PHP_LDAP_INCLUDE	""
#define PHP_LDAP_LIBS		""
#define PHP_BIRDSTEP_INCLUDE     ""
#define PHP_BIRDSTEP_LIBS        ""
#define PEAR_INSTALLDIR         ""
#define PHP_INCLUDE_PATH	".:"
#define PHP_EXTENSION_DIR       "./install-arm/lib/php/extensions/no-debug-non-zts-20060613"
#define PHP_PREFIX              "./install-arm"
#define PHP_BINDIR              "./install-arm/bin"
#define PHP_SBINDIR             "./install-arm/sbin"
#define PHP_LIBDIR              "./install-arm/lib/php"
#define PHP_DATADIR             "./install-arm/share/php"
#define PHP_SYSCONFDIR          "./install-arm/etc"
#define PHP_LOCALSTATEDIR       "./install-arm/var"
#define PHP_CONFIG_FILE_PATH    "./install-arm/lib"
#define PHP_CONFIG_FILE_SCAN_DIR    ""
#define PHP_SHLIB_SUFFIX        "so"
