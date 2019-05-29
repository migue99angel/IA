#ifndef PLAYER_H
#define PLAYER_H

#include "environment.h"

class Player{
    public:
      Player(int jug);
      Environment::ActionType Think();
      void Perceive(const Environment &env);
	double alphabeta(const Environment & T, int jugador, int profundidad, double alpha, double beta, Environment::ActionType & accion,int max, int & numNodos);
    private:
      int jugador_;
      Environment actual_;
};
#endif
