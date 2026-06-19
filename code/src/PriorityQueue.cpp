#include "PriorityQueue.hpp"

void PriorityQueue::push(double cost, int vertex) {
    heap.push({cost, vertex});
}

bool PriorityQueue::empty() const {
    return heap.empty();
}

PriorityQueue::Item PriorityQueue::pop(const std::vector<double>& current) {
 
    while (!heap.empty()) {
        Item top = heap.top();
        heap.pop();

        if (top.vertex >= 0 &&
            top.vertex < static_cast<int>(current.size()) &&
            top.cost > current[top.vertex]) {
            continue;  // entrada obsoleta, ignora
        }

        return top;
    }

    return {0.0, -1};  
}
