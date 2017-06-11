CC=g++

PYTHON_VERSION = 2.7
PYTHON_INCLUDE = /usr/include/python$(PYTHON_VERSION)

BOOST_DIR = /usr/local/boost_1_63_0
BOOST_LIB = $(BOOST_DIR)/stage/lib
BOOST_INC = $(BOOST_DIR)
CFLAGS = -std=c++11 -Wall -fPIC
LIBS = -lpython2.7 -lboost_python -lboost_numpy
SRC_DIR = src
OUTPUT_DIR = bin

MODULES =$(OUTPUT_DIR)/processingchain.o $(OUTPUT_DIR)/process.o $(OUTPUT_DIR)/gain.o $(OUTPUT_DIR)/sigproengine.o

all: $(MODULES) $(OUTPUT_DIR)/sigproengine.so

$(OUTPUT_DIR)/%.so: $(MODULES)
	$(CC) -shared -L$(BOOST_LIB) -o $@ $^ $(LIBS)

$(OUTPUT_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ -I$(PYTHON_INCLUDE) -I$(BOOST_INC)

clean:
	rm -r $(OUTPUT_DIR)/* 
	touch $(OUTPUT_DIR)/__init__.py

test:
	make clean && make all && python callfrompy.py

