#include "MyServer.h"

bool MyServer::s_quit = false;

MyServer::MyServer()
{
	_clients.reserve(kClientCountMax);
}

void MyServer::run()
{
	MyUtil::getCurrentDir(_documentRoot);
	_documentRoot.append("/www");

	_listenSock.createTCP();

	MySocketAddr addr;
	addr.setIPv4(0, 0, 0, 0);
	addr.setPort(8011);

	_listenSock.bind(addr);
	_listenSock.listen();

	while (!s_quit) {
		fd_set readfds;
		FD_ZERO(&readfds);
		FD_SET(_listenSock.sock(), &readfds);
		int n = 1;

		removeClosedClients();

		for (auto& client : _clients) {
			if (!client || !client->sock.isValid())
				continue;

			if (n >= FD_SETSIZE) {
				throw MyError("n >= FD_SETSIZE");
				break;
			}

			FD_SET(client->sock.sock(), &readfds);
			n++;
		}

		struct timeval tv;
		tv.tv_sec = 1;
		tv.tv_usec = 0;

		int ret = ::select(n, &readfds, nullptr, nullptr, &tv);
		if (ret < 0) {
			throw MyError("select");
		}
		if (ret == 0) {
			continue;
		}

		if (FD_ISSET(_listenSock.sock(), &readfds)) {
			if (_clients.size() < kClientCountMax) {
				_clients.emplace_back(new MyClient());
				auto& client = _clients.back();
				client->server = this;

				try {
					_listenSock.accept(client->sock);
					client->onConnected();
				}
				catch (...) {
					client->close();
				}
			}
		}

		for (auto& client : _clients) {
			if (!client || !client->sock.isValid())
				continue;

			if (!FD_ISSET(client->sock.sock(), &readfds))
				continue;

			try {
				client->onRecv();
			}
			catch (...) {
				client->close();
			}
		}
	}
}

void MyServer::removeClosedClients()
{

	for (size_t i = 0; i < _clients.size();) {
		auto & c = _clients[i];
		if (c && c->sock.isValid()) {
			i++;
			continue;
		}

		try {
			std::swap(c, _clients.back());
			_clients.resize(_clients.size() - 1);
		}
		catch (...) {
			printf("exception in removeClosedClients");
		}
	}
}

