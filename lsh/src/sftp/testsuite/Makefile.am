TS_SH = connect-test get-1-test ls-1-test ls-2-test put-1-test

EXTRA_DIST = common.sh run-tests \
	    setup-env teardown-env \
	    $(TS_SH)

# I thought srcdir was exported automatically?
check:
	srcdir="$(srcdir)" $(srcdir)/run-tests $(TS_SH)
