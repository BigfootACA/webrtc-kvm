#ifndef RESOURCE_RELEASE
#define RESOURCE_RELEASE
#include<stdlib.h>
#include<unistd.h>
#include<functional>
template<typename T>
class ResourceRelease{
	public:
		inline ResourceRelease(T data,std::function<void(T)>callback)
			:data(data),callback(callback){}
		inline ~ResourceRelease(){End();}
		inline T Take(){call=false;return data;}
		inline void Set(T val){data=val;}
		inline T Get()const{return data;}
		inline bool IsCall()const{return call;}
		inline void Call(){callback(data);}
		inline void End(){if(IsCall())Call();}
	private:
		T data;
		std::function<void(T)>callback;
		bool call=true;
};
class FDRelease:public ResourceRelease<int>{
	public:inline FDRelease(int data):ResourceRelease<int>(data,[](int fd){
		if(fd>=0)close(fd);
	}){}
};
class PointerRelease:public ResourceRelease<void*>{
	public:inline PointerRelease(void*data):ResourceRelease<void*>(data,[](void*ptr){
		if(ptr)free(ptr);
	}){}
};
template<typename T>
class ObjectRelease:public ResourceRelease<T>{
	public:inline ObjectRelease(T data):ResourceRelease<T>(data,[](T d){
		if(d)delete d;
	}){}
};
#endif
