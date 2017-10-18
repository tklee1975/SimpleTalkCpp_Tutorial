#pragma once

#include <cstdlib>
#include <new>
#include <stdio.h>

class NonCopyable {
public:
	NonCopyable() {}

private:
	NonCopyable(const NonCopyable&) = delete;
	void operator=(const NonCopyable&) = delete;
};

class MyError : public std::exception {
public:
	MyError(const char* msg) {
		printf("\nMyError %s\n", msg);
	}
};

template<typename T>
class MyVector : public NonCopyable {
public:
	MyVector() {}

		// no inBound check
	      T& unsafeAt(int i)       { return m_data[i]; }
	const T& unsafeAt(int i) const { return m_data[i]; }

	      T& at(int i)       { inBoundCheck(i); return m_data[i]; }
	const T& at(int i) const { inBoundCheck(i); return m_data[i]; }

	      T& operator[](int i)       { return at(i); }
	const T& operator[](int i) const { return at(i); }

		  T& back()					{ return at(m_size -1); }
	const T& back() const			{ return at(m_size -1); }

		  T& back(int i)			{ return at(m_size -i -1); }
	const T& back(int i) const		{ return at(m_size -i -1); }

	bool inBound(int i) const		{ return i >= 0 && i < m_size; }

	void append(const T& v);
	void append(T && v);

	template<typename... ARGS>
	void emplace_back(ARGS&&... args);

	int  size		() const { return m_size; }
	void resize		(int newSize);
	void reserve	(int n);
	int	 capacity	() const { return m_capacity; }

private:
	void inBoundCheck(int i) const {
		if (!inBound(i)) {
			throw MyError("OutOfBound");
		}
	}

	T*	m_data = nullptr;
	int	m_size = 0;
	int	m_capacity = 0;
};

template<typename T>
template<typename... ARGS>
void MyVector<T>::emplace_back(ARGS&&... args) {
	reserve(m_size + 1);
	new (m_data + m_size) T(std::forward<ARGS>(args)...);
	m_size++;
}

template<typename T> inline
void MyVector<T>::append(const T& v) {
	reserve(m_size + 1);
	new (m_data + m_size) T(v);
	m_size++;
}

template<typename T> inline
void MyVector<T>::append(T && v) {
	reserve(m_size + 1);
	new (m_data + m_size) T(std::forward<T>(v));
	m_size++;
}

template<typename T> inline
void MyVector<T>::reserve(int n)
{
	if (m_capacity >= n)
		return;

	auto newCapacity = m_capacity + m_capacity / 2; // try to grow 1.5x
	if (newCapacity < n) {
		newCapacity = n;
	}

	auto* newData = reinterpret_cast<T*>(std::malloc(newCapacity * sizeof(T)));

	try {
		auto* src = m_data;
		auto* end = m_data + m_size;
		auto* dst = newData;

		for (; src < end; src++, dst++) {
			new(dst) T(std::move(*src)); //placement new
			src->~T();
		}
	}
	catch (...) {
		std::free(newData);
		throw;
	}

	m_data = newData;
	m_capacity = newCapacity;
}

template<typename T> inline
void MyVector<T>::resize(int newSize)
{
	if (m_size == newSize)
		return;
	reserve(newSize);

	if (newSize > m_size) {
		auto* dst = m_data + m_size;
		auto* end = m_data + newSize;
		for (; dst < end; dst++) {
			new (dst) T();
		}
	}

	m_size = newSize;
}
