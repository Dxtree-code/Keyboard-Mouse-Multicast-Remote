#include "mmki/network/Multicast.hpp"

bool NetRecvData::setData(uint8_t *data, int  len){
    if (len > RECV_DATA_LEN){
        return false;
    }
    this->len = len;
    for(int i = 0 ; i < len; i++){
        this->data[i] = data[i];
    }
    return true;
}

int NetRecvData::getData(uint8_t **store){
    *store = data.data();
    return this->len;
}

// do not adding count before data finally push
// to avoid using mutex
bool NetReceiverBuffer::push( uint8_t *data, int byteSize)
{
    if (this->count.load(std::memory_order_acquire) == RECV_BUFFER_LEN){
        return false;
    }

    this->buffer[this->tail].setData(data, byteSize);
    this->tail=  (this->tail +1)%RECV_BUFFER_LEN;
    this->count.fetch_add(1,  std::memory_order_release);
    return true;
}

NetRecvData * NetReceiverBuffer::pop(){
    if (this->count.load(std::memory_order_acquire) <= 0 ){
        return nullptr;
    }

    NetRecvData * data = &this->buffer[this->head];
    this->head = (this->head+1)%RECV_BUFFER_LEN;
    this->count.fetch_sub(1,  std::memory_order_release);
    
    return data;
}
