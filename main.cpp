#include "Server.h"
int main() {
    AsioService service;
    Server server(service);

    service.run();
    return 0;
}
