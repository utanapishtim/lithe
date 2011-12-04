# Add the m4 directory to the include path for aclocal
ACLOCAL_AMFLAGS=-I m4

# Set up global variables
LIBNAME = lithe
SRCDIR = $(srcdir)/@SRCDIR@

LIB_CFILES = \
  @SRCDIR@/fatal.c         \
  @SRCDIR@/lithe_barrier.c \
  @SRCDIR@/lithe.c         \
  @SRCDIR@/lithe_mutex.c

LIB_CXXFILES = \
  @SRCDIR@/sched.cc

LIB_HFILES = \
  @SRCDIR@/context.h       \
  @SRCDIR@/defaults.h      \
  @SRCDIR@/deque.h         \
  @SRCDIR@/fatal.h         \
  @SRCDIR@/hart.h          \
  @SRCDIR@/lithe_barrier.h \
  @SRCDIR@/lithe_mutex.h   \
  @SRCDIR@/lithe.h         \
  @SRCDIR@/sched.h

LIB_HHFILES = \
  @SRCDIR@/lithe.hh \
  @SRCDIR@/sched.hh

TEST_EXECS = \
  test_mutex        \
  test_parent       \
  test_scheduler    \
  test_mutex_cc     \
  test_parent_cc    \
  test_scheduler_cc

# Setup parameters to build the library
lib_LTLIBRARIES = liblithe.la
liblithe_la_CFLAGS = $(AM_CFLAGS)
liblithe_la_CFLAGS += 
liblithe_la_CXXFLAGS = $(AM_CXXFLAGS)
liblithe_la_SOURCES = $(LIB_CFILES) $(LIB_HFILES)
liblithe_la_SOURCES += $(LIB_CXXFILES) $(LIB_HHFILES)
liblithe_la_LIBADD = -lparlib

# Setup a directory where all of the include files will be installed
litheincdir = $(includedir)/$(LIBNAME)
dist_litheinc_DATA = $(LIB_HFILES) $(LIB_HHFILES)

# Setup parameters to build the test programs
check_PROGRAMS = $(TEST_EXECS)

test_mutex_SOURCES = @TESTSDIR@/test-mutex.c
test_mutex_CFLAGS = $(AM_CFLAGS)
test_mutex_CFLAGS += -I$(SRCDIR)
test_mutex_LDADD = liblithe.la

test_parent_SOURCES = @TESTSDIR@/test-parent.c
test_parent_CFLAGS = $(AM_CFLAGS)
test_parent_CFLAGS += -I$(SRCDIR)
test_parent_LDADD = liblithe.la

test_scheduler_SOURCES = @TESTSDIR@/test-scheduler.c
test_scheduler_CFLAGS = $(AM_CFLAGS)
test_scheduler_CFLAGS += -I$(SRCDIR)
test_scheduler_LDADD = liblithe.la

test_mutex_cc_SOURCES = @TESTSDIR@/test-mutex.cc
test_mutex_cc_CXXFLAGS = $(AM_CXXFLAGS)
test_mutex_cc_CXXFLAGS += -I$(SRCDIR)
test_mutex_cc_LDADD = liblithe.la

test_parent_cc_SOURCES = @TESTSDIR@/test-parent.cc
test_parent_cc_CXXFLAGS = $(AM_CXXFLAGS)
test_parent_cc_CXXFLAGS += -I$(SRCDIR)
test_parent_cc_LDADD = liblithe.la

test_scheduler_cc_SOURCES = @TESTSDIR@/test-scheduler.cc
test_scheduler_cc_CXXFLAGS = $(AM_CXXFLAGS)
test_scheduler_cc_CXXFLAGS += -I$(SRCDIR)
test_scheduler_cc_LDADD = liblithe.la

ChangeLog:
	cd $(srcdir); \
	./git-changelog.sh

uninstall-hook:
	rm -rf $(litheincdir)

.PHONY: ChangeLog
