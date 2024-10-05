# Makefile

# Compiler
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

# LaTeX
LATEX = pdflatex
LATEXFLAGS = 

# Target executable
TARGET = HW1

# Source files
SOURCES = HW1.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# PDF target
PDF = main.pdf

# Default target
all: $(TARGET) $(PDF)

# Compile C++ source files
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile PDF from LaTeX
$(PDF): main.tex
	$(LATEX) $(LATEXFLAGS) main.tex
	$(LATEX) $(LATEXFLAGS) main.tex  # Run twice for references

# Clean up
clean:
	rm -f $(OBJECTS) $(TARGET) $(PDF) main.aux main.log main.out

.PHONY: all clean