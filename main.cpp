//
// Created by Raphael Winkler on 01.04.23.
//
#include "window.h"
//#include "directVolume.h"
//#include "directVolume2.h"
//#include "directVolume3.h"

int main(int, char* [])
{
//    auto window = std::make_unique<DirectVolumeRendering>();
    auto window = std::make_unique<Window>();
    window->Loop();
//    auto window = std::make_unique<Window>();
//    window->Loop();
    return EXIT_SUCCESS;
}