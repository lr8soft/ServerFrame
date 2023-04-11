#include "Server.h"
int main() {
    AsioService service;
    Server server(service);
    server.init();
    server.work();

    service.run();
    return 0;
}
