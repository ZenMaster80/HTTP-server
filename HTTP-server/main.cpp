#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>

int main() {
    // 1. ALUSTUS
    // Luodaan WSADATA-muuttuja ja kutsutaan WSAStartup-funktiota (pyydetään versiota 2.2).
    // Tarkistetaan, että palautusarvo on 0 (onnistui).
    WSADATA wsaData;

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (result != 0)
    {
        std::cerr << "Winsocketin alustus epäonnistui! Virhekoodi: " << result << std::endl;
        return 1; //Lopetetaan ohjelma virheeseen
    }
    else
    {
        std::cout << "Winsocket alustettu onnistuneesti!";
    }



    // 2. SOCKETIN LUONTI
    //  Luodaan SOCKET-tyyppinen muuttuja (esim. listenSocket) käyttäen socket()-funktiota.
    // Käytetään parametreja: AF_INET (IPv4), SOCK_STREAM (TCP) ja IPPROTO_TCP.
    // Tarkista, ettei palautusarvo ole INVALID_SOCKET.

    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (listenSocket == INVALID_SOCKET)
    {
        std::cerr << "Socketin luominen epäonnistui! Virhe: " << WSAGetLastError() << std::endl; //Listataan virhe
        WSACleanup();//Siivotaan vaiheessa 1 luotu alustus
        return 1; //Keskeytetään ohjelma
    }
    else
    {
        std::cout << "Socketin luominen onnistui" << std::endl;
    }

    // 3. OSOITTEEN ASETUKSET JA BIND
    // Luodaan sockaddr_in -rakenne. Asetetaan sille:
    sockaddr_in addr;
    //Nollataan rakenne
    std::memset(&addr, 0, sizeof(addr));
    // sin_family = AF_INET
    addr.sin_family = AF_INET;
    // sin_port = htons(8080)
    addr.sin_port = htons(8080);
    // sin_addr.s_addr = INADDR_ANY
    addr.sin_addr.s_addr = INADDR_ANY;
    // Kutsutaan bind()-funktiota ja tarkistetaan, ettei se palauta SOCKET_ERROR-virhettä.

    if (bind(listenSocket, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        std::cerr << "Bind epäonnistui! Virhe: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Bind onnistui porttiin 8080";

    // 4. KUUNTELU (LISTEN)
    // Kutsutaan listen()-funktiota luodulle socketille. Asetetaan jonon pituudeksi (backlog) esim. 5.
    if (listen(listenSocket, 5) == SOCKET_ERROR)
    {
        std::cerr << "Kuuntelu epäonnistui! Virhe: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }


    std::cout << "Palvelin odottaa yhteytta portissa 8080..." << std::endl;

    // 5. HYVÄKSYNTÄ (ACCEPT)
    // Luodaan uusi SOCKET (esim. clientSocket) ja kutsutaan accept()-funktiota.
    // Ohjelma pysähtyy tähän odottamaan selainta.

    SOCKET clientSocket; 
    clientSocket = accept(listenSocket, nullptr, nullptr);

    if (clientSocket == INVALID_SOCKET)
    {
        std::cerr << "Clientin luominen epäonnistui! Virhe: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }
    


    std::cout << "Asiakas yhditetty!" << std::endl;

    // 6. DATAN LUKEMINEN (RECV) - Vapaaehtoinen lisäys
    // Luodaan char-taulukko tai std::vector tiedon vastaanottoon.
       std::vector<char> buffer(1024, 0);
    // Kutsutaan recv()-funktiota että pystytään lukemaan selaimen lähettämä HTTP-pyyntö.
       int bytesReceived = recv(clientSocket, buffer.data(), 1023, 0);

       if (bytesReceived == SOCKET_ERROR)
       {
           std::cerr << "Datan lukeminen epäonnistui! Virhe: " << WSAGetLastError() << std::endl;
           closesocket(listenSocket);
           closesocket(clientSocket);
           WSACleanup();
           return 1;
       }
       else
       {
           std::cout << "\n --- Saapunut HTTP-PYYNTÖ ---\n";
           std::cout << buffer.data() << std::endl;
           std::cout << "---------------------------------";
       }



    // 7. VASTAUKSEN LÄHETYS (SEND) - Vapaaehtoinen lisäys
    // Luodaan std::string, joka sisältää HTTP-vastauksen (esim. "HTTP/1.1 200 OK\r\n\r\nHei maailma!").
       std::string response = "HTTP/1.1 200 OK\r\n"
           "Content-Type: text/plain; charset=utf-8\r\n"
           "Content-Length: 13\r\n"
           "Connection: close\r\n"
           "\r\n"
           "Hei Maailma!";
    // Kutsutaan send()-funktiota lähettääksesi sen clientSocketille.
       send(clientSocket, response.c_str(), static_cast<int>(response.length()), 0);


    // 8. SIIVOUS
    // Sulje socketit closesocket()-funktiolla.
    // Kutsu lopuksi WSACleanup().

    std::cout << "Asiakas yhditetty!" << std::endl;
    closesocket(clientSocket);
    closesocket(listenSocket);
    WSACleanup();

    return 0;
};