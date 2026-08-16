#include "HTTP-server.h"
#include <iostream>
#include <ws2tcpip.h>
#include <vector>
#include <string>

// Konstruktori: Alustetaan luokan omat muuttujat turvallisilla oletusarvoilla
HttpServer::HttpServer(int port)
    : m_port(port), m_listenSocket(INVALID_SOCKET), m_clientSocket(INVALID_SOCKET) {}

// Destruktori: Huolehtii, että resurssit vapautetaan aina kun olio tuhoutuu
HttpServer::~HttpServer() {
    cleanup();
}

// METODI 1: ALUSTUS JA BIND
bool HttpServer::init()
{
    // 1. ALUSTUS
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0)
    {
        std::cerr << "Winsocketin alustus epäonnistui! Virhekoodi: " << result << std::endl;
        return false; // Palautetaan false virheen merkiksi
    }
    std::cout << "Winsocket alustettu onnistuneesti!\n";

    // 2. SOCKETIN LUONTI 
    m_listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_listenSocket == INVALID_SOCKET)
    {
        std::cerr << "Socketin luominen epäonnistui! Virhe: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return false;
    }
    std::cout << "Socketin luominen onnistui\n";

    // 3. OSOITTEEN ASETUKSET JA BIND
    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(m_port); // Käytetään luokan omaa m_port muuttujaa (esim. 8080)
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(m_listenSocket, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        std::cerr << "Bind epäonnistui! Virhe: " << WSAGetLastError() << std::endl;
        closesocket(m_listenSocket);
        m_listenSocket = INVALID_SOCKET;
        WSACleanup();
        return false;
    }
    std::cout << "Bind onnistui porttiin " << m_port << "\n";

    return true; // Kaikki onnistui!
}

// METODI 2: KUUNTELU JA HYVÄKSYNTÄ
void HttpServer::start()
{
    // 4. KUUNTELU (LISTEN)
        if (listen(m_listenSocket, 5) == SOCKET_ERROR)
        {
            std::cerr << "Kuuntelu epäonnistui! Virhe: " << WSAGetLastError() << std::endl;
            return;
        }
        std::cout << "Palvelin odottaa yhteytta portissa " << m_port << "...\n" << std::endl;
        while (true)
        {
            // 5. HYVÄKSYNTÄ (ACCEPT) 
            m_clientSocket = accept(m_listenSocket, nullptr, nullptr);
            if (m_clientSocket == INVALID_SOCKET)
            {
                std::cerr << "Clientin luominen epäonnistui! Virhe: " << WSAGetLastError() << std::endl;
                return;
            }
            std::cout << "Asiakas yhdistetty!\n" << std::endl;

            // Kutsutaan apumetodeja hoitamaan datan lukeminen ja vastaaminen
            handleRequest();
            sendResponse();

            // Suljetaan tämän yksittäisen asiakkaan yhteys pyynnön jälkeen
            closesocket(m_clientSocket);
            m_clientSocket = INVALID_SOCKET;
            
        }//suljetaan while silmukka ja palataan odottamaan seuraavaa asiakasta
    }

    // METODI 3: DATAN LUKEMINEN
    void HttpServer::handleRequest()
    {
        // 6. DATAN LUKEMINEN (RECV)
        std::vector<char> buffer(1024, 0);
        int bytesReceived = recv(m_clientSocket, buffer.data(), 1023, 0);

        if (bytesReceived == SOCKET_ERROR)
        {
            std::cerr << "Datan lukeminen epäonnistui! Virhe: " << WSAGetLastError() << std::endl;
        }
        else
        {
            std::cout << "\n --- Saapunut HTTP-PYYNTÖ ---\n";
            std::cout << buffer.data() << std::endl;
            std::cout << "---------------------------------\n";
        }
    }

    // METODI 4: VASTAUKSEN LÄHETYS
    void HttpServer::sendResponse()
    {
        // 1. Määritetään pelkkä selaimeen tuleva sisältöteksti
        std::string body = "Hei Maailma!";

        // 2. Rakennetaan HTTP-otsikot ja liitetään body-tekstin pituus automaattisesti mukaan
        std::string response = "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain; charset=utf-8\r\n"
            "Content-Length: " + std::to_string(body.length()) + "\r\n"
            "Connection: close\r\n"
            "\r\n"
            + body;

        // 3. Lähetetään koko paketti selaimeen
        send(m_clientSocket, response.c_str(), static_cast<int>(response.length()), 0);
        std::cout << "Vastaus lahetetty asiakkaalle.\n";
    }

// METODI 5: LOPULLINEN SIIVOUS
void HttpServer::cleanup()
{
    // 8. SIIVOUS
    if (m_clientSocket != INVALID_SOCKET) {
        closesocket(m_clientSocket);
        m_clientSocket = INVALID_SOCKET;
    }
    if (m_listenSocket != INVALID_SOCKET) {
        closesocket(m_listenSocket);
        m_listenSocket = INVALID_SOCKET;
    }
    WSACleanup();
    std::cout << "Palvelimen resurssit siivottu.\n";
}

