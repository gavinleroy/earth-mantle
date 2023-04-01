//
// Created by Raphael Winkler on 01.04.23.
//
#include "window.h"

int main(int, char* [])
{
    auto window = std::make_unique<Window>();
    window->Loop();
    return EXIT_SUCCESS;
}