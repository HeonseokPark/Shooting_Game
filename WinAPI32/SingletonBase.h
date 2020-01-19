#pragma once



template <typename T>
class SingletonBase 
{	
protected:
	static T* Instance;

	SingletonBase() {};
	~SingletonBase() {};
public:
	static T* GetInstance();
	void ReleaseSingleton();
};

template <typename T>
T* SingletonBase<T>::Instance = nullptr;


template<typename T>
T * SingletonBase<T>::GetInstance()
{
	if (!Instance)Instance = new T;
	return Instance;
}

template<typename T>
void SingletonBase<T>::ReleaseSingleton()
{
	if (Instance)
	{
		delete Instance;
		Instance = nullptr;
	}
}
