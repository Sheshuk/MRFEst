CXX = g++
CFLAGS=-O3 -Wall -std=c++0x 
CFLAGS+=-I `root-config --incdir` -I include/
LIBS=`root-config --libs`  -lm  -lstdc++
OBJ = MyConfig.o Terrain.o RayTrace.o Dem.o Attenuation.o Flux.o
TARGET =MuRaySim

all:  $(TARGET)

% : %.o
	$(CXX) $(CFLAGS) $(LIBS) -o $@ $^
	strip $@

%.o: src/%.cpp
	$(CXX) $(CFLAGS) -o $@ -c $^

obj: $(OBJ)

$(TARGET): obj

clean:
	rm *.o
