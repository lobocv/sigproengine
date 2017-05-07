CC=g++

PYTHON_VERSION = 2.7
PYTHON_INCLUDE = /usr/include/python$(PYTHON_VERSION)

BOOST_DIR = /usr/local/boost_1_63_0
BOOST_LIB = $(BOOST_DIR)/stage/lib
BOOST_INC = $(BOOST_DIR)


TARGET = sigproengine
SRC_DIR = src
OUTPUT_DIR = bin

all: $(OUTPUT_DIR)/process.o $(OUTPUT_DIR)/gain.o $(OUTPUT_DIR)/$(TARGET).o $(OUTPUT_DIR)/$(TARGET).so


$(OUTPUT_DIR)/$(TARGET).so: $(OUTPUT_DIR)/$(TARGET).o
	$(CC) -shared -I$(BOOST_LIB) -L$(BOOST_LIB) -o $(OUTPUT_DIR)/$(TARGET).so $(OUTPUT_DIR)/process.o $(OUTPUT_DIR)/gain.o $(OUTPUT_DIR)/$(TARGET).o  -lpython2.7 -lboost_python -lboost_numpy


$(OUTPUT_DIR)/$(TARGET).o:
	$(CC) -std=c++11 -o $(OUTPUT_DIR)/$(TARGET).o -c $(SRC_DIR)/$(TARGET).cpp -Wall -fPIC -I$(PYTHON_INCLUDE) -I$(BOOST_INC)

$(OUTPUT_DIR)/process.o:
	$(CC) -std=c++11 -o $(OUTPUT_DIR)/process.o -c $(SRC_DIR)/process.cpp -Wall -fPIC -I$(PYTHON_INCLUDE) -I$(BOOST_INC)

$(OUTPUT_DIR)/gain.o:
	$(CC) -std=c++11 -o $(OUTPUT_DIR)/gain.o -c $(SRC_DIR)/gain.cpp -Wall -fPIC -I$(PYTHON_INCLUDE) -I$(BOOST_INC)

clean:
	rm -r $(OUTPUT_DIR)/* 
	touch $(OUTPUT_DIR)/__init__.py

test:
	make clean && make all && python callfrompy.py 

