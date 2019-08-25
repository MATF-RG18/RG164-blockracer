#define GL_SILENCE_DEPRECATION (1)

#ifndef _GENERATOR_H_
#define _GENERATOR_H_

#include <GL/glut.h>
#include <list>
#include <chrono>
#include <random>
#include "Block.h"

class Generator {
private:
    std::chrono::duration<double> next_spawn_time;

public:
    Generator();
    void generate(std::list<Block*> &block_array);
};

#endif