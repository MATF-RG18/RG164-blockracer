#include "Generator.h"

Generator::Generator() {
    // Postavljamo inicijalno vreme sledeceg generisanja bloka (za prvi blok)
    next_spawn_time = std::chrono::system_clock::now().time_since_epoch();
};

// Metod koji generise blokove i dodaje ih u listu
void Generator::generate(std::list<Block*> &block_array) {
    if (block_array.size() < 40) {
        if (std::chrono::system_clock::now().time_since_epoch() >= next_spawn_time) {

            block_array.push_back(new Block(50));

            static auto rng = std::mt19937(std::random_device()());
            static std::uniform_real_distribution<float> uni(1 / Block::getSpeed(), 1.0 - 1 / Block::getSpeed());
            next_spawn_time = std::chrono::system_clock::now().time_since_epoch() + std::chrono::milliseconds((int) (uni(rng) * 1000));
        }
    }
}
