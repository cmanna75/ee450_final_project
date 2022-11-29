
# all creates executable for the following
all: serverM serverC serverEE serverCS client

#serverM compile
serverM: serverM.cpp utilities.o
	g++ -o serverM serverM.cpp utilities.o

#serverC compile
serverC: serverC.cpp utilities.o
	g++ -o serverC serverC.cpp utilities.o

#serverEE compile
serverEE: serverEE.cpp utilities.o
	g++ -o serverEE serverEE.cpp utilities.o

#serverCS compile
serverCS: serverCS.cpp utilities.o
	g++ -o serverCS serverCS.cpp utilities.o

#client compile
client: client.cpp utilities.o
	g++ -o client client.cpp utilities.o

#utilities compile, used in all other makes
utilities.o: utilities.cpp utilities.h
	g++ -c utilities.cpp