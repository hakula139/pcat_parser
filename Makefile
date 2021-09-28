TARGET    := lexer

BIN_DIR   := bin
SRC_DIR   := src

LEX_IN    := $(SRC_DIR)/lexer.lex
LEX_OUT   := $(LEX_IN:%.lex=%.cpp)
SRCS      := $(shell find $(SRC_DIR) -name *.cpp)

LEX       := flex
CXX       := g++
CXXFLAGS  := -g -Wall -O3 -std=c++17
MKDIR     := mkdir -p
RM        := rm -rf

.PHONY: clean

$(BIN_DIR)/$(TARGET): $(SRCS) $(LEX_OUT)
	@echo + $@
	@$(MKDIR) $(dir $@)
	@$(CXX) $(CXXFLAGS) -o $@ $^

$(LEX_OUT): $(LEX_IN)
	@echo + $@
	@$(LEX) -o $@ $<

clean:
	@$(RM) $(BIN_DIR) $(LEX_OUT)
