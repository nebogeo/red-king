AC_INIT(coevol-range, version-0.1)
AC_PROG_CC

AC_CHECK_LIB(m, cos)
AC_CHECK_LIB(portaudio, main)
AC_CHECK_LIB(sndfile, main, [], [AC_MSG_ERROR([sndfile was not found])])
AC_CHECK_LIB(jellyfish, main)

AC_PROG_CXX
AC_LANG(C++)
AC_CHECK_HEADERS(iostream)

AC_CONFIG_HEADER(src/config.h)
AC_OUTPUT(Makefile)
