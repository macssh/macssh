#! /bin/sh

# Build all automatically generated files that are not present in the
# CVS repository.

# This script is for use in the source directory, before you run
# configure. To get started from a fresh CVS checkout, you also need
# to run configure and make bootstrap from your build directory.

set -e
bash make_am
aclocal
autoheader
autoconf
(cd src/argp && aclocal)
(cd src/argp && autoconf)
(cd src/argp && autoheader)

(cd src/nettle && ./.bootstrap)
(cd src/sftp && ./.bootstrap)

automake -a Makefile src/Makefile

(cd src/argp && automake -a)
(cd src/nettle && automake -a)
(cd src/sftp && automake -a)
