#pragma once

#include "base.h"

//���䶨������Դ�����������ʹ������
template<typename T>
class IResourcePool
{
private:
	DISALLOW_COPY_AND_ASSIGN(IResourcePool);

public:
	IResourcePool() : Quota(0), FreeCount(0), Entries(NULL_PTR)	{ InitializeListHead(&FreeList); }
	virtual ~IResourcePool() 
	{ 
		if (Entries)
		{
			delete[] Entries;
		}

		InitializeListHead(&FreeList);
		FreeCount = Quota;
	}

public:
	void allocateAll(u32 quota);

	T* get();
	void put(T* t);

	u32	getUsedSize() const { return Quota - FreeCount; }

private:
	struct SEntry
	{
		LENTRY	Link;
		T	Data;
	};

private:

	u32		Quota;
	u32		FreeCount;
	SEntry*		Entries;
	LENTRY		FreeList;
};

template<typename T>
void IResourcePool<T>::put( T* t )
{
	SEntry* entry = CONTAINING_RECORD(t, SEntry, Data);
	InsertTailList(&FreeList, &entry->Link);	

	++FreeCount;
	ASSERT(FreeCount <= Quota);
}

template<typename T>
T* IResourcePool<T>::get()
{
	if (IsListEmpty(&FreeList))
		return NULL_PTR;

	LENTRY* p = RemoveHeadList(&FreeList);
	SEntry* entry = reinterpret_cast<SEntry*>CONTAINING_RECORD(p, SEntry, Link);
	--FreeCount;

	return &entry->Data;
}

template<typename T>
void IResourcePool<T>::allocateAll( u32 quota )
{
	if(Entries)
		return;

	Quota = quota;

	Entries = new SEntry[Quota];

	for (u32 i=0; i<Quota; ++i)
	{
		InsertTailList(&FreeList, &Entries[i].Link);
	}

	FreeCount = Quota;
}