#pragma once

#include <Windows.h>
#include <string>

class pipe
{
	HANDLE mPipe;

	std::wstring mName;
public:
	enum
	{
		MaxBufferSize = 1024 * 4
	};

	pipe();
	~pipe();

	bool create(const std::wstring &aName);
	bool createUnique();
	bool open(const std::wstring &aName);
	bool read(void *aBuffer, int aBufSize, DWORD &aReadSize);
	bool write(const void *aBuffer, int aBufSize);
	const std::wstring &getName() const;
	bool wait();

	void close();
};
