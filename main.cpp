#include "Application.h"
int main() {
    Application app;
    app.init();
    app.work();
    app.finalize();
    return 0;
}
