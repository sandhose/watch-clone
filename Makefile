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
CC	= clang
RM	= rm -f
COV	= -coverage
CFLAGS	= -Wall -Wextra -Werror -g $(COVERAGE)

NAME	= detecter
PROGS	= $(NAME)

ARCHIVE	= $(NAME)-snapshot
FILES   = $(SOURCES) $(HEADERS) $(TESTS) test/test-150-script.c Makefile

SOURCES	= src/buffer.c src/main.c src/spawn.c src/util.c src/watch.c
HEADERS	= src/buffer.h src/spawn.h src/util.h src/watch.h

TESTS	= test/test-100.sh test/test-110.sh test/test-120.sh test/test-130.sh test/test-140.sh test/test-150.sh test/test-160.sh

OBJECTS	= $(SOURCES:src/%.c=obj/%.o)

.PHONY: all
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

.PHONY: coverage
coverage: clean
	$(MAKE) COVERAGE=$(COV)

.PHONY: gcov
gcov:
	gcov -o obj/ src/*.c

$(ARCHIVE).tar.gz: $(FILES)
	ln -s . $(ARCHIVE)
	tar -czvf $@ $(^:%=$(ARCHIVE)/%)
	$(RM) $(ARCHIVE)

# Par défaut, "test" lance les tests sans valgrind.
# Si on souhaite utiliser valgrind (conseillé), positionner la
# variable VALGRIND ou utiliser la cible "test-avec-valgrind"

.PHONY: test
test: $(TESTS:test/test-%.sh=test-%.log)

.PHONY: test-avec-valgrind
test-avec-valgrind:
	VALGRIND="valgrind -q" make test

test-%.log: test/test-%.sh $(NAME)
	sh $<

# Additional binary needed for a test
bin/test-150-script: test/test-150-script.c
	@mkdir -p bin
	$(CC) $(CFLAGS) $^ -o $@

test/test-150.sh: bin/test-150-script

.PHONY: couverture-et-tests
couverture-et-tests: clean coverage test gcov

.PHONY: ctags
ctags: tags
tags: $(SOURCES) $(HEADERS)
	ctags $^

.PHONY: indent
indent: $(SOURCES) $(HEADERS)
	indent $^

.PHONY: clean
clean:
	$(RM) $(PROGS)
	$(RM) -r obj/* bin/*
	$(RM) *.log *.tmp
	$(RM) tags core
	$(RM) *.gcov
	$(RM) $(ARCHIVE).tar.gz
