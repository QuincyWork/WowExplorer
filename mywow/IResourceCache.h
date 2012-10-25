#pragma once

#include "core.h"
#include <list>
#include <map>

//T�ǻ��������

template <class T>
class IResourceCache;

extern CRITICAL_SECTION g_refCS;

template <class T>
class IReferenceCounted
{
public:
	//
	IReferenceCounted() : ReferenceCounter(1), Cache(NULL) {}
	virtual ~IReferenceCounted(){}

	//
	void grab()
	{ 
		::EnterCriticalSection(&g_refCS);
		++ReferenceCounter;
		::LeaveCriticalSection(&g_refCS);
	}

	bool drop()
	{
		s32 refCount;
		::EnterCriticalSection(&g_refCS);

		_ASSERT( ReferenceCounter>0 );
		--ReferenceCounter;

		refCount = ReferenceCounter;
		::LeaveCriticalSection(&g_refCS);

		if (refCount == 1 && Cache)
		{
			onRemove();
			Cache->removeFromCache(static_cast<T*>(this));
		}
		else if ( 0 == refCount )
		{
			delete this;
			return true;
		}
		return false;
	}

	//
	s32 getReferenceCount() const { return ReferenceCounter; }
	const c8* getFileName() const { return fileName; }
	void setFileName(const c8* filename) { strcpy_s(fileName, MAX_PATH, filename); }

	IResourceCache<T>* Cache;	

protected:
	//�������Դ���Դ����(ITexture, IFileM2��)��Ϊ���Ż����̼߳��غͽ�ʡ�Դ棬�����߳�ֻ�����ڴ���Դ�����̸߳������Դ���Դ
	virtual void onRemove() = 0;

	volatile s32 ReferenceCounter;

private:
	c8		fileName[MAX_PATH];
};

template <class T>			
class IResourceCache
{
public:
	IResourceCache() : CacheLimit(10) { ::InitializeCriticalSection(&cs); }
	virtual ~IResourceCache() { ::DeleteCriticalSection(&cs); }

public:
	T* tryLoadFromCache( const char* filename );
	void addToCache( const char* filename, T* item );
	void removeFromCache( T* item );
	void flushCache();
	void setCacheLimit(u32 limit) { CacheLimit = limit; }

protected:
	typedef std::list<T*, qzone_allocator<T*>> T_FreeList;

	T_FreeList FreeList;			//����ʹ���У�����ɾ��
	u32 CacheLimit;		//�����б��С

	typedef std::map<string<MAX_PATH>, T*, std::less<string<MAX_PATH>>, qzone_allocator<std::pair<string<MAX_PATH>, T*>>>	T_UseMap;
	T_UseMap UseMap;

	CRITICAL_SECTION cs;
};

template <class T>
T* IResourceCache<T>::tryLoadFromCache( const char* filename )
{
	::EnterCriticalSection(&cs);

	T_UseMap::iterator itr = UseMap.find(filename);
	if (itr != UseMap.end())
	{
		itr->second->grab();

		::LeaveCriticalSection(&cs);
		return itr->second;
	}

	// free cache �в���
	for ( T_FreeList::iterator itr = FreeList.begin(); itr != FreeList.end(); ++itr )
	{
		T* t = (*itr);
		const c8* fname = t->getFileName();
		if ( _stricmp(fname, filename) == 0 )			//�ҵ����Ƶ�use cache
		{
			t->grab();
			UseMap[filename] = t;
			FreeList.erase(itr);		

			::LeaveCriticalSection(&cs);
			return t;
		}
	}

	::LeaveCriticalSection(&cs);
	return NULL;
}

template <class T>
void IResourceCache<T>::addToCache( const char* filename, T* item)
{
	::EnterCriticalSection(&cs);

	UseMap[filename] = item;
	item->grab();		
	item->Cache = this;

	::LeaveCriticalSection(&cs);
}

template <class T>
void IResourceCache<T>::removeFromCache( T* item )
{
	::EnterCriticalSection(&cs);

	const c8* filename = item->getFileName();
	T_UseMap::iterator itr = UseMap.find(filename);
	_ASSERT(itr != UseMap.end());
	if (itr != UseMap.end())
		UseMap.erase(itr);

	if (!CacheLimit)			//�����Ƶ�freelist
	{
		item->drop();
	}
	else
	{
		while( FreeList.size() >= CacheLimit )
		{
			_ASSERT(FreeList.back()->getReferenceCount() == 1);
			FreeList.back()->drop();
			FreeList.pop_back();
		}
		FreeList.push_front(item);
	}
	
	::LeaveCriticalSection(&cs);
}

template <class T>
void IResourceCache<T>::flushCache()
{
	::EnterCriticalSection(&cs);

	for(T_UseMap::iterator itr = UseMap.begin(); itr != UseMap.end(); ++itr)
	{
		T* t = itr->second;
		_ASSERT(t->getReferenceCount() == 1);
		if (t->getReferenceCount() == 2)
		{
			MessageBoxA(NULL, "Error!", "", 0);
		}
		t->drop();
	}

	while( FreeList.size() > 0 )
	{
		_ASSERT(FreeList.back()->getReferenceCount() == 1);
		FreeList.back()->drop();
		FreeList.pop_back();
	}

	::LeaveCriticalSection(&cs);
}
