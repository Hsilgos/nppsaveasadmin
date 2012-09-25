#include "pipe.h"


#include <set>
//////////////////////////////////////////////////////////////////////////

std::set<std::wstring> &getIdMap()
{
	static std::set<std::wstring> tMap;
	return tMap;
}

bool isUnique(const std::wstring &aId)
{
	return getIdMap().find(aId) == getIdMap().end();
}

void freeId(const std::wstring &aId)
{
	std::set<std::wstring>::iterator it = getIdMap().find(aId);
	if( it != getIdMap().end() )
		getIdMap().erase(it);
}

std::wstring doGen(int aLen)
{
	static bool tInitialized = false;
	if(!tInitialized)
		srand(GetTickCount());

	const char random_map[] = "1234567890";

	std::wstring tResult;

	for( int i = 0; i < aLen; ++i )
		tResult.append(1, random_map[rand()*(sizeof(random_map) - 1)/RAND_MAX] );

	return tResult;
}

std::wstring generateName()
{
	std::wstring tResult;

	do 
	{
		tResult = L"\\\\.\\pipe\\npp_";
		tResult += doGen(5);

	} while ( !isUnique(tResult) );

	getIdMap().insert(tResult);

	return tResult;
}

//////////////////////////////////////////////////////////////////////////

pipe::pipe()
	:mPipe(NULL)
{
}

pipe::~pipe()
{
	close();
}

bool pipe::create(const std::wstring &aName)
{
	close();

	mName = aName;

	HANDLE tPipe = CreateNamedPipe(
		aName.c_str(),			//pipe's name
		PIPE_ACCESS_DUPLEX		/*|FILE_FLAG_OVERLAPPED*/,		//
		PIPE_TYPE_BYTE,
		1,
		MaxBufferSize,
		MaxBufferSize,
		10000,
		NULL);

	if (tPipe == INVALID_HANDLE_VALUE)
		return false;

	mPipe = tPipe;

	return true;
}


bool pipe::createUnique()
{
	return create(generateName());
}

bool pipe::open(const std::wstring &aName)
{
	close();

	HANDLE tPipe = CreateFile(
		aName.c_str(), //pipe's name
		GENERIC_READ | GENERIC_WRITE, // only need read access
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (tPipe == INVALID_HANDLE_VALUE)
		return false;

	mPipe = tPipe;

	return true;
}

bool pipe::read(void *aBuffer, int aBufSize, DWORD &aReadSize)
{
	const BOOL tReadResult = ReadFile(mPipe, aBuffer, aBufSize, &aReadSize, NULL);
	return TRUE == tReadResult;
}

bool pipe::write(const void *aBuffer, int aBufSize)
{
	DWORD tWritten = 0;
	const BOOL tResult = WriteFile(mPipe, aBuffer, aBufSize, &tWritten, NULL);
	return TRUE == tResult && tWritten == aBufSize;
}

void pipe::close()
{
	if( mPipe && mPipe != INVALID_HANDLE_VALUE )
	{
		CloseHandle(mPipe);
		mPipe = NULL;
		freeId(mName);
		mName.clear();
	}
}

const std::wstring &pipe::getName() const
{
	return mName;
}

bool pipe::wait()
{
	BOOL ret = ConnectNamedPipe(mPipe, NULL);

	return TRUE == ret;
}

