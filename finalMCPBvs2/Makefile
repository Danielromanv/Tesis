OBJS = main.o Node.o  Truck.o ProblemInstance.o Reader.o Trip.o Route.o Solution.o Construction.o ReorderNodes.o RemoveNodes.o AddNodes.o

all: MCPB

MCPB: $(OBJS)
	g++ -o MCPB $(OBJS) $(CFLAGS)

%.o: %.cpp
	g++ -c $(CFLAGS) $< -o $@

clean:
	rm -f MCPB *.o

exe:
	@./MCPB ${ARGS}
