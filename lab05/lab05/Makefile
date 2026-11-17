CXX ?= g++
CXXFLAGS ?= -std=c++11 -Wall -Wextra -Wno-sign-compare
GRAM ?= 9

TARGET := compilador
BUILD_DIR := build
GRAM_DIR := src/src-gram-java
TREE_DIR := src/Tree
GRAM_FILE ?= gramatica-$(GRAM)/gramatica.conf
TABELA_FILE ?= gramatica-$(GRAM)/tabela_lr1.conf

CORE_SRCS := \
	src/sol.cpp \
	src/Arvore.cpp \
	src/Gramatica.cpp \
	src/Parser.cpp \
	src/TabelaLR1.cpp \
	src/debug-util.cpp \
	src/matriz-util.cpp

GRAM_SRCS := $(wildcard $(GRAM_DIR)/*.cpp)
TREE_SRCS := $(wildcard $(TREE_DIR)/*.cpp)
SRCS := $(CORE_SRCS) $(GRAM_SRCS) $(TREE_SRCS)
OBJS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I$(GRAM_DIR) -Isrc -MMD -MP -c $< -o $@

run: $(TARGET)
	./$(TARGET) $(GRAM_FILE) $(TABELA_FILE) $(PARAMS)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

-include $(DEPS)
