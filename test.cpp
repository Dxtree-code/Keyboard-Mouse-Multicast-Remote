
#include <iostream>
#include "internal_lib/network/Multicast.hpp"
#include "internal_lib/tools/datstruct.hpp"
using namespace std;

// void test1(){
//     MouseState state;
//     state.x = 300;
//     state.y = 325;
//     state.leftClick = 1;
//     state.rightClick = 1;
//     state.midClick =1;

//     MouseState parsed;

//     uint8_t buf[16];
//     formatMouseData(state,buf, 16);
//     parseMouseData(parsed, buf, 16);

//     cout<< "x: "<<state.x<<" "<<parsed.x<<endl;
//     cout<< "y: "<<state.y<<" "<<parsed.y<<endl;
//     cout<< "r: "<<state.rightClick<<" "<<parsed.rightClick<<endl;
//     cout<< "l: "<<state.leftClick<<" "<<parsed.leftClick<<endl;
//     cout<< "m: "<<state.midClick<<" "<<parsed.midClick<<endl;

//     for ( uint8_t i : buf){
//         cout<<(int)i<<endl;
//     }
//     cout<<endl;
// }

void test2()
{
    KeyboardState state;
    KeyboardState parsed = {};
    uint8_t buf[16];
    state.press = 1;
    state.code = 65;

    formatKeyboardData(state, buf, 16);

    parseKeyboardData(parsed, buf, 16);

    cout << "press: " << state.press << " " << parsed.press << endl;
    cout << "code: " << state.code << " " << parsed.code << endl;

    cout << "is Mouse: " << isMouseData(buf, 16) << endl;
    cout << "is Keyboard: " << isKeyboardData(buf, 16) << endl;
    for (uint8_t i : buf)
    {
        cout << (int)i << endl;
    }
    cout << endl;
}

void test3()
{
    int arr[4] = {10, 22, 65, 104};
    std::cout << has_ip(arr) << std::endl;
}


void test4(){
    CilcularQueue<MouseState, 255> queue;

    // queue.push();
}
int main()
{

    test4();
}