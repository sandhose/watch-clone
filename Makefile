#
# Ce Makefile contient les cibles suivantes :
#
# all		: compile les programmes
# clean		: supprime les fichiers générés automatiquement
# coverage	: compile les programmes pour mesurer la couverture de code
# test		: lance tous les tests (scripts shell test-xxx.sh)
#		  (sans appeler valgrind)
# gcov		: génère les rapports de couverture (à lancer après avoir
#		  lancé les cibles 'coverage' et 'test').
#		  Résultats dans *.gcov
# ctags		: génère un fichier tags pour la navigation avec vim.
#		  (voir http://usevim.com/2013/01/18/tags/)
#
# De plus, les cibles supplémentaires suivantes sont fournies pour
# simplifier les tâches répétitives :
#
# test-avec-valgrind	: lance les tests avec valgrind (conseillé)
# couverture-et-tests	: automatise les tests avec rapport de couverture
#
NAME    = detecter

ARCHIVE = $(NAME)-snapshot

COV     = -coverage

CFLAGS  = -Wall -Wextra -Werror -g $(COVERAGE)

PROGS   = $(NAME)

SOURCES = src/buffer.c src/main.c src/spawn.c src/util.c src/watch.c
HEADERS = src/buffer.h src/spawn.h src/util.h src/watch.h
TESTS   = test/test-100.sh test/test-110.sh test/test-120.sh test/test-130.sh test/test-140.sh test/test-160.sh

OBJECTS = $(SOURCES:src/%.c=obj/%.o)

all: ctags $(PROGS) $(ARCHIVE).tar.gz

$(NAME): bin/$(NAME)
	ln -sf $< $@

src/buffer.c: src/buffer.h
src/main.c: src/watch.h src/util.h src/buffer.h
src/spawn.c: src/spawn.h
src/util.c: src/util.h
src/watch.c: src/watch.h src/spawn.h

obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

bin/$(NAME): $(OBJECTS)
	@mkdir -p bin
	$(CC) $(CFLAGS) $^ -o $@

coverage: clean
	$(MAKE) COVERAGE=$(COV)

gcov:
	gcov -o obj/ src/*.c

$(ARCHIVE).tar.gz: $(SOURCES) $(HEADERS) $(TESTS) Makefile
	ln -s . $(ARCHIVE)
	tar -czvf $@ $(^:%=$(ARCHIVE)/%)
	rm $(ARCHIVE)

# Par défaut, "test" lance les tests sans valgrind.
# Si on souhaite utiliser valgrind (conseillé), positionner la
# variable VALGRIND ou utiliser la cible "test-avec-valgrind"

test:	test-sans-valgrind

test-sans-valgrind: all
	@for i in $(TESTS) ; do echo $$i ; sh $$i || exit 1 ; done

test-avec-valgrind: all
	VALGRIND="valgrind -q" ; export VALGRIND ; for i in $(TESTS) ; do echo $$i ; sh $$i || exit 1 ; done

couverture-et-tests: clean coverage test gcov

ctags:
	ctags src/*.[ch]

clean:
	rm -f $(PROGS)
	rm -f obj/* bin/*
	rm -f *.log *.tmp
	rm -f tags core
	rm -f *.gcov
	rm -f $(ARCHIVE).tar.gz
