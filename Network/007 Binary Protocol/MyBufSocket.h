#pragma once

#include "MySocket.h"
#include "MyPacket.h"

class MyBufSocket : public MyNonCopyable {
public:

	void connect(const char* hostname, uint16_t port) {
		_sock.createTCP();
		_sock.setNonBlocking(true);
		if (_sock.connect(hostname, port)) {
			_onConnected();
			return;
		}
		_status = Status::Connecting;
		printf("connecting\n");
	}

	void _onConnected() {
		_status = Status::Connected;
		printf("connected");
		onConnected();
	}

	virtual void onConnected() {}

	void acceptFromListenSock(MySocket& listenSock) {
		listenSock.accept(_sock);
		_pollfd.fd = _sock.sock();
		onConnected();
	}

	void onRecv() {
		auto n = _sock.availableBytesToRead();
		if (!n) {
			close();
			return;
		}
	}

	void onSend() {
		if (_status == Status::Connecting) {
			_onConnected();
		}
	}

	template<typename PACKET>
	void send(PACKET& p) {
		_tmp.resize(sizeof(MyPacket::header));
		MySerializer se(_tmp);
		p.io(se);

		auto s = _tmp.size();
		if (s > MyPacket::kMaxPacketSize)
			throw MyError("packet is too big");

		
		_tmp.data();
	}

	enum class Status {
		None,
		Connecting,
		Connected,
		Closed,
	};

	void onUpdate(MyPollFD& fd) {
		if (fd.canRead())
			onRecv();
		if (fd.canWrite())
			onSend();
	}

	void getPollFD(MyPollFD& pf) {
		bool write = false;
		if (_status == Status::Connecting)
			write = true;
		if (_status == Status::Connected && _sendBuf.size())
			write = true;

		pf.reset(_sock, true, write);
	}

	void close() {
		_status = Status::Closed;
		_sock.close();
		printf("closed\n");
	}

	bool isValid() { return _sock.isValid(); }

private:
	Status _status = Status::None;
	
	MyPollFD _pollfd;

	std::vector<char> _tmp;
	std::vector<char> _sendBuf;
	std::vector<char> _recvBuf;
	MySocket _sock;
};