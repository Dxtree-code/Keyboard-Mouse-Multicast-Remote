#include "./Multicast.hpp"

void NetReceiverBuffer::push(std::array<uint8_t, RECV_DATA_LEN> data)
{
    this->buffer[this->tail] = data;
}
