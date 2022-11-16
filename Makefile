#define colors for nice output
ifneq (,$(findstring xterm,${TERM}))
	RED          := $(shell tput -Txterm setaf 1)
	GREEN        := $(shell tput -Txterm setaf 2)
	DARKBLUE         := $(shell tput -Txterm setaf 4)
	PURPLE       := $(shell tput -Txterm setaf 5)
	BLUE         := $(shell tput -Txterm setaf 6)
	RESET := $(shell tput -Txterm sgr0)
else
	RED       := ""
	GREEN		:= ""
	PURPLE       := ""
	BLUE         := ""
	RESET        := ""
endif

#TODO -- The CXX compiler identification is GNU 10.2.1

#Tools to check what the OS is
UNAME := $(shell uname)

PYTHON_SUB_VERSION = $(word 2, $(wordlist 2,4,$(subst ., ,$(shell python3 --version 2>&1))))

CXXFLAGS= -Wno-unused-result -Wsign-compare -Wunreachable-code -fno-common -fwrapv -dynamic -O3 -I./include -I$(shell python3 -c "from sysconfig import get_paths as gp; print(gp()[\"include\"])") --std=c++20
LDFLAGS=-lboost_python310

ifneq ($(UNAME), Darwin)
    CXXFLAGS += -lpython3 -fPIC
else
	LDFLAGS += -undefined dynamic_lookup -Wl,-no_fixup_chains
endif

OUT=./bin/libChineseCheckers.so
CXXFILES = ./src/ChineseCheckers.cpp ./src/ChineseCheckersWrapper.cpp
OFILES = $(patsubst ./src/%.cpp, ./objects/%.o, $(CXXFILES))

OUTALPHABETA = ./bin/solvers/AlphaBeta.so
CXXFILESALPHABETA = ./solvers/AlphaBeta/src/AlphaBeta.cpp ./solvers/AlphaBeta/src/AlphaBetaWrapper.cpp
OFILESALPHABETA = $(patsubst ./solvers/AlphaBeta/src/%.cpp, ./objects/%.o, $(CXXFILESALPHABETA))

OUTUNITESTS = ./bin/unittests.out
CXXFILESUNITTESTS = ./src/ChineseCheckers_unittest.cpp
OFILESUNITTESTS = $(patsubst ./src/%.cpp, ./objects/%.o, $(CXXFILESUNITTESTS))

DIRECTORIES = ./objects ./bin ./bin/solvers

####Main library
all: $(DIRECTORIES) $(OUT) AlphaBeta unittests
	@echo "${BLUE}Checking if the C++ code respects Google's conventions${RESET}"
	@cpplint ./src/* ./include/* > /dev/null
	@echo "${DARKBLUE}Running unit tests${RESET}"
	@./bin/unittests.out


$(DIRECTORIES) :
	@echo "${GREEN}Creating missing directory" $@ "${RESET}"
	@mkdir $@

$(OUT): $(OFILES)
	@echo "${BLUE}Linking the library${RESET}"
	@$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS) -shared



./objects/%.o: ./src/%.cpp | $(DIRECTORIES)
	@echo "${PURPLE}Building CXX object" $@ "${RESET}"
	@$(CXX)  -o $@ -c $< $(CXXFLAGS)
	
####Documentation
.PHONY: doc
doc:
	@echo "${BLUE}Updating the documentation${RESET}"
	@doxygen > /dev/null

####Solvers

#AlphaBeta
AlphaBeta: $(OUT) $(DIRECTORIES) $(OUTALPHABETA)
	@echo "${BLUE}Checking if the C++ code of AlphaBeta respects Google's conventions${RESET}"
	@cpplint ./solvers/AlphaBeta/src/* ./solvers/AlphaBeta/include/* > /dev/null

$(OUTALPHABETA): $(OFILESALPHABETA)
	@echo "${BLUE}Linking AlphaBeta solver${RESET}"
	@$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS) -pthread -shared -Wl,-rpath,./bin -Lbin -lChineseCheckers
	
./objects/%.o: ./solvers/AlphaBeta/src/%.cpp | $(DIRECTORIES)
	@echo "${PURPLE}Building CXX object" $@ "${RESET}"
	@$(CXX) -o $@ -c $< $(CXXFLAGS) -I./solvers/AlphaBeta/include/
	



####Unit tests
.PHONY: unittests
unittests: $(OUT) $(OUTUNITESTS)

$(OUTUNITESTS): $(OFILESUNITTESTS)
	@echo "${BLUE}Linking unit tests${RESET}"
	@$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS) -pthread -Wl,-rpath,./bin -Lbin -lChineseCheckers -lgtest $(shell python3-config --embed --ldflags)

$(OFILESUNITTESTS): $(CXXFILESUNITTESTS)
	@echo "${PURPLE}Building CXX object" $@ "${RESET}"
	@$(CXX) -o $@ -c $< $(CXXFLAGS) -I./include/


####Cleaning
clean :
	@echo "${RED}Cleaning${RESET}"
	@$(RM) $(OFILES)
	@$(RM) -dr ./objects/


mrproper : clean
	@echo "${RED}Cleaning all files${RESET}"
	@$(RM) -dr ./bin
