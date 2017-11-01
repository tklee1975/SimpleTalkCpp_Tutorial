#include "MySocket.h"
#include "MyFilePath.h"

class MyServer;
class MyClient : public MyNonCopyable {
public:
	const size_t kHeaderSizeLimit = 16 * 1024;

	void onConnected();

	void onRecv();
	void onRecvHeader(const char* header);

	void beginResponse(int status);
	void endResponse(const char* content, uint64_t contentLength);

	template<size_t N>
	void endResponse(const char (&content)[N]) {
		if (N) endResponse(content, N-1);
	}

	void sendResponse(int status, const char* content, uint64_t contentLength);

	template<size_t N>
	void sendResponse(int status, const char (&content)[N]) {
		if (N) sendResponse(status, content, N-1);
	}

	void close();

	void send(const char* s, size_t n);

	template<size_t N>
	void send(const char (&sz)[N]) { if (N) send(sz, N-1); }
	void send(const std::string& s) { send(s.c_str(), s.size()); }

	MySocket sock;

	MyServer* server = nullptr;

	std::string recvBuf;
	std::string lineBuf;
	std::string token;
	std::string responseBuf;
	std::vector<char> responseContent;
};
