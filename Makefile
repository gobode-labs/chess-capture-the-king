# @gobode_lab
# Makefile — Build system for Chess (capture‑the‑king)
# Author: Eduardo

CXX       = g++
CXXFLAGS  = -std=c++17 -Wall -Wextra

TARGET = chess

SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
 $(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o: %.cpp
 $(CXX) $(CXXFLAGS) -c $< -o $@

clean:
 rm -f $(TARGET) $(OBJS)
