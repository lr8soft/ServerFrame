#include "Server.h"
int main() {
    AsioService service;
    Server server(service);
    server.start();
    return 0;
}
