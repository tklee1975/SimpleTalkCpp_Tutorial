#include "../MyBufSocket.h"

bool g_quit = false;

class MyServer : public MyNonCopyable {
public:
	class Client : public MyBufSocket {
	public:
	};

	void run() {
		_listenSock.createTCP();
		_listenSock.setNonBlocking(true);

		MySocketAddr addr;
		addr.setIPv4(0,0,0,0);
		addr.setPort(3300);
		_listenSock.bind(addr);
		_listenSock.listen();
		printf("listening ...\n");

		while (!g_quit) {
			auto n = _clients.size();
			_pollfds.resize(n + 1);

			{
				for (size_t i = 0; i < _clients.size(); i++) {
					auto& c = _clients[i];
					if (!c->isValid()) {
						if (_clients.size() > 1)
							std::swap(c, _clients.back());
						_clients.resize(_clients.size() - 1);
						i--;
						continue;
					}
					c->getPollFD(_pollfds[i]);
				}
			}

			_pollfds[n].reset(_listenSock, true, false);

			int ret = my_poll(_pollfds.data(), n + 1, 1000);

			{
				int i = 0;
				for (auto& c : _clients) {
					try {
						c->onUpdate(_pollfds[i]);
					} catch(...) {
						c->close();
					}
					i++;
				}
			}

			if (_pollfds[n].canRead()) {
				_clients.emplace_back(new Client);
				auto& newClient = _clients.back();
				newClient->acceptFromListenSock(_listenSock);
				printf("accepted\n");
			}
		}		
	}

	MySocket _listenSock;
	std::vector< std::unique_ptr<Client> > _clients;
	std::vector< MyPollFD > _pollfds;
};

void my_singal_handler(int sig) {
	printf("my_singal_handler %d\n", sig);
	switch (sig) {
		case SIGINT:
		case SIGTERM: {
			g_quit = true;
		}break;
	}
}

int main(int argv, const char* argc[]) {
	signal(SIGTERM, my_singal_handler); // kill process
	signal(SIGINT,  my_singal_handler); // Ctrl + C

	MyServer server;
	server.run();
	return 0;
}

