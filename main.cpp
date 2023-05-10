#include "Server.h"

int main() {
    Server server(true);
    server.init();
    server.start();
    return 0;
}
