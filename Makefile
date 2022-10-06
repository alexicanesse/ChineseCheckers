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


OUT=./bin/ChineseCheckers
CXXFLAGS=-Wall -I./include --std=c++17

CXXFILES = $(wildcard ./src/*.cpp)
OFILES = $(patsubst ./objects/%.hpp, ./src/%.cpp, $(CXXFILES))
DIRECTORIES = ./objects ./bin


all: $(DIRECTORIES) $(OUT)
$(DIRECTORIES) :
	@echo "${GREEN}Creating missing directory" $@ "${RESET}"
	@mkdir $@

$(OUT): $(OFILES)
	@echo "${BLUE}Linking CXX objects${RESET}"
	@$(CXX) $(CXXFLAGS) $(LDFLAGS) -shared -o $@ $^

./objects/%.o: ./src/%.cpp | $(DIRECTORIES)
	@echo "${PURPLE}Building CXX object" $@ "${RESET}"
	@$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ -c $<


clean :
	@echo "${RED}Cleaning${RESET}"
	@$(RM) $(OFILES)
	@$(RM) -dr ./objects/
	

mrproper : clean
	@echo "${RED}Cleaning all files${RESET}"
	@$(RM) $(OUT)
	@$(RM) $(LATEXFILES_pdf)
