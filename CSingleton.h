/*
* created by wsh on 2023-12-31
*
*/

#ifndef CSINGLETON_H_
#define CSINGLETON_H_
#include<iostream>
#include<pthread.h>
template<class T>
class Singleton
{
private:
    static T* m_pInstance;
    static pthread_mutex_t mutex;

    /* data */
public:
    Singleton(/* args */){


    }
    static T* GetInstance(){
        if(m_pInstance == nullptr){
            pthread_mutex_lock(&mutex);
            if(m_pInstance == nullptr){
                T* temp = new T;
                m_pInstance = temp;
            }


            pthread_mutex_unlock(&mutex);
        }
        return m_pInstance;
    }

};

template<class T>
T* Singleton<T>::m_pInstance = nullptr;

template<class T>
pthread_mutex_t Singleton<T>::mutex;



class Singletest:public Singleton<Singletest>
{
    public:
        Singletest(){
            std::cout<<"Single test!"<<std::endl;
        }


};

#endif