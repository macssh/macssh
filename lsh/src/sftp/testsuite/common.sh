# Common stuff for all test scripts.

set -e

TESTHOME=home
export TESTHOME

# We need an absolute path.
PROGRAMDIR=`cd .. && pwd`
export PROGRAMDIR

if [ -n "$SFTP_DEBUG" ] ; then
    SERVER=debug-server
else
    SERVER=sftp-server
fi

client () {
    (cd $TESTHOME && "$PROGRAMDIR"/sftp-test-client "$PROGRAMDIR"/"$SERVER" "$@")
}
    
