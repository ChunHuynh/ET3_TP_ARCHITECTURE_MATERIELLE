
CXX=g++
CXXFLAGS=

%.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

tp2: tpCaches.o cSimCache.o parseArgs.o exo3.o exo4.o exo5.o exo6.o exo7.o
	$(CXX) $^ -o $@

.PHONY: run clean

clean:
	rm -f *.o *~ core 

