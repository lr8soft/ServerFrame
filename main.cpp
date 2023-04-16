#include "Server.h"

int main() {
    AsioService service;
    Server server(service);
    server.init();
    server.start();
    return 0;
}
