#!/bin/sh

#
# Additional tests for coverage
#

TEST=$(basename $0 .sh)

TMP=/tmp/$TEST
LOG=$TEST.log
V=${VALGRIND}		# appeler avec la var. VALGRIND à "" ou "valgrind -q"

exec 2> $LOG
set -x

fail ()
{
    echo "==> Échec du test '$TEST' sur '$1'."
    echo "==> Log : '$LOG'."
    echo "==> Exit"
    exit 1
}

./detecter -i1 -l1 some_invalid_command		|| fail "commande invalide"

./detecter -h					&& fail "syntaxe -h"

./detecter -w					&& fail "syntaxe invalide"

./detecter -i1 -l10 true			|| fail "commande sans output"

exit 0
