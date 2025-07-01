#include "game.hpp"

// ---------------------------------------------------
// MAIN DEL JUEGO & OTRAS FUNCIONES
// ----------------------------------------------------------------------

int main() {
 while (true) {
        int result = runGame();
        if (result == 0) break;
    }

    return 0;
}