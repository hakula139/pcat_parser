TARGET    := parser

BIN_DIR   := bin
BUILD_DIR := build
SRC_DIR   := src
OUT_DIR   := output

LEX_IN    := $(SRC_DIR)/lexer.lex
LEX_SRC   := $(SRC_DIR)/lexer.cpp

YACC_IN   := $(SRC_DIR)/parser.yy
YACC_SRC  := $(SRC_DIR)/parser.cpp
YACC_SRCS := $(SRC_DIR)/location.hpp $(SRC_DIR)/parser.hpp $(SRC_DIR)/parser.cpp

SRCS      := $(shell find $(SRC_DIR) -name *.cpp) $(YACC_SRC)
OBJS      := $(SRCS:%=$(BUILD_DIR)/%.o)

LEX       := flex
YACC      := bison
CXX       := g++
CXXFLAGS  := -g -Wall -O3 -std=c++17
MKDIR     := mkdir -p
RM        := rm -rf

.PHONY: start clean

start: $(BIN_DIR)/$(TARGET)
	@$(MKDIR) $(OUT_DIR)
	@$< $(INPUT)

$(BIN_DIR)/$(TARGET): $(OBJS)
	@echo + $@
	@$(MKDIR) $(dir $@)
	@$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.cpp.o: %.cpp
	@echo + $@
	@$(MKDIR) $(dir $@)
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

$(LEX_SRC): $(LEX_IN)
	@echo + $@
	@$(LEX) -o $@ $<

$(YACC_SRC): $(YACC_IN)
	@echo + $@
	@$(YACC) -o $@ -d $<

clean:
	@$(RM) $(BIN_DIR) $(BUILD_DIR) $(LEX_SRC) $(YACC_SRCS)
