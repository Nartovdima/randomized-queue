#include "Subset.hpp"

#include "RandomizedQueue.hpp"

#include <string>

void subset(unsigned long k, std::istream& in, std::ostream& out) {
    RandomizedQueue<std::string> queue;
    std::string line;
    while (std::getline(in, line)) {
        queue.enqueue(line);
    }
    
    auto it = queue.cbegin();
    auto q_end = queue.cend();
    while (k-- && it != q_end) {
        out << *it << std::endl;
        ++it;
    }
}