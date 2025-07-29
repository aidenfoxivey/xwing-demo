CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2
CC = gcc
CFLAGS = -std=c99 -Wall -O2

ML_KEM_DIR = ml-kem-768

ML_KEM_SOURCES = $(ML_KEM_DIR)/cbd.c \
                 $(ML_KEM_DIR)/fips202.c \
                 $(ML_KEM_DIR)/indcpa.c \
                 $(ML_KEM_DIR)/kem.c \
                 $(ML_KEM_DIR)/ntt.c \
                 $(ML_KEM_DIR)/poly.c \
                 $(ML_KEM_DIR)/polyvec.c \
                 $(ML_KEM_DIR)/randombytes.c \
                 $(ML_KEM_DIR)/reduce.c \
                 $(ML_KEM_DIR)/symmetric-shake.c \
                 $(ML_KEM_DIR)/verify.c

ML_KEM_OBJECTS = $(ML_KEM_SOURCES:.c=.o)
CXX_OBJECTS = XWing.o main.o Curve25519.o RNG.o ChaCha.o Cipher.o Crypto.o
C_OBJECTS = BigNumberUtil.o

# Target executable
TARGET = xwing

# Default target
all: $(TARGET)

# Link the final executable
$(TARGET): $(CXX_OBJECTS) $(ML_KEM_OBJECTS) $(C_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile C++ source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile C source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(CXX_OBJECTS) $(ML_KEM_OBJECTS) $(C_OBJECTS) $(TARGET)

# Phony targets
.PHONY: all clean
