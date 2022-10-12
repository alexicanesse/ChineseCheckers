#define colors for nice output
ifneq (,$(findstring xterm,${TERM}))
	RED          := $(shell tput -Txterm setaf 1)
	GREEN        := $(shell tput -Txterm setaf 2)
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


#Tools to check if the OS is macOS.
UNAME := $(shell uname)

OUT=./bin/ChineseCheckers.so
CXXFLAGS=-Wno-unused-result -Wsign-compare -Wunreachable-code -fno-common -fwrapv -dynamic -O3 -I./include -I$(shell python3 -c "from sysconfig import get_paths as gp; print(gp()[\"include\"])") --std=c++17
LDFLAGS=-lboost_python310 

ifneq ($(UNAME), Darwin)
	CXXFLAGS += -lpython3.10 -fPIC
else
	CXXFLAGS += -undefined dynamic_lookup
endif

CXXFILES = $(wildcard ./src/*.cpp)
OFILES = $(patsubst ./src/%.cpp, ./objects/%.o, $(CXXFILES))
DIRECTORIES = ./objects ./bin


all: $(DIRECTORIES) $(OUT)
	@echo "${BLUE}Updating the documentation${RESET}"
	@doxygen > /dev/null

$(DIRECTORIES) :
	@echo "${GREEN}Creating missing directory" $@ "${RESET}"
	@mkdir $@

$(OUT): $(OFILES)
	@echo "${BLUE}Linking CXX objects${RESET}"
	@$(CXX) $(CXXFLAGS) $(LDFLAGS) -shared -o $@ $^

./objects/%.o: ./src/%.cpp | $(DIRECTORIES)
	@echo "${PURPLE}Building CXX object" $@ "${RESET}"
	@$(CXX) $(CXXFLAGS) -o $@ -c $<

clean :
	@echo "${RED}Cleaning${RESET}"
	@$(RM) $(OFILES)
	@$(RM) -dr ./objects/


mrproper : clean
	@echo "${RED}Cleaning all files${RESET}"
	@$(RM) $(OUT)
