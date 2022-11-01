#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.59])
AC_INIT(src/datafile.cpp)
AM_INIT_AUTOMAKE(PACKAGE, VERSION, [NO-DEFINE])

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX

AC_CONFIG_MACRO_DIR([m4])

CXXFLAGS="-ggdb3 -O3 -fpermissive -DNDEBUG -DANSITZ -Wno-deprecated"

AC_ARG_WITH(swe,[  --with-swe[=DIR]    Swiss ephemeris library location])
if test "x$with_swe" != "x" ; then
	CXXFLAGS="${CXXFLAGS} -I$with_swe"
	LIBS="${LIBS} -L$with_swe -lswe"
	AC_SUBST(ACE_HOME, [$with_swe])
fi

AC_CONFIG_FILES([Makefile 
				
                 ])
AC_OUTPUT
