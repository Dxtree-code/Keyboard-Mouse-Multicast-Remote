#include <cstring>
#include "mmki/network/net.hpp"

bool NetData::setData(uint8_t *data, int len)
{
    if (len > RECV_DATA_LEN)
    {
        return false;
    }
    this->len = len;
    memcpy(this->data.data(), data, len);
    return true;
}

int NetData::getData(uint8_t **store)
{
    *store = data.data();
    return this->len;
}

// do not adding count before data finally push
// to avoid using mutex
bool NetBuffer::push(uint8_t *data, int byteSize)
{
    if (this->count.load(std::memory_order_acquire) == RECV_BUFFER_LEN)
    {
        return false;
    }

    this->buffer[this->tail].setData(data, byteSize);
    this->tail = (this->tail + 1) % RECV_BUFFER_LEN;
    this->count.fetch_add(1, std::memory_order_release);
    return true;
}

NetData *NetBuffer::pop()
{
    if (this->count.load(std::memory_order_acquire) <= 0)
    {
        return nullptr;
    }

    NetData *data = &this->buffer[this->head];
    this->head = (this->head + 1) % RECV_BUFFER_LEN;
    this->count.fetch_sub(1, std::memory_order_release);

    return data;
}
