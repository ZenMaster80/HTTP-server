// HTTP-server.h : Include file for standard system include files,
// or project specific include files.
#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H
#include <winsock2.h>

#pragma once

class HttpServer
{
public:
	//Konstruktori (oletusportti = 8080)
	HttpServer(int port = 8080);
	//Destruktori
	~HttpServer();
	//Julkiset metodit
	bool init();
	void start();

private:
	int m_port;
	SOCKET m_listenSocket;
	SOCKET m_clientSocket;

	//Sisäiset apumetodit
	void handleRequest();
	void sendResponse();
	void cleanup();
};

#endif // HTTP_SERVER_H
// TODO: Reference additional headers your program requires here.
