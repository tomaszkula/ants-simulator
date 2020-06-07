main:
	g++ -o ants_simulator main.cpp Board.cpp GameObject.cpp Creature.cpp Ant.cpp Monster.cpp Food.cpp Blockade.cpp -pthread -lncurses
	./ants_simulator
