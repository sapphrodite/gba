#ifndef MMIO_ARRAY_H
#define MMIO_ARRAY_H

#include "integral_types.h"
#include "register_types.h"

template <typename mmio, typename T>
class mmio_array {
public:
    static constexpr volatile T& get(size_t index) { return (volatile T&) mmio::memory[index * sizeof(T)]; }
};


#endif //MMIO_ARRAY_H
