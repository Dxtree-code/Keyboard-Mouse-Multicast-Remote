#include "internal_lib/serializer.hpp"

using namespace std;

int main(){
    MouseState state;
    state.dx = 300;
    state.dy = 325;
    state.leftClick = 1;
    state.rightClick = 1;
    state.midClick =1;

    MouseState parsed;

    uint8_t buf[16];
    formatMouseData(state,buf, 16);
    parseMouseData(parsed, buf, 16);

    cout<< "x: "<<state.dx<<" "<<parsed.dx<<endl;
    cout<< "y: "<<state.dy<<" "<<parsed.dy<<endl;
    cout<< "r: "<<state.rightClick<<" "<<parsed.rightClick<<endl;
    cout<< "l: "<<state.leftClick<<" "<<parsed.leftClick<<endl;
    cout<< "m: "<<state.midClick<<" "<<parsed.midClick<<endl;

    for ( uint8_t i : buf){
        cout<<(int)i<<endl;
    }
    cout<<endl;
}