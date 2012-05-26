NAME = 		main
CXX =		g++
DEFINES = 
CXXFLAGS =	-O3 -pipe -ffast-math -funroll-loops -march=i686 -Wall -std=c++11
LINK =		g++
LFLAGS =	-O3 -s
LIBS =		-lboost_serialization

ECHO = 		printf
CAT = 		cat
NO_COLOR =	\033[0m
OK_COLOR =	\033[32;01m
ERROR_COLOR =	\033[31;01m
WARN_COLOR =	\033[33;01m

OK_STRING = 	$(OK_COLOR)[OK]$(NO_COLOR)
ERROR_STRING =	$(ERROR_COLOR)[ERRORS]$(NO_COLOR)
WARN_STRING =	$(WARN_COLOR)[WARNINGS]$(NO_COLOR)

ERR_HANDLING = 	2> temp.log || touch temp.errors
ERR_PRINTING = 	@if test -e temp.errors; \
		then $(ECHO) "$(ERROR_STRING)\n" && $(CAT) temp.log;\
		rm -f temp.errors temp.log; exit 1; \
		elif test -s temp.log; \
		then $(ECHO) "$(WARN_STRING)\n" && $(CAT) temp.log; \
		else $(ECHO) "$(OK_STRING)\n"; fi; rm -f temp.errors temp.log


SRC = 		main.cpp
OBJ = 		$(SRC:.cpp=.o)

all: $(NAME)

tur: header tur.o
	@$(ECHO) "Linking... \t"
	@$(LINK) $(LFLAGS) -o $@ tur.o $(LIBS) $(ERR_HANDLING)
	$(ERR_PRINTING)

exp: header experiments.o
	@$(ECHO) "Linking... \t"
	@$(LINK) $(LFLAGS) -o $@ experiments.o $(LIBS) $(ERR_HANDLING)
	$(ERR_PRINTING)

$(NAME): header $(NAME).o
	@$(ECHO) "Linking... \t"
	@$(LINK) $(LFLAGS) -o $@ $@.o $(LIBS) $(ERR_HANDLING)
	$(ERR_PRINTING)

%.o: %.cpp turing-machine.hpp action.hpp random.hpp utils.hpp living-tm.hpp population.hpp
	@$(ECHO) "Compiling $<... \t"
	@$(CXX) $(CXXFLAGS) $(INCPATH) -c -o $@ $< $(ERR_HANDLING)
	$(ERR_PRINTING)

header:
	@$(ECHO) "$(OK_COLOR)"
	@$(CAT) HEADER
	@$(ECHO) "$(NO_COLOR)"

clean:
	rm -rf *.o
	rm -rf main
