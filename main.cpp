#include "Listener.h"
#include "ListenerManager.h"
int main() {
    auto mgr = ListenerManager::getInstance();
    mgr->init();
    mgr->start();
    return 0;
}
