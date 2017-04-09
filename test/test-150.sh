#!/bin/sh

#
# Test dans le cas où le remplissage d'un buffer se fait en plusieurs fois,
# avec plusieurs à read, en utilisant fsync.
# Voir test/test-150-script.c
#

TEST=$(basename $0 .sh)

TMP=/tmp/$TEST
LOG=$TEST.log
V=${VALGRIND}		# appeler avec la var. VALGRIND à "" ou "valgrind -q"

exec 2> $LOG
set -x

rm -f *.tmp

fail ()
{
    echo "==> Échec du test '$TEST' sur '$1'."
    echo "==> Log : '$LOG'."
    echo "==> Exit"
    exit 1
}

# The output should be the same as one run
./bin/test-150-script > f2.tmp

$V ./detecter -i1 -l5 bin/test-150-script > f1.tmp \
	&& cmp -s f1.tmp f2.tmp 		|| fail "write avec fsync"

