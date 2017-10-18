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

//template<typename T> inline
//bool isRawCopyable() {
//	return false;
//}

template<typename T>
struct isRawCopyable {
	static const bool value = std::is_pod<T>::value;
};

template<typename T>
void my_memcpy(T* dst, const T* src, int n) {
	if (n * sizeof(T) < 128) {
		for () {
		}
	}else{
		memcpy(dst, src, n * sizeof(T));
	}
}

template<typename T>
class MyVector : public NonCopyable {
public:
	MyVector() {}
	MyVector(MyVector && r)			{ operator=(std::move(r)); }
	~MyVector() { clearAndFree();}

	void operator=(MyVector && r);

	// TODO
//	MyVector(const MyVector& r)		{ operator=(r); }
//	void operator=(const MyVector& r);

//	bool operator==(const MyVector& r) {
//	}

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

	void clear();
	void clearAndFree();

	bool inBound(int i) const		{ return i >= 0 && i < m_size; }

	void append(const T& v);
	void append(T && v);

	template<typename... ARGS>
	void emplace_back(ARGS&&... args);

	void removeAt(int i); // TODO
	void removeAndSwapLast(); // TODO

	int  size		() const { return m_size; }

	template<typename... ARGS>
	void resize		(int newSize, ARGS&&... args);

	void reserve	(int n);
	int	 capacity	() const { return m_capacity; }

			T* begin()				{ return m_data; }
	const	T* begin() const		{ return m_data; }

			T* end	()				{ return m_data + m_size; }
	const	T* end  () const		{ return m_data + m_size; }

	template<typename A>
	class RevIterator_ {
	public:
		RevIterator_(A* p) : m_p(p) {}

		A& operator*() { return *m_p; }
		bool operator==(const RevIterator_& r) const { return m_p == r.m_p; }
		bool operator!=(const RevIterator_& r) const { return m_p != r.m_p; }

		void operator++() { --m_p; } // <----- reverse order

	private:
		A* m_p;
	};

	using RevIterator  = RevIterator_<T>;
	using CRevIterator = RevIterator_<const T>;

	RevIterator		rbegin()		{ return m_data + m_size - 1; }
	CRevIterator	rbegin() const	{ return m_data + m_size - 1; }

	RevIterator		rend()			{ return m_data - 1; }
	CRevIterator	rend() const	{ return m_data - 1; }

	template<typename A>
	class RevEnumerator_ {
	public:
		RevEnumerator_(RevIterator_<A> begin, RevIterator_<A> end) 
			: m_begin(begin)
			, m_end(end)
		{
		}

		RevIterator_<A> begin()	{ return m_begin; }
		RevIterator_<A> end()	{ return m_end; }

	private:
		RevIterator_<A> m_begin;
		RevIterator_<A> m_end;
	};

	using RevEnumerator  = RevEnumerator_<T>;
	using CRevEnumerator = RevEnumerator_<const T>;

	RevEnumerator  revEach()		{ return RevEnumerator( rbegin(), rend()); }
	CRevEnumerator revEach() const	{ return CRevEnumerator(rbegin(), rend()); }

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
void MyVector<T>::clear() {
	auto* dst = m_data;
	auto* end = m_data + m_size;

	for (; dst < end; dst++) {
		dst->~T();
	}
	m_size = 0;
}

template<typename T>
void MyVector<T>::operator=(MyVector && r) {
	clearAndFree();
	m_data = r.m_data;
	m_size = r.m_size;
	m_capacity = r.m_capacity;

	r.m_data = nullptr;
	r.m_size = 0;
	r.m_capacity = 0;
}

template<typename T> inline
void MyVector<T>::clearAndFree() {
	clear();
	if (m_data) {
		std::free(m_data);
		m_capacity = 0;
	}
}

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

		if (isRawCopyable<T>::value) {
			memcpy(dst, src, sizeof(T) * m_size);
		} else {
			for (; src < end; src++, dst++) {
				new(dst) T(std::move(*src)); //placement new
				src->~T();
			}
		}
	}
	catch (...) {
		std::free(newData);
		throw;
	}

	m_data = newData;
	m_capacity = newCapacity;
}

template<typename T> 
template<typename... ARGS> inline
void MyVector<T>::resize(int newSize, ARGS&&... args)
{
	if (m_size == newSize)
		return;
	reserve(newSize);

	if (newSize > m_size) {
		// call ctor
		auto* dst = m_data + m_size;
		auto* end = m_data + newSize;
		for (; dst < end; dst++) {
			new (dst) T(std::forward<ARGS>(args)...);
		}
	} else if (newSize < m_size) {
		// call dtor
		auto* dst = m_data + newSize;
		auto* end = m_data + m_size;
		for (; dst < end; dst++) {
			dst->~T();
		}
	}

	m_size = newSize;
}
