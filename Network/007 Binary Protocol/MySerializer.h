#pragma once

#include "my_common.h"

inline uint8_t my_byte_swap(uint8_t v) {
	return v;
}

inline uint16_t my_byte_swap(uint16_t v) {
	return ((v >> 8) & 0x00FF)
		 | ((v << 8) & 0xFF00);
}

inline uint32_t my_byte_swap(uint32_t v) {
	auto a = ((v >> 8 ) & 0x00FF00FFUL)
		   | ((v << 8 ) & 0xFF00FF00UL);

	return   ((a >> 16) & 0x0000FFFFUL)
		   | ((a << 16) & 0xFFFF0000UL);
}

inline uint64_t my_byte_swap(uint64_t v) {
	auto a = ((v >> 8 ) & 0x00FF00FF00FF00FFULL)
		   | ((v << 8 ) & 0xFF00FF00FF00FF00ULL);

	auto b = ((a >> 16) & 0x0000FFFF0000FFFFULL)
		   | ((a << 16) & 0xFFFF0000FFFF0000ULL);

	return   ((b >> 32) & 0x00000000FFFFFFFFULL)
		   | ((b << 32) & 0xFFFFFFFF00000000ULL);
}

#if 1 // byte swap needed

inline uint8_t  my_hton(uint8_t  v) { return my_byte_swap(v); }
inline uint16_t my_hton(uint16_t v) { return my_byte_swap(v); }
inline uint32_t my_hton(uint32_t v) { return my_byte_swap(v); }
inline uint64_t my_hton(uint64_t v) { return my_byte_swap(v); }

inline int8_t   my_hton(int8_t   v) { return (int8_t )my_byte_swap((uint8_t )v); }
inline int16_t  my_hton(int16_t  v) { return (int16_t)my_byte_swap((uint16_t)v); }
inline int32_t  my_hton(int32_t  v) { return (int32_t)my_byte_swap((uint32_t)v); }
inline int64_t  my_hton(int64_t  v) { return (int64_t)my_byte_swap((uint64_t)v); }

#else // byte swap not needed

inline uint8_t  my_hton(uint8_t  v) { return v; }
inline uint16_t my_hton(uint16_t v) { return v; }
inline uint32_t my_hton(uint32_t v) { return v; }
inline uint64_t my_hton(uint64_t v) { return v; }

inline int8_t   my_hton(int8_t   v) { return v; }
inline int16_t  my_hton(int16_t  v) { return v; }
inline int32_t  my_hton(int32_t  v) { return v; }
inline int64_t  my_hton(int64_t  v) { return v; }

#endif

inline float my_hton(float v) {
	auto t = my_hton(*(uint32_t*)&v);
	return *(float*)&t;
}

inline double my_hton(double v) {
	auto t = my_hton(*(uint64_t*)&v);
	return *(double*)&t;
}

class MySerializer {
public:
	MySerializer(std::vector<char>& buf)
		: _buf(buf)
	{
	}

	template<typename T>
	void _io_number(T& v) {
		_buf.resize(sizeof(v));
		auto offset = (_buf.size() - sizeof(v));
		*(T*)(&_buf[offset]) = my_hton(v);
	}

	void io(int8_t  & v) { _io_number(v); }
	void io(int16_t & v) { _io_number(v); }
	void io(int32_t & v) { _io_number(v); }
	void io(int64_t & v) { _io_number(v); }

	void io(uint8_t & v) { _io_number(v); }
	void io(uint16_t& v) { _io_number(v); }
	void io(uint32_t& v) { _io_number(v); }
	void io(uint64_t& v) { _io_number(v); }

	void io(float&  v) { _io_number(v); }
	void io(double& v) { _io_number(v); }

	std::vector<char>& _buf;
};

class MyDeserializer {
public:
	MyDeserializer(const std::vector<char>& buf)
		: _buf(buf)
	{
	}

	void io(uint16_t& v) {
	}

	const std::vector<char>& _buf;
};