dnl aclocal.m4 generated automatically by aclocal 1.4

dnl Copyright (C) 1994, 1995-8, 1999 Free Software Foundation, Inc.
dnl This file is free software; the Free Software Foundation
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.

dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY, to the extent permitted by law; without
dnl even the implied warranty of MERCHANTABILITY or FITNESS FOR A
dnl PARTICULAR PURPOSE.

AH_TEMPLATE([socklen_t], [Length type used by getsockopt])

# Try to detect the type of the third arg to getsockname() et al
AC_DEFUN([AC_TYPE_SOCKLEN_T],
[AC_CACHE_CHECK([for socklen_t in sys/socket.h], ac_cv_type_socklen_t,
[AC_EGREP_HEADER(socklen_t, sys/socket.h,
  [ac_cv_type_socklen_t=yes], [ac_cv_type_socklen_t=no])])
if test $ac_cv_type_socklen_t = no; then
        AC_MSG_CHECKING(for AIX)
        AC_EGREP_CPP(yes, [
#ifdef _AIX
 yes
#endif
],[
AC_MSG_RESULT(yes)
AC_DEFINE(socklen_t, size_t)
],[
AC_MSG_RESULT(no)
AC_DEFINE(socklen_t, int)
])
fi
])

# checks for gmp version 2 or later. 
# AC_CHECK_LIBGMP(library, [, if-found [, if-not-found]])
AC_DEFUN([AC_CHECK_LIBGMP],
[AC_CACHE_CHECK([for mpz_get_d in -l$1], ac_cv_lib_$1_mpz_get_d,
[ac_save_libs="$LIBS"
LIBS="-l$1 $LIBS"
AC_TRY_LINK(dnl
[#if HAVE_GMP_H
#include <gmp.h>
#elif HAVE_GMP2_GMP_H
#include <gmp2/gmp.h>
#endif
],
[mpz_get_d(0);],
ac_cv_lib_$1_mpz_get_d=yes,
ac_cv_lib_$1_mpz_get_d=no)
LIBS="$ac_save_LIBS"
])
if test x$ac_cv_lib_$1_mpz_get_d = xyes ; then
ifelse([$2], ,
[AC_DEFINE(HAVE_LIBGMP)
LIBS="-l$1 $LIBS"
], [$2])
ifelse([$3], , ,
[else
$3
])dnl
fi
])

# checks for gmp version 2 or later. 
# AC_SEARCH_LIBGMP(libraries, [, if-found [, if-not-found]])
AC_DEFUN([AC_SEARCH_LIBGMP],
[AC_CACHE_CHECK([for library containing mpz_get_d], ac_cv_search_mpz_get_d,
[ac_search_save_LIBS="$LIBS"
ac_cv_search_mpz_get_d="no"
for i in $1; do
LIBS="-l$i $ac_search_save_LIBS"
AC_TRY_LINK(dnl
[#if HAVE_GMP_H
#include <gmp.h>
#elif HAVE_GMP2_GMP_H
#include <gmp2/gmp.h>
#endif
],
[mpz_get_d(0);],
[ac_cv_search_mpz_get_d=-l$i
break
])
done
LIBS="$ac_search_save_LIBS"
])
if test "x$ac_cv_search_mpz_get_d" != xno ; then
  LIBS="$ac_cv_search_mpz_get_d $LIBS"
ifelse([$2], ,
[AC_DEFINE(HAVE_LIBGMP)
], [$2])
ifelse([$3], , ,
[else
$3
])dnl
fi
])

# LSH_PATH_ADD(path-id, directory)
AC_DEFUN([LSH_PATH_ADD],
[AC_MSG_CHECKING($2)
ac_exists=no
if test -d "$2/." ; then
  ac_real_dir=`cd $2 && pwd`
  if test -n "$ac_real_dir" ; then
    ac_exists=yes
    for old in $1_REAL_DIRS ; do
      ac_found=no
      if test x$ac_real_dir = x$old ; then
        ac_found=yes;
	break;
      fi
    done
    if test $ac_found = yes ; then
      AC_MSG_RESULT(already added)
    else
      AC_MSG_RESULT(added)
      # LDFLAGS="$LDFLAGS -L $2"
      $1_REAL_DIRS="$ac_real_dir [$]$1_REAL_DIRS"
      $1_DIRS="$2 [$]$1_DIRS"
    fi
  fi
fi
if test $ac_exists = no ; then
  AC_MSG_RESULT(not found)
fi
])

# LSH_RPATH_ADD(dir)
AC_DEFUN([LSH_RPATH_ADD], [LSH_PATH_ADD(RPATH_CANDIDATE, $1)])

# LSH_RPATH_INIT(candidates)
AC_DEFUN([LSH_RPATH_INIT],
[AC_MSG_CHECKING([for -R flag])
RPATHFLAG=''
case `uname -sr` in
  OSF1\ V4.*)
    RPATHFLAG="-rpath "
    ;;
  IRIX\ 6.*)
    RPATHFLAG="-rpath "
    ;;
  IRIX\ 5.*)
    RPATHFLAG="-rpath "
    ;;
  SunOS\ 5.*)
    if test "$TCC" = "yes"; then
      # tcc doesn't know about -R
      RPATHFLAG="-Wl,-R,"
    else
      RPATHFLAG=-R
    fi
    ;;
  Linux\ 2.*)
    RPATHFLAG="-Wl,-rpath,"
    ;;
  *)
    :
    ;;
esac

if test x$RPATHFLAG = x ; then
  AC_MSG_RESULT(none)
else
  AC_MSG_RESULT([using $RPATHFLAG])
fi

RPATH_CANDIDATE_REAL_DIRS=''
RPATH_CANDIDATE_DIRS=''

AC_MSG_RESULT([Searching for libraries])

for d in $1 ; do
  LSH_RPATH_ADD($d)
done
])    

# Try to execute a main program, and if it fails, try adding some
# -R flag.
# LSH_RPATH_FIX
AC_DEFUN([LSH_RPATH_FIX],
[if test $cross_compiling = no -a "x$RPATHFLAG" != x ; then
  ac_success=no
  AC_TRY_RUN([int main(int argc, char **argv) { return 0; }],
    ac_success=yes, ac_success=no, :)
  
  if test $ac_success = no ; then
    AC_MSG_CHECKING([Running simple test program failed. Trying -R flags])
dnl echo RPATH_CANDIDATE_DIRS = $RPATH_CANDIDATE_DIRS
    ac_remaining_dirs=''
    ac_rpath_save_LDFLAGS="$LDFLAGS"
    for d in $RPATH_CANDIDATE_DIRS ; do
      if test $ac_success = yes ; then
  	ac_remaining_dirs="$ac_remaining_dirs $d"
      else
  	LDFLAGS="$RPATHFLAG$d $LDFLAGS"
dnl echo LDFLAGS = $LDFLAGS
  	AC_TRY_RUN([int main(int argc, char **argv) { return 0; }],
  	  [ac_success=yes
  	  ac_rpath_save_LDFLAGS="$LDFLAGS"
  	  AC_MSG_RESULT([adding $RPATHFLAG$d])
  	  ],
  	  [ac_remaining_dirs="$ac_remaining_dirs $d"], :)
  	LDFLAGS="$ac_rpath_save_LDFLAGS"
      fi
    done
    RPATH_CANDIDATE_DIRS=$ac_remaining_dirs
  fi
  if test $ac_success = no ; then
    AC_MSG_RESULT(failed)
  fi
fi
])

# Like AC_CHECK_LIB, but uses $KRB_LIBS rather than $LIBS.
# AC_CHECK_KRB_LIB(LIBRARY, FUNCTION, [, ACTION-IF-FOUND [,
#                  ACTION-IF-NOT-FOUND [, OTHER-LIBRARIES]]])

AC_DEFUN([AC_CHECK_KRB_LIB],
[AC_CHECK_LIB([$1], [$2],
  ifelse([$3], ,
      [[ac_tr_lib=HAVE_LIB`echo $1 | sed -e 's/[^a-zA-Z0-9_]/_/g' \
     	    -e 'y/abcdefghijklmnopqrstuvwxyz/ABCDEFGHIJKLMNOPQRSTUVWXYZ/'`
        AC_DEFINE_UNQUOTED($ac_tr_lib)
        KRB_LIBS="-l$1 $KRB_LIBS"
      ]], [$3]),
  ifelse([$4], , , [$4
])dnl
, [$5 $KRB_LIBS])
])

# AC_LIB_ARGP(ACTION-IF-OK, ACTION-IF-BAD)
AC_DEFUN([AC_LIB_ARGP],
[ ac_argp_save_LIBS="$LIBS"
  ac_argp_save_LDFLAGS="$LDFLAGS"
  ac_argp_ok=no
  # First check if we can link with argp.
  AC_SEARCH_LIBS(argp_parse, argp,
  [ LSH_RPATH_FIX
    AC_CACHE_CHECK([for working argp],
      lsh_cv_lib_argp_works,
      [ AC_TRY_RUN(
[#include <argp.h>
#include <stdlib.h>

static const struct argp_option
options[] =
{
  { NULL, 0, NULL, 0, NULL, 0 }
};

struct child_state
{
  int n;
};

static error_t
child_parser(int key, char *arg, struct argp_state *state)
{
  struct child_state *input = (struct child_state *) state->input;
  
  switch(key)
    {
    default:
      return ARGP_ERR_UNKNOWN;
    case ARGP_KEY_END:
      if (!input->n)
	input->n = 1;
      break;
    }
  return 0;
}

const struct argp child_argp =
{
  options,
  child_parser,
  NULL, NULL, NULL, NULL, NULL
};

struct main_state
{
  struct child_state child;
  int m;
};

static error_t
main_parser(int key, char *arg, struct argp_state *state)
{
  struct main_state *input = (struct main_state *) state->input;

  switch(key)
    {
    default:
      return ARGP_ERR_UNKNOWN;
    case ARGP_KEY_INIT:
      state->child_inputs[0] = &input->child;
      break;
    case ARGP_KEY_END:
      if (!input->m)
	input->m = input->child.n;
      
      break;
    }
  return 0;
}

static const struct argp_child
main_children[] =
{
  { &child_argp, 0, "", 0 },
  { NULL, 0, NULL, 0}
};

static const struct argp
main_argp =
{ options, main_parser, 
  NULL,
  NULL,
  main_children,
  NULL, NULL
};

int main(int argc, char **argv)
{
  struct main_state input = { { 0 }, 0 };
  char *v[2] = { "foo", NULL };

  argp_parse(&main_argp, 1, v, 0, NULL, &input);

  if ( (input.m == 1) && (input.child.n == 1) )
    return 0;
  else
    return 1;
}
], lsh_cv_lib_argp_works=yes,
   lsh_cv_lib_argp_works=no,
   lsh_cv_lib_argp_works=no)])

  if test x$lsh_cv_lib_argp_works = xyes ; then
    ac_argp_ok=yes
  else
    # Reset link flags
    LIBS="$ac_argp_save_LIBS"
    LDFLAGS="$ac_argp_save_LDFLAGS"
  fi])

  if test x$ac_argp_ok = xyes ; then
    ifelse([$1],, true, [$1])
  else
    ifelse([$2],, true, [$2])
  fi   
])

# Do all the work for Automake.  This macro actually does too much --
# some checks are only needed if your package does certain things.
# But this isn't really a big deal.

# serial 1

dnl Usage:
dnl AM_INIT_AUTOMAKE(package,version, [no-define])

AC_DEFUN(AM_INIT_AUTOMAKE,
[AC_REQUIRE([AC_PROG_INSTALL])
PACKAGE=[$1]
AC_SUBST(PACKAGE)
VERSION=[$2]
AC_SUBST(VERSION)
dnl test to see if srcdir already configured
if test "`cd $srcdir && pwd`" != "`pwd`" && test -f $srcdir/config.status; then
  AC_MSG_ERROR([source directory already configured; run "make distclean" there first])
fi
ifelse([$3],,
AC_DEFINE_UNQUOTED(PACKAGE, "$PACKAGE", [Name of package])
AC_DEFINE_UNQUOTED(VERSION, "$VERSION", [Version number of package]))
AC_REQUIRE([AM_SANITY_CHECK])
AC_REQUIRE([AC_ARG_PROGRAM])
dnl FIXME This is truly gross.
missing_dir=`cd $ac_aux_dir && pwd`
AM_MISSING_PROG(ACLOCAL, aclocal, $missing_dir)
AM_MISSING_PROG(AUTOCONF, autoconf, $missing_dir)
AM_MISSING_PROG(AUTOMAKE, automake, $missing_dir)
AM_MISSING_PROG(AUTOHEADER, autoheader, $missing_dir)
AM_MISSING_PROG(MAKEINFO, makeinfo, $missing_dir)
AC_REQUIRE([AC_PROG_MAKE_SET])])

#
# Check to make sure that the build environment is sane.
#

AC_DEFUN(AM_SANITY_CHECK,
[AC_MSG_CHECKING([whether build environment is sane])
# Just in case
sleep 1
echo timestamp > conftestfile
# Do `set' in a subshell so we don't clobber the current shell's
# arguments.  Must try -L first in case configure is actually a
# symlink; some systems play weird games with the mod time of symlinks
# (eg FreeBSD returns the mod time of the symlink's containing
# directory).
if (
   set X `ls -Lt $srcdir/configure conftestfile 2> /dev/null`
   if test "[$]*" = "X"; then
      # -L didn't work.
      set X `ls -t $srcdir/configure conftestfile`
   fi
   if test "[$]*" != "X $srcdir/configure conftestfile" \
      && test "[$]*" != "X conftestfile $srcdir/configure"; then

      # If neither matched, then we have a broken ls.  This can happen
      # if, for instance, CONFIG_SHELL is bash and it inherits a
      # broken ls alias from the environment.  This has actually
      # happened.  Such a system could not be considered "sane".
      AC_MSG_ERROR([ls -t appears to fail.  Make sure there is not a broken
alias in your environment])
   fi

   test "[$]2" = conftestfile
   )
then
   # Ok.
   :
else
   AC_MSG_ERROR([newly created file is older than distributed files!
Check your system clock])
fi
rm -f conftest*
AC_MSG_RESULT(yes)])

dnl AM_MISSING_PROG(NAME, PROGRAM, DIRECTORY)
dnl The program must properly implement --version.
AC_DEFUN(AM_MISSING_PROG,
[AC_MSG_CHECKING(for working $2)
# Run test in a subshell; some versions of sh will print an error if
# an executable is not found, even if stderr is redirected.
# Redirect stdin to placate older versions of autoconf.  Sigh.
if ($2 --version) < /dev/null > /dev/null 2>&1; then
   $1=$2
   AC_MSG_RESULT(found)
else
   $1="$3/missing $2"
   AC_MSG_RESULT(missing)
fi
AC_SUBST($1)])

# Like AC_CONFIG_HEADER, but automatically create stamp file.

AC_DEFUN(AM_CONFIG_HEADER,
[AC_PREREQ([2.12])
AC_CONFIG_HEADER([$1])
dnl When config.status generates a header, we must update the stamp-h file.
dnl This file resides in the same directory as the config header
dnl that is generated.  We must strip everything past the first ":",
dnl and everything past the last "/".
AC_OUTPUT_COMMANDS(changequote(<<,>>)dnl
ifelse(patsubst(<<$1>>, <<[^ ]>>, <<>>), <<>>,
<<test -z "<<$>>CONFIG_HEADERS" || echo timestamp > patsubst(<<$1>>, <<^\([^:]*/\)?.*>>, <<\1>>)stamp-h<<>>dnl>>,
<<am_indx=1
for am_file in <<$1>>; do
  case " <<$>>CONFIG_HEADERS " in
  *" <<$>>am_file "*<<)>>
    echo timestamp > `echo <<$>>am_file | sed -e 's%:.*%%' -e 's%[^/]*$%%'`stamp-h$am_indx
    ;;
  esac
  am_indx=`expr "<<$>>am_indx" + 1`
done<<>>dnl>>)
changequote([,]))])


# serial 1

# @defmac AC_PROG_CC_STDC
# @maindex PROG_CC_STDC
# @ovindex CC
# If the C compiler in not in ANSI C mode by default, try to add an option
# to output variable @code{CC} to make it so.  This macro tries various
# options that select ANSI C on some system or another.  It considers the
# compiler to be in ANSI C mode if it handles function prototypes correctly.
#
# If you use this macro, you should check after calling it whether the C
# compiler has been set to accept ANSI C; if not, the shell variable
# @code{am_cv_prog_cc_stdc} is set to @samp{no}.  If you wrote your source
# code in ANSI C, you can make an un-ANSIfied copy of it by using the
# program @code{ansi2knr}, which comes with Ghostscript.
# @end defmac

AC_DEFUN(AM_PROG_CC_STDC,
[AC_REQUIRE([AC_PROG_CC])
AC_BEFORE([$0], [AC_C_INLINE])
AC_BEFORE([$0], [AC_C_CONST])
dnl Force this before AC_PROG_CPP.  Some cpp's, eg on HPUX, require
dnl a magic option to avoid problems with ANSI preprocessor commands
dnl like #elif.
dnl FIXME: can't do this because then AC_AIX won't work due to a
dnl circular dependency.
dnl AC_BEFORE([$0], [AC_PROG_CPP])
AC_MSG_CHECKING(for ${CC-cc} option to accept ANSI C)
AC_CACHE_VAL(am_cv_prog_cc_stdc,
[am_cv_prog_cc_stdc=no
ac_save_CC="$CC"
# Don't try gcc -ansi; that turns off useful extensions and
# breaks some systems' header files.
# AIX			-qlanglvl=ansi
# Ultrix and OSF/1	-std1
# HP-UX			-Aa -D_HPUX_SOURCE
# SVR4			-Xc -D__EXTENSIONS__
for ac_arg in "" -qlanglvl=ansi -std1 "-Aa -D_HPUX_SOURCE" "-Xc -D__EXTENSIONS__"
do
  CC="$ac_save_CC $ac_arg"
  AC_TRY_COMPILE(
[#include <stdarg.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
/* Most of the following tests are stolen from RCS 5.7's src/conf.sh.  */
struct buf { int x; };
FILE * (*rcsopen) (struct buf *, struct stat *, int);
static char *e (p, i)
     char **p;
     int i;
{
  return p[i];
}
static char *f (char * (*g) (char **, int), char **p, ...)
{
  char *s;
  va_list v;
  va_start (v,p);
  s = g (p, va_arg (v,int));
  va_end (v);
  return s;
}
int test (int i, double x);
struct s1 {int (*f) (int a);};
struct s2 {int (*f) (double a);};
int pairnames (int, char **, FILE *(*)(struct buf *, struct stat *, int), int, int);
int argc;
char **argv;
], [
return f (e, argv, 0) != argv[0]  ||  f (e, argv, 1) != argv[1];
],
[am_cv_prog_cc_stdc="$ac_arg"; break])
done
CC="$ac_save_CC"
])
if test -z "$am_cv_prog_cc_stdc"; then
  AC_MSG_RESULT([none needed])
else
  AC_MSG_RESULT($am_cv_prog_cc_stdc)
fi
case "x$am_cv_prog_cc_stdc" in
  x|xno) ;;
  *) CC="$CC $am_cv_prog_cc_stdc" ;;
esac
])

