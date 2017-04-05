#!/bin/sh

#
# Tests additionels de changement pour couverture
# Test dans le cas où la taille de la sortie est la même, mais pas le même
# contenu
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

# test de changement

cat > script.tmp <<'EOF'
#!/bin/sh
    # script pour faire un changement à chaque fois
    F=f1.tmp
    if [ -f $F ]
    then echo "toto" ; rm $F ; r=1
    else echo "titi" ; touch $F ; r=0
    fi
    exit $r
EOF
chmod +x script.tmp

N=5
for i in $(seq 1 $N)
do
    echo titi
    echo toto
done > f3.tmp

rm -f f1.tmp
./detecter -i1 -l$((N*2)) ./script.tmp > f2.tmp \
	&& cmp -s f2.tmp f3.tmp 		|| fail "toto-titi"

