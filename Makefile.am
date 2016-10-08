AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}
EXTRA_DIST = autogen.sh images misc src/gettext.h src/occillo.rc game fonts music sounds pkg
SUBDIRS = src tests images misc po docs fonts game music sounds

# Netbeans support for cppunit testing.

# Test Directory
TESTDIR=tests

# build tests
build-tests: check
	
# Run Test Targets
test: 
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/collision || true; \
			${TESTDIR}/colours || true; \
	else  \
	    ./${TEST} || true; \
	fi
