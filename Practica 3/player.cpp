#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include "player.h"
#include "environment.h"

using namespace std;

const double masinf=9999999999.0, menosinf=-9999999999.0;


// Constructor
Player::Player(int jug){
    jugador_=jug;
}

// Actualiza el estado del juego para el jugador
void Player::Perceive(const Environment & env){
    actual_=env;
}

double Puntuacion(int jugador, const Environment &estado){
    double suma=0;

    for (int i=0; i<7; i++)
      for (int j=0; j<7; j++){
         if (estado.See_Casilla(i,j)==jugador){
            if (j<3)
               suma += j;
            else
               suma += (6-j);
         }
      }

    return suma;
}


// Funcion de valoracion para testear Poda Alfabeta
double ValoracionTest(const Environment &estado, int jugador){
    int ganador = estado.RevisarTablero();

    if (ganador==jugador)
       return 99999999.0; // Gana el jugador que pide la valoracion
    else if (ganador!=0)
            return -99999999.0; // Pierde el jugador que pide la valoracion
    else if (estado.Get_Casillas_Libres()==0)
            return 0;  // Hay un empate global y se ha rellenado completamente el tablero
    else
          return Puntuacion(jugador,estado);
}

// ------------------- Los tres metodos anteriores no se pueden modificar

int Hor(int jugador, const Environment &estado){
   int valor=0, consecutivas=0, boom, adv,boom_adv;
   if(jugador==1){
      boom=4;
      adv=2;
      boom_adv=5;
   }
   else{
      boom=5;
      adv=1;
      boom_adv=4;
   }
   for(int i=0;i<7;i++){
      for(int j=0;j<7;j++){
         if(estado.See_Casilla(i,j)==jugador || estado.See_Casilla(i,j)==boom){
            consecutivas++;
            if(consecutivas == 1){
               valor += 5;
            }
            else if(consecutivas == 2){
               valor += 10;
            }
            else if(consecutivas == 3){
               valor += 15;
            }
         }
         else{
            consecutivas=0;
            if(consecutivas == 1){
               valor -= 5;
            }
            else if(consecutivas == 2){
               valor -= 10;
            }
            else if(consecutivas == 3){
               valor -= 15;
            }
         }
      }
   }
   return valor;
}
int Ver(int jugador, const Environment &estado){
   int valor=0, consecutivas=0, boom, adv,boom_adv;
   if(jugador==1){
      boom=4;
      adv=2;
      boom_adv=5;
   }
   else{
      boom=5;
      adv=1;
      boom_adv=4;
   }
   for(int i=0;i<7;i++){
      for(int j=0;j<7;j++){
         if(estado.See_Casilla(j,i)==jugador || estado.See_Casilla(j,i)==boom){
            consecutivas++;
            if(consecutivas == 1){
               valor += 5;
            }
            else if(consecutivas == 2){
               valor += 10;
            }
            else if(consecutivas == 3){
               valor += 15;
            }
         }
         else{
            consecutivas=0;
            if(consecutivas == 1){
               valor -= 5;
            }
            else if(consecutivas == 2){
               valor -= 10;
            }
            else if(consecutivas == 3){
               valor -= 15;
            }
         }
      }
   }
   return valor;
}

int CompruebaDiagonal(int jugador, const Environment &estado){
   int puntuacion = 0, seguidas = 0, bomba, enemigo, bomba_enemigo;
    if(jugador == 1){
        bomba = 4;
        enemigo = 2;
        bomba_enemigo = 5;
    }
    else{
        bomba = 5;
        enemigo = 1;
        bomba_enemigo = 4;
    }

    // Derecha-abajo / izquierda-arriba.
    for(int i = 0; i < 4; i++) {
        for(int j = 3; j < 7; j++) {
            for(int k = 0; k < 4; k++) {
                if(estado.See_Casilla(i+k,j-k) == jugador || estado.See_Casilla(i+k,j-k) == bomba) {
                    seguidas++;
                    if(seguidas == 2)
                        puntuacion = puntuacion + 2 * seguidas;
                    else if(seguidas == 3)
                        puntuacion = puntuacion + 4 * seguidas;
                    else
                        puntuacion += seguidas;
                }
                else if(estado.See_Casilla(i+k,j-k) == enemigo || estado.See_Casilla(i+k,j-k) == bomba_enemigo){
                    if(seguidas == 2)
                        puntuacion = puntuacion - 2 * seguidas;
                    else if(seguidas == 3)
                        puntuacion = puntuacion - 4 * seguidas;
                    else
                        puntuacion -= seguidas;
                    seguidas = 0;
                }
                else {
                    seguidas = 0;
                }
            }
            seguidas = 0;
        }
    }

    // Derecha-arriba / izquierda-abajo.
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            for(int k = 0; k < 4; k++) {
                if(estado.See_Casilla(j+k,i+k) == jugador || estado.See_Casilla(j+k,i+k) == bomba) {
                    seguidas++;
                    if(seguidas == 2)
                        puntuacion = puntuacion + 2 * seguidas;
                    else if(seguidas == 3)
                        puntuacion = puntuacion + 4 * seguidas;
                    else
                        puntuacion += seguidas;
                }
                else if(estado.See_Casilla(j+k,i+k) == enemigo || estado.See_Casilla(j+k,i+k) == bomba_enemigo){
                    if(seguidas == 2)
                        puntuacion = puntuacion - 2 * seguidas;
                    else if(seguidas == 3)
                        puntuacion = puntuacion - 4 * seguidas;
                    else
                        puntuacion -= seguidas;
                    seguidas = 0;
                }
                else {
                    seguidas = 0;
                }
            }
            seguidas = 0;
        }
    }

    return puntuacion;
}

double MiHeuristica(int jugador,const Environment &estado){

   int adv,puntosAdv=0,puntosJug = 0;
    if(jugador == 1)
      adv = 2;
    else
      adv = 1;

      puntosAdv += Hor(adv,estado);
      puntosAdv += Ver(adv,estado);
      puntosAdv += CompruebaDiagonal(adv,estado);

      puntosJug += Hor(jugador,estado);
      puntosJug += Ver(jugador,estado);
      puntosJug += CompruebaDiagonal(jugador,estado);
      return (double) (puntosAdv - puntosJug );

}


// Funcion heuristica (ESTA ES LA QUE TENEIS QUE MODIFICAR)
double Valoracion(const Environment &estado, int jugador){
   int ganador = estado.RevisarTablero();

   if (ganador==jugador)
      return 99999999.0; // Gana el jugador que pide la valoracion
   else if (ganador!=0)
      return -99999999.0; // Pierde el jugador que pide la valoracion
   else if (estado.Get_Casillas_Libres()==0)
      return 0;  // Hay un empate global y se ha rellenado completamente el tablero
   else {
      //Si no he ganado ni perdido ni empatado aplicamos la heurística
      return MiHeuristica(jugador,estado);
   }
}







// Esta funcion no se puede usar en la version entregable
// Aparece aqui solo para ILUSTRAR el comportamiento del juego
// ESTO NO IMPLEMENTA NI MINIMAX, NI PODA ALFABETA
void JuegoAleatorio(bool aplicables[], int opciones[], int &j){
    j=0;
    for (int i=0; i<8; i++){
        if (aplicables[i]){
           opciones[j]=i;
           j++;
        }
    }
}






// Invoca el siguiente movimiento del jugador
Environment::ActionType Player::Think(){
    const int PROFUNDIDAD_MINIMAX = 6;  // Umbral maximo de profundidad para el metodo MiniMax
    const int PROFUNDIDAD_ALFABETA = 8; // Umbral maximo de profundidad para la poda Alfa_Beta

    Environment::ActionType accion; // acci�n que se va a devolver
    bool aplicables[8]; // Vector bool usado para obtener las acciones que son aplicables en el estado actual. La interpretacion es
                        // aplicables[0]==true si PUT1 es aplicable
                        // aplicables[1]==true si PUT2 es aplicable
                        // aplicables[2]==true si PUT3 es aplicable
                        // aplicables[3]==true si PUT4 es aplicable
                        // aplicables[4]==true si PUT5 es aplicable
                        // aplicables[5]==true si PUT6 es aplicable
                        // aplicables[6]==true si PUT7 es aplicable
                        // aplicables[7]==true si BOOM es aplicable



    double valor; // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alpha, beta; // Cotas de la poda AlfaBeta

    int n_act,numNodos = 0; //Acciones posibles en el estado actual


    n_act = actual_.possible_actions(aplicables); // Obtengo las acciones aplicables al estado actual en "aplicables"
    int opciones[10];

    // Muestra por la consola las acciones aplicable para el jugador activo
    //actual_.PintaTablero();
    cout << " Acciones aplicables ";
    (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
    for (int t=0; t<8; t++)
      if (aplicables[t])
         cout << " " << actual_.ActionStr( static_cast< Environment::ActionType > (t)  );
    cout << endl;

   alpha = menosinf;
   beta = masinf;
   valor = alphabeta(actual_, jugador_, PROFUNDIDAD_ALFABETA, alpha, beta, accion,numNodos);
   cout << "Valor: " << valor << "  Accion elegida: " << actual_.ActionStr(accion) << " Nodos evaludos: " << numNodos << endl;

    // Opcion: Poda AlfaBeta
    // NOTA: La parametrizacion es solo orientativa
    // valor = Poda_AlfaBeta(actual_, jugador_, 0, PROFUNDIDAD_ALFABETA, accion, alpha, beta);
    //cout << "Valor MiniMax: " << valor << "  Accion: " << actual_.ActionStr(accion) << endl;

    return accion;
}
    //--------------------- COMENTAR Desde aqui
   //  cout << "\n\t";
   //  int n_opciones=0;
   //  JuegoAleatorio(aplicables, opciones, n_opciones);

   //  if (n_act==0){
   //    (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
   //    cout << " No puede realizar ninguna accion!!!\n";
   //    //accion = Environment::actIDLE;
   //  }
   //  else if (n_act==1){
   //         (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
   //          cout << " Solo se puede realizar la accion "
   //               << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[0])  ) << endl;
   //          accion = static_cast< Environment::ActionType > (opciones[0]);

   //       }
   //       else { // Hay que elegir entre varias posibles acciones
   //          int aleatorio = rand()%n_opciones;
   //          cout << " -> " << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[aleatorio])  ) << endl;
   //          accion = static_cast< Environment::ActionType > (opciones[aleatorio]);
   //       }
   
    //--------------------- COMENTAR Hasta aqui
   

    //--------------------- AQUI EMPIEZA LA PARTE A REALIZAR POR EL ALUMNO ------------------------------------------------

double Player::alphabeta(const Environment & T, int jugador, int prof, double alpha, double beta, Environment::ActionType & accion, int & numNodos){
   bool acciones[8];
   double value;
   int last_act = -1;
   
    if (prof == 0 || T.JuegoTerminado()){
        numNodos++;
        return Valoracion(T, jugador);
    }

    
    Environment::ActionType acc_ant;
    Environment hijo = T.GenerateNextMove(last_act);
    int nHijos = T.possible_actions(acciones);

    if (jugador == 1) {
        for (int i = 0; i < nHijos; i++){
            value = alphabeta(hijo, 0, prof - 1, alpha, beta, acc_ant, numNodos);
            if (value > alpha){
                alpha = value;
                accion = static_cast <Environment::ActionType > (last_act);
            }
            //Si la beta es menor o igual al alpha "podamos" es decir cortamos la recursividad
            if (beta <= alpha)
                break;

            hijo = T.GenerateNextMove(last_act);
        }
        return alpha;
    }
    else{
        for (int i = 0; i < nHijos; i++){
            value = alphabeta(hijo, 1, prof - 1, alpha, beta, acc_ant, numNodos);
            if (value < beta) {
               beta = value;
               accion = static_cast <Environment::ActionType > (last_act);
            }
            //Si la beta es menor o igual al alpha "podamos" es decir cortamos la recursividad
            if (beta <= alpha)
                break;

            hijo = T.GenerateNextMove(last_act);
        }
        return beta;
    }
}
