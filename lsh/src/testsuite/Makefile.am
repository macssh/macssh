# Testsuite

CFLAGS = @CFLAGS@ -O0

# Tests that can be used without any special setup.
TS_PROGS = arcfour-test des-test rijndael-test twofish-test \
           md5-test sha1-test rsa-test dsa-test spki-tag-test \
           string-test
TS_SH = conv-1-test conv-2-test export-1-test keygen-1-test keygen-2-test \
	write-key-1-test write-key-2-test

TS_SOME = $(TS_PROGS) $(TS_SH)

# Tests that require that lsh is set up for password-less login.
TS_MORE_SH = lsh-1-test lsh-2-test lsh-3-test lsh-4-test lsh-5-test \
	lsh-cat-test lshg-cat-test lshg-cat-2-test tcpip-local-test tcpip-remote-test \
	lshg-tcpip-local-test ssh1-fallback-test
TS_MORE_PROGS =

TS_ALL = $(TS_PROGS) $(TS_SH) $(TS_MORE_PROGS) $(TS_MORE_SH) 

# Don't use automakes testsuite support
# TESTS = $(TS_PROGS) $(TS_SH)

# SUFFIXES = .c .o
noinst_PROGRAMS = $(TS_PROGS)

LDADD = ../liblsh.a ../nettle/libnettle.a $(addprefix ../,@LIBARGP@)

EXTRA_DIST = macros.m4 $(SOURCES:.c=.m4) $(TS_SH) $(TS_MORE_SH) \
	functions.sh runtests key-1.private

all:
	@echo make check to build testprograms.

M4FLAGS = -P #-s
%.c: macros.m4 %.m4
	$(M4) $(M4FLAGS) $^ > $@T
	test -s $@T && mv -f $@T $@


.PHONY: check-some check-more

check: check-some

check-some: $(TS_SOME)
	srcdir=$(srcdir) $(srcdir)/runtests $(TS_SOME)

check-more: $(TS_ALL)
	srcdir=$(srcdir) $(srcdir)/runtests $(TS_ALL)

