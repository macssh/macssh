SUBDIRS = . testsuite

AUTOMAKE_OPTIONS = foreign

bin_PROGRAMS = sftp-server
noinst_PROGRAMS = sftp-test-client dump-hex

EXTRA_DIST = buffer.h client.h filemode.h idcache.h sftp.h werror.h xmalloc.h

sftp_server_SOURCES = sftp-server.c buffer.c filemode.c idcache.c xmalloc.c

sftp_test_client_SOURCES = sftp-test-client.c buffer.c xmalloc.c werror.c client.c

