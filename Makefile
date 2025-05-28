CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -fPIC `pkg-config --cflags Qt5Widgets`
LDFLAGS = `pkg-config --libs Qt5Widgets`
MOC = moc
RM = rm -f

SRC = Game.cpp player.cpp GameWindow.cpp StartDialog.cpp
MOC_HEADERS = GameWindow.h StartDialog.h
MOC_SRC = $(MOC_HEADERS:.h=.moc.cpp)
OBJ = $(SRC:.cpp=.o) $(MOC_SRC:.cpp=.o)

all: Main

Main: CoupDemo.o $(OBJ)
	$(CXX) $^ -o CoupGame $(LDFLAGS)

test: test.cpp Game.cpp player.cpp
	$(CXX) $(CXXFLAGS) test.cpp Game.cpp player.cpp -o test_runner $(LDFLAGS)
	./test_runner

valgrind: test
	valgrind ./test_runner

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.moc.cpp: %.h
	$(MOC) $< -o $@

clean:
	$(RM) *.o *.moc.cpp test_runner CoupGame
