#include "MySocket.h"

class MyClient : public MyNonCopyable {
public:
	MySocket sock;
};

class MyServer : public MyNonCopyable {
public:

	const size_t kClientCountMax = 16;

	void run() {
		_listenSock.createTCP();

		MySocketAddr addr;
		addr.setIPv4(0,0,0,0);
		addr.setPort(2525);

		_listenSock.bind(addr);
		_listenSock.listen();

		for(;;) {
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
			tv.tv_sec = 3;
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
					//auto* client = new MyClient();
					//_clients.emplace_back(client);

					_clients.emplace_back(new MyClient());
					auto& client = _clients.back();

					_listenSock.accept(client->sock);
					onClientConnected(*client);
				}
			}

			for (auto& client : _clients) {
				if (!client || !client->sock.isValid())
					continue;

				if (!FD_ISSET(client->sock.sock(), &readfds)) 
					continue;
				onClientRead(*client);
			}
		}
	}

	void removeClosedClients() {		
		for (size_t i = 0; i < _clients.size();) {
			auto & c = _clients[i];
			if (c && c->sock.isValid()) {
				i++;
				continue;
			}

			std::swap(c, _clients.back());
			_clients.resize(_clients.size() - 1);
		}
	}

	void onClientConnected(MyClient& client) {
		printf("client %p: connected\n", &client);
		client.sock.send("220 MyServer ESMTP MyServer\r\n");
	}

	void onClientRead(MyClient& client) {
		size_t n = client.sock.availableBytesToRead();
		if (!n)
			return;

		std::vector<char> buf;
		client.sock.recv(buf, n);
		buf.push_back(0);

		printf("client %p: recv %s\n", &client, buf.data());

		if (0 == strncmp(buf.data(), "DATA", 4)) {
			client.sock.send("354 OK\r\n");
		}else{
			client.sock.send("250 OK\r\n");
		}
	}

private:
	MySocket _listenSock;
	std::vector< std::unique_ptr<MyClient> > _clients;
};



int main(int argv, const char* argc[]) {
	MyServer server;
	server.run();


	printf("\n======= Program Ended ======\n");
#ifdef _WIN32
	if (IsDebuggerPresent()) {
		printf("Press any key to Exit");
		_getch();
	}
#endif

	return 0;
}