#include<array>
template <typename T, int size>class CilcularQueue{
    protected:
        std::array<T, size> queue;
        int head;
        int tail;
        int count = 0;
    public:
        virtual int push(T data);
        virtual T* pop();   
};