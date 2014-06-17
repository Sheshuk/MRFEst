CXX = g++
CFLAGS=-O3 -Wall 
#-std=c++0x
CFLAGS+=-I `root-config --incdir` -I include/
LIBS=`root-config --libs`  -lm  -lstdc++
OBJ = main.o MyConfig.o Terrain.o RayTrace.o Dem.o Attenuation.o Flux.o
TARGET =MuRaySim
BIN=./bin/
OBJECTS=$(addprefix $(BIN),$(OBJ))

all:  $(TARGET)

$(TARGET): main.o
	$(CXX) $(CFLAGS) $(LIBS) -o $@ $(OBJECTS)
	strip $@

%.o: src/%.cpp
	$(CXX) $(CFLAGS) -o $@ -c $^
	mv $@ $(BIN)
obj: $(OBJECTS)

$(TARGET): $(OBJ)

clean:
	rm $(BIN)*
	rm *.o
