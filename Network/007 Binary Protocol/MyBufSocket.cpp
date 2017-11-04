#include "MyBufSocket.h"

void MyBufSocket::connect(const char* hostname, uint16_t port)
{
	_sock.createTCP();
	_sock.setNonBlocking(true);
	if (_sock.connect(hostname, port)) {
		_onConnected();
		return;
	}
	_status = Status::Connecting;
	printf("connecting\n");
}

void MyBufSocket::_onConnected()
{
	_status = Status::Connected;
	printf("connected\n");
	onConnected();
}

void MyBufSocket::acceptFromListenSock(MySocket& listenSock)
{
	listenSock.accept(_sock);
	_pollfd.fd = _sock.sock();
	_onConnected();
}

void MyBufSocket::sendPacket(MyPacket& pkt)
{
	auto type = pkt.type();
	pkt.toBuffer(_tmp);

	printf("send packet %d\n", my_enum_to_int(type));

	//append to sendBuf
	_sendBuf.insert(_sendBuf.end(), _tmp.begin(), _tmp.end());
}

void MyBufSocket::_onRecv()
{
	auto bytesInSocket = _sock.availableBytesToRead();
	if (!bytesInSocket) {
		close();
		return;
	}

	const auto headerSize = sizeof(MyPacket::PacketSize) + sizeof(MyPacketType);

	// recv header
	if (_recvBuf.size() < headerSize) {
		auto bytesToRead = headerSize - _recvBuf.size();
		if (bytesInSocket < bytesToRead) {
			bytesToRead = bytesInSocket;
		}
		_sock.appendRecv(_recvBuf, bytesToRead);
		bytesInSocket -= bytesToRead;
	}

	// not enough for header
	if (_recvBuf.size() < headerSize)
		return;

	// recv body
	MyDeserializer de(_recvBuf);
	MyPacket::PacketSize packetSize;
	MyPacketType packetType;

	de.io_fixed(packetSize);
	de.io_fixed(my_enum_to_int(packetType));

	if (packetSize < headerSize)
		throw MyError("invalid header");

	auto bytesToRead = packetSize - headerSize;
	if (bytesInSocket < bytesToRead) {
		bytesToRead = bytesInSocket;
	}

	_sock.appendRecv(_recvBuf, bytesToRead);

	// not enough for body
	if (_recvBuf.size() < packetSize)
		return;

	assert(_recvBuf.size() == packetSize);

	printf("onRecvPacket %d\n", my_enum_to_int(packetType));
	onRecvPacket(packetType, _recvBuf);
	_recvBuf.clear();
}

void MyBufSocket::_onSend()
{
	if (_status == Status::Connecting) {
		_onConnected();
	}

	if (_sendBufOffset >= _sendBuf.size()) {
		_sendBufOffset = 0;
		_sendBuf.clear();
		return;
	}

	auto n = _sendBuf.size() - _sendBufOffset;
	auto ret = _sock.send(&_sendBuf[_sendBufOffset], n);
	if (ret <= 0) {
		close();
		return;
	}

	_sendBufOffset += ret;
}

void MyBufSocket::onUpdate(MyPollFD& fd)
{
	if (fd.canRead())
		_onRecv();
	if (fd.canWrite())
		_onSend();
}

void MyBufSocket::getPollFD(MyPollFD& pf)
{
	bool write = false;
	if (_status == Status::Connecting)
		write = true;
	if (_status == Status::Connected && _sendBuf.size())
		write = true;

	pf.reset(_sock, true, write);
}

void MyBufSocket::close()
{
	_status = Status::Closed;
	_sock.close();
	printf("closed\n");
}

bool MyBufSocket::isValid()
{
	return _sock.isValid();
}

