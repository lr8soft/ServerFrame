#include "Server.h"
int main() {
    Server* app = Server::getInstance();
    app->init();
    app->work();
    app->finalize();
    return 0;
}
