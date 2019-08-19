#include "Generator.h"

Generator::Generator() {
    next_spawn_time = std::chrono::system_clock::now().time_since_epoch();
};

void Generator::generate(std::list<Block*> &block_array) {
    std::cout << "Velicina: " << block_array.size() << std::endl;
    //std::cout << "Vreme: " << time(NULL) << " " << next_spawn_time << std::endl;
    if (block_array.size() < 2000) {
        if (std::chrono::system_clock::now().time_since_epoch() >= next_spawn_time) {

            block_array.push_back(new Block(50));

            //std::cout << "Generisan blok u vreme: " << std::chrono::system_clock::now().time_since_epoch() << std::endl;

            static auto rng = std::mt19937(std::random_device()());
            static std::uniform_real_distribution<float> uni((int)1 / Block::getSpeed(), 1.0);
            next_spawn_time = std::chrono::system_clock::now().time_since_epoch() + std::chrono::milliseconds((int) (uni(rng) * 1000));
        }
    }
}
