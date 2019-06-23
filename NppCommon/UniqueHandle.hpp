#pragma once

#include <Windows.h>

template<class HandleCloser = BOOL(*)(HANDLE)>
class UniqueHandle final
{
public:
	UniqueHandle(const UniqueHandle&) = delete;
	UniqueHandle& operator = (const UniqueHandle&) = delete;

	UniqueHandle(UniqueHandle&& that) : m_handle(that.release()), m_handle_closer(that.m_handle_closer) {
		that.release();
	}
	UniqueHandle& operator = (UniqueHandle&& that) {
		reset();
		m_handle = that.release();
		m_handle_closer = that.m_handle_closer;
	
		return *this;
	}

	UniqueHandle& operator = (HANDLE handle) {
		reset();
		m_handle = handle;
		return *this;
	}

	UniqueHandle() : UniqueHandle(INVALID_HANDLE_VALUE) {}

	UniqueHandle(HANDLE handle, HandleCloser handle_closer = &CloseHandle) : m_handle(handle), m_handle_closer(handle_closer) {
	}

	~UniqueHandle() {
		reset();
	}

	void reset() {
		if (*this) {
			m_handle_closer(m_handle);
			release();
		}
	}

	HANDLE release() {
		const HANDLE handle = m_handle;
		m_handle = INVALID_HANDLE_VALUE;
		return handle;
	}

	explicit operator bool() const {
		return m_handle != INVALID_HANDLE_VALUE;
	}

	HANDLE get() const {
		return m_handle;
	}

	HANDLE operator*() const {
		return get();
	}

private:

	HANDLE m_handle;
	HandleCloser m_handle_closer;
};

template<class HC1, class HC2>
bool operator == (const UniqueHandle<HC1>& lhs, const UniqueHandle<HC2>& rhs) {
	return *lhs == *rhs;
}

template<class HC1>
bool operator == (const UniqueHandle<HC1>& lhs, const HANDLE rhs) {
	return *lhs == rhs;
}

template<class HC2>
bool operator == (const HANDLE lhs, const UniqueHandle<HC2>& rhs) {
	return lhs == *rhs;
}

template<class HC1, class HC2>
bool operator != (const UniqueHandle<HC1>& lhs, const UniqueHandle<HC2>& rhs) {
	return !(lhs == rhs);
}

template<class HC1>
bool operator != (const UniqueHandle<HC1>& lhs, const HANDLE rhs) {
	return !(lhs == rhs);
}

template<class HC2>
bool operator != (const HANDLE lhs, const UniqueHandle<HC2>& rhs) {
	return !(lhs == rhs);
}
