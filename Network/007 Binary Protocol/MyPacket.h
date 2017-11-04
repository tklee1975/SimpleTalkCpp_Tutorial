#pragma once
#include "MySerializer.h"

enum class MyPacketType : uint16_t;

class MyPacket : public MyNonCopyable {
public:
	using Type = MyPacketType;

	static const uint16_t kMaxPacketSize = 32 * 1024;

	struct Header {
		uint32_t packetSize = 0;
		Type	 type       = (Type)0;
	};
	Header header;

	template<typename SE>
	void io(SE& se) {
	}
};

enum class MyPacketType : uint16_t {
	None,
	Hello,
	Chat,
	Chat2,
};

class MyPacket_Hello : public MyPacket {
	using Base = MyPacket;
public:
	MyPacket_Hello() {
		header.type = Type::Hello;
	}

	uint32_t version;

	template<typename SE>
	void io(SE& se) {
		Base::io(se);
		se.io(version);
	}
};

class MyPacket_Chat : public MyPacket {
	using Base = MyPacket;
public:
	MyPacket_Chat() {
		header.type = Type::Chat;
	}

	std::string msg;
	template<typename SE>
	void io(SE& se) {
		Base::io(se);
		se.io(msg);
	}
};

class MyPacket_Chat2 : public MyPacket_Chat {
	using Base = MyPacket_Chat;
public:
	MyPacket_Chat2() {
		header.type = Type::Chat2;
	}

	std::vector< std::string > extra;
	template<typename SE>
	void io(SE& se) {
		Base::io(se);
		se.io(extra);
	}
};