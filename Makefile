CXX= g++
CXXFLAGS = -Wall -O -g `pkg-config gtkmm-2.4 --cflags --libs`
OBJS = DeckGUI.o view.o main.o model.o controller.o subject.o Computer.o Human.o Player.o Card.o
DEPENDS = ${OBJECTS:.o=.d}
EXEC = straights

$(EXEC):$(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) -o $(EXEC)

clean:
	rm -rf ${DEPENDS} ${OBJS} -o ${EXEC}

-include ${DEPENDS}
