#include "internal_lib/tools/serializer.hpp"
#include <iostream>

using namespace std;


// void test1(){
//     MouseState state;
//     state.dx = 300;
//     state.dy = 325;
//     state.leftClick = 1;
//     state.rightClick = 1;
//     state.midClick =1;

//     MouseState parsed;

//     uint8_t buf[16];
//     formatMouseData(state,buf, 16);
//     parseMouseData(parsed, buf, 16);

//     cout<< "x: "<<state.dx<<" "<<parsed.dx<<endl;
//     cout<< "y: "<<state.dy<<" "<<parsed.dy<<endl;
//     cout<< "r: "<<state.rightClick<<" "<<parsed.rightClick<<endl;
//     cout<< "l: "<<state.leftClick<<" "<<parsed.leftClick<<endl;
//     cout<< "m: "<<state.midClick<<" "<<parsed.midClick<<endl;

//     for ( uint8_t i : buf){
//         cout<<(int)i<<endl;
//     }
//     cout<<endl;
// }

void test2(){
    KeyboardState state;
    KeyboardState parsed = {};
    uint8_t buf[16];
    state.press=1;
    state.code = 65;

    formatKeyboardData(state, buf, 16);
    
    parseKeyboardData(parsed, buf, 16);
    
    cout<< "press: "<<state.press<<" "<<parsed.press<<endl;
    cout<< "code: "<<state.code<<" "<<parsed.code<<endl;
    
    cout<< "is Mouse: "<<isMouseData(buf, 16)<<endl;
    cout<< "is Keyboard: "<<isKeyboardData(buf, 16)<<endl;
    for ( uint8_t i : buf){
        cout<<(int)i<<endl;
    }
    cout<<endl;
}
int main(){
    test2();
}