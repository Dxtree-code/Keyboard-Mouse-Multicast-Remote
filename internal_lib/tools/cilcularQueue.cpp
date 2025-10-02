#include<datstruct.hpp>

template<typename T, int size>
int CilcularQueue<T, size>::push(T data){
    if(count+1 >size) return 1; // fail to push
    this->queue[tail] = data;
    this->tail = (this->tail+1)%size;
    this->count ++;
    return 0; // success push
}

template<typename T, int size>
T* CilcularQueue<T, size>::pop(){
    if (count <= 0 ) return nullptr;
    T* data =  this->queue[head];
    this->head = (this->head+1)%size;
    this->count--;
}