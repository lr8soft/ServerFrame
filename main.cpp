#include "Server.h"

int main() {
    Server server(false);
    server.init();
    server.start();
    return 0;
}
