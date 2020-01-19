#pragma once
#include "SingletonBase.h"

template<typename T>
class ObjectPoolManager
{
private:
	queue<T * > m_Object;
	int m_MaxSize;
public:
	ObjectPoolManager(int Size)
	{
		m_MaxSize = Size;
		for (int i = 0; i < m_MaxSize; i++)
		{
			T *Item = new T();
			m_Object.push(Item);
		}
	}
	ObjectPoolManager()
	{

	}
	void DeleteAll()
	{
		while (!m_Object.empty()) {
			T* object = m_Object.pop();
			delete object;
		}
		m_MaxSize = 0;
	}
	~ObjectPoolManager()
	{
		
	}
	T * ReturnObject() {

		if (m_Object.empty())
		{
			ObjectExpand();
		}

		T * item = m_Object.front();
		m_Object.pop();
		return  item;
	}
	void ObjectExpand() {
		for (int i = 0; i < m_MaxSize; i++)
		{
			T * Item = new T();
			m_Object.push(Item);
		}
		m_MaxSize *= 2;
	}
	void PushObject(T * Object) {
		m_Object.push(Object);
	}
	int GetMaxSize()
	{
		return m_MaxSize;
	}
};

template<typename T>
class ObjectPool : public SingletonBase<ObjectPool<T>>
{
	ObjectPoolManager<T> m_Object;

	~ObjectPool()
	{
		m_Object.DeleteAll();
	}
public:
	ObjectPool()
	{
		m_Object = { 100 };
	}

	T * GetObObject()
	{
		return m_Object.ReturnObject();
	}
	void PushObject(T * object)
	{
		m_Object.PushObject(object);
	}
	int GetObjectSize()
	{
		return m_Object.GetMaxSize();
	}
};