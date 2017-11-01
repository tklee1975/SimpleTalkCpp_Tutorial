#include "MyServer.h"
#include "MyFileStream.h"

void MyClient::onConnected()
{
	printf("> client %p: connected\n", this);
}

void MyClient::onRecv()
{
	{
		size_t n = sock.availableBytesToRead();
		if (!n) {
			close();
			return;
		}

		// append into recvBuf
		auto oldSize = recvBuf.size();
		auto newSize = oldSize + n;

		// !!! limit the recv buf size, otherwise may attack by sending data without header end !!!
		if (newSize > kHeaderSizeLimit) {
			throw MyError("Excess header size limit");
		}

		recvBuf.resize(newSize);
		auto* p = &*(recvBuf.begin() + oldSize);
		sock.recv(p, n);
	}

	// find header
	auto* header = &(*recvBuf.begin());
	auto* headerEnd = strstr(header, "\r\n\r\n");
	if (!headerEnd)
		return;

	*headerEnd = 0;

	onRecvHeader(header);

	auto headerSize = headerEnd + 4 - header;
	auto remainSize = recvBuf.size() - headerSize;
	memmove(header, headerEnd + 4, remainSize);
	recvBuf.resize(remainSize);
}

void MyClient::onRecvHeader(const char* header)
{
	responseBuf.clear();
	responseContent.clear();

	printf("> client %p: recv header\n%s\n\n", this, header);

	auto* nextLine = header;
	nextLine = MyStrUtil::getLine(lineBuf, nextLine);

	auto* input = lineBuf.c_str();
	input = MyStrUtil::getUpperToken(token, input, ' ');

	if (token == "GET") {
		input = MyStrUtil::getToken(token, input, ' ');

		std::string filename;

		{ // get absolute file path
			std::string tmp = server->documentRoot();
			tmp.append(token);
			MyFilePath::getAbsPath(filename, tmp.c_str());
		}

		// !!! avoid access file out of document root !!!
		if (!MyStrUtil::startsWith(filename.c_str(), server->documentRoot().c_str())) {
			sendResponse(401, "401 - Unauthorized");
		}

		printf("GET filename = [%s]\n", filename.c_str());

		try {
			MyFileStream file;
			file.openRead(filename.c_str());
	
			int status = 200;
			const size_t tmpSize = 1024;
			char tmp[tmpSize + 1];
			int n = snprintf(tmp, tmpSize,	"HTTP/1.1 %d \r\n"
											"Content-Length: %llu\r\n\r\n",
											status,
											file.fileSize());
			responseBuf.append(tmp);

			file.read(responseContent, (size_t) file.fileSize()); // !!! size_t can be 32 bit
			send(responseBuf);
			sock.send(responseContent.data(), responseContent.size());
			
		}catch(...) {
			sendResponse(404, "404 - File Not Found");
			return;
		}
	}
	else if (token == "POST") {
	}

	sendResponse(400, "400 - Bad Request");
}

void MyClient::beginResponse(int status)
{
	const size_t tmpSize = 1024;
	char tmp[tmpSize + 1];
	int n = snprintf(tmp, tmpSize, "HTTP/1.1 %d \r\n", status);
	if (n < 0) {
		throw MyError("makeResponseHeader");
	}
	tmp[tmpSize] = 0;
	responseBuf = tmp;
}

void MyClient::endResponse(const char* content, uint64_t contentLength)
{
	const size_t tmpSize = 1024;
	char tmp[tmpSize + 1];
	int n = snprintf(tmp, tmpSize, "Content-Length: %llu\r\n\r\n", contentLength);
	if (n < 0) {
		throw MyError("makeResponseHeader");
	}
	tmp[tmpSize] = 0;
	responseBuf.append(tmp);
	responseBuf.append(content, content + contentLength);
}

void MyClient::sendResponse(int status, const char* content, uint64_t contentLength)
{
	beginResponse(status);
	endResponse(content, contentLength);
	send(responseBuf.c_str(), responseBuf.size());
}

void MyClient::close()
{
	printf("> client %p: close\n", this);
	sock.close();
}

void MyClient::send(const char* s, size_t n)
{
	printf("> client %p: send\n", this);
	fwrite(s, n, 1, stdout);
	printf("\n\n");

	sock.send(s, n);
}

