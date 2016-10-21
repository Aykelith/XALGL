#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <Global.hpp>

#include <GL.hpp>
#include <iostream>

static bool checkErrors(int line = 0, const char* file = "") {
    GLenum glErr;
    glErr = glGetError();
    if (glErr != GL_NO_ERROR)
    {
        if (file[0] != '\0') std::cout << file << ":";
        if (line) std::cout << line << ":";
        std::cout << "Error: " << gluErrorString(glErr) << "\n";
        return 1;
    }
    return 0;
}

#endif