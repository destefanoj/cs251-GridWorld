prog2: driver.cpp GridWorld.h GWInterface.h
	g++ -std=c++11 driver.cpp -o prog2
clean:
	rm -f prog2
