EXE = enigma
OBJ = errors.o plugboard.o rotor.o enigma.o reflector.o main.o
CXX = g++
CXXFLAGS = -Wall -MMD -g

$(EXE): $(OBJ)
	$(CXX) $^ -o $(EXE)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

-include $(OBJ:.o=.d)

clean:
	rm -rf $(EXE) $(OBJ) $(OBJ:.o=.d)

.PHONY: clean
