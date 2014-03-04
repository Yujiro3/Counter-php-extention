dnl $Id$
dnl config.m4 for extension counter

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(counter, for counter support,
dnl Make sure that the comment is aligned:
dnl [  --with-counter             Include counter support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(counter, whether to enable counter support,
dnl Make sure that the comment is aligned:
[  --enable-counter           Enable counter support])

if test "$PHP_COUNTER" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-counter -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/counter.h"  # you most likely want to change this
  dnl if test -r $PHP_COUNTER/$SEARCH_FOR; then # path given as parameter
  dnl   COUNTER_DIR=$PHP_COUNTER
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for counter files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       COUNTER_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$COUNTER_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the counter distribution])
  dnl fi

  dnl # --with-counter -> add include path
  dnl PHP_ADD_INCLUDE($COUNTER_DIR/include)

  dnl # --with-counter -> check for lib and symbol presence
  dnl LIBNAME=counter # you may want to change this
  dnl LIBSYMBOL=counter # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $COUNTER_DIR/lib, COUNTER_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_COUNTERLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong counter lib version or lib not found])
  dnl ],[
  dnl   -L$COUNTER_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(COUNTER_SHARED_LIBADD)

  PHP_NEW_EXTENSION(counter, counter.c, $ext_shared)
fi
