# Makefile for RTGP lab lecture exercises - Linux environment
# author: Davide Gadia
# Real-Time Graphics Programming - a.a. 2025/2026
# Master degree in Computer Science
# Universita' degli Studi di Milano
#

# name of the file
FILENAME = main

CC = gcc
CXX = g++

# Include path
IDIR = Libraries/include/
STB_DIR = Textures/Headers
RP3D_INC = /usr/local/include

# Libraries path
LDIR = Libraries/lib/
RP3D_LIB = /usr/local/lib

# compiler flags:
CXXFLAGS  = -g -O0 -x c++ -Wall -Wno-invalid-offsetof -std=c++17 -I$(IDIR) -I$(STB_DIR) -I$(RP3D_INC)

# linker flags:
LDFLAGS = -L$(LDIR) -L$(RP3D_LIB) -lglfw3 -lassimp -lz -lreactphysics3d -ldl -lm -lpthread -lX11 

SOURCES = Libraries/src/glad.c $(FILENAME).cpp stb_image.cpp global.cpp graphics.cpp input.cpp physics.cpp rayCaster.cpp

TARGET = $(FILENAME).out

.PHONY : all
all:
	$(CXX) $(CXXFLAGS) $(SOURCES) $(LDFLAGS) -o $(TARGET)

.PHONY : clean
clean :
	-rm $(TARGET)
