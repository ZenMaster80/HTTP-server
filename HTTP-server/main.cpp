#include "HTTP-server.h"
#include <iostream>

int main() {
    HttpServer server(8080);

    if (!server.init()) {
        std::cerr << "Palvelimen alustus epäonnistui!\n";
        return 1;
    }

    server.start();

    return 0;
};
