#pragma once

#include "core.h"
#include <list>
#include <map>

//T�ǻ��������

template <class T>			
class IResourceCache
{
public:
	IResourceCache() : CacheLimit(10) { }
	virtual ~IResourceCache() {}

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
};

template <class T>
T* IResourceCache<T>::tryLoadFromCache( const char* filename )
{
	T_UseMap::iterator itr = UseMap.find(filename);
	if (itr != UseMap.end())
	{
		itr->second->grab();
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
			return t;
		}
	}
	return NULL;
}

template <class T>
void IResourceCache<T>::addToCache( const char* filename, T* item)
{
	UseMap[filename] = item;
	item->grab();		
	item->Cache = this;
}

template <class T>
void IResourceCache<T>::removeFromCache( T* item )
{
	const c8* filename = item->getFileName();
	T_UseMap::iterator itr = UseMap.find(filename);
	_ASSERT(itr != UseMap.end());
	if (itr != UseMap.end())
		UseMap.erase(itr);

	while( FreeList.size() > CacheLimit )
	{
		_ASSERT(FreeList.back()->getReferenceCount() == 1);
		FreeList.back()->drop();
		FreeList.pop_back();
	}
	FreeList.push_front(item);
}

template <class T>
void IResourceCache<T>::flushCache()
{
	for(T_UseMap::iterator itr = UseMap.begin(); itr != UseMap.end();)
	{
		T* t = itr->second;
		_ASSERT(t->getReferenceCount() == 1);
		t->drop();
		itr = UseMap.erase(itr);
	}

	while( FreeList.size() > 0 )
	{
		_ASSERT(FreeList.back()->getReferenceCount() == 1);
		FreeList.back()->drop();
		FreeList.pop_back();
	}
}
