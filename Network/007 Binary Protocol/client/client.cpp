#include "../MyBufSocket.h"

bool g_quit = false;

class MyClient : public MyBufSocket {
public:
	void update() {
		MyPollFD pf;
		getPollFD(pf);

		if (my_poll(&pf, 1, 100))
			onUpdate(pf);
	}

	virtual void onConnected() override {
		MyPacket_Hello pkt;
		pkt.version = 100;
		send(pkt);
	}
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
	
	if (argv < 2) {
		printf("client.exe <hostname>");
		return -1;
	}

	MyClient client;
	client.connect(argc[1], 3300);

	while (!g_quit) {
		client.update();
	}
}

