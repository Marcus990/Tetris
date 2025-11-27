CXX = g++-14 -std=c++20 -fmodules-ts
CXXFLAGS = -Wall -Wextra -g -I/opt/X11/include
COMPH = $(CXX) -c -x c++-system-header
LDFLAGS = -L/opt/X11/lib -lX11

EXEC = biquadris

SOURCES = constants.cc cell.cc block.cc blocks.cc observer.cc scorekeeper.cc \
          level.cc effect.cc board.cc window.cc window-impl.cc textdisplay.cc \
          graphicsdisplay.cc game.cc command.cc main.cc

OBJECTS = $(SOURCES:.cc=.o)

HEADERS = vector utility map memory algorithm iostream cstdlib fstream random cctype string

$(EXEC): precompiled-headers $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXEC) $(LDFLAGS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $<

precompiled-headers:
	$(COMPH) $(HEADERS)

.PHONY: clean rebuild precompiled-headers

clean:
	rm -rf gcm.cache
	rm -f *.o $(EXEC)

rebuild: clean $(EXEC)
