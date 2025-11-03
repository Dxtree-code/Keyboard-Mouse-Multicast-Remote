#pragma once

#include <array>
#include <stdint.h>
#include <atomic>

#include "mmki/config.hpp"

using std::array;
using std::atomic;

// This struct used for transfer data between receiver thread and executor thread
struct NetData
{
    array<uint8_t, RECV_DATA_LEN> data;
    int len;

    bool setData(uint8_t *data, int len);
    int getData(uint8_t **store);
};
// This Provide buffer from data received to executor thread
struct NetBuffer
{
    int head = 0;
    int tail = 0;
    atomic<int> count = 0;
    array<NetData, RECV_BUFFER_LEN> buffer;

    bool push(uint8_t *data, int byteSize);
    NetData *pop();
};