#include<windows.h>
#include <vector>
#include <shellscalingapi.h>
#include <iostream>
#include"mmki/system/system.hpp"


void DisplaySetting::changeDisplaySetting(int width,int height,  int relativeDpi){
    if (!this->resChanged){
        EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &this->originalDm);
    }
    DEVMODE devmode;
    ZeroMemory(&devmode, sizeof(DEVMODE));
    
    devmode.dmPelsWidth = width;
    devmode.dmPelsHeight = height;
    devmode.dmSize = sizeof(DEVMODE);
    devmode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
    
    ChangeDisplaySettings(&devmode, CDS_FULLSCREEN);

    //MICROSOFT DOES NOT RECOMENDING THIS API USES;
    //IT MIGHT REMOVED IN UPCOMING WINDOWS VERSION
    SystemParametersInfo(SPI_SETLOGICALDPIOVERRIDE, relativeDpi, (LPVOID)0, 1);
};

void DisplaySetting::revertDisplaySetting(){
    ChangeDisplaySettings(&this->originalDm, CDS_FULLSCREEN);
}

void DisplaySetting::changeDisplayScale(int relativeDpi){
      SystemParametersInfo(SPI_SETLOGICALDPIOVERRIDE, relativeDpi, (LPVOID)0, 1);
}