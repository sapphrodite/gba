#ifndef DMA_H
#define DMA_H

#include <atomic>
#include <common/integral_types.h>
#include <common/register_types.h>


template <size_t index, typename access_type>
class dma {
public:
    static void write(access_type* dst_in, const access_type* src_in, size_t bytes, u16 settings) {
        std::atomic_signal_fence(std::memory_order_release);
        src::set(src_in);
        dst::set(dst_in);
        wordcount::set(bytes);
        control::set(0x8000 | settings);
        std::atomic_signal_fence(std::memory_order_acquire);
    }
private:
    constexpr static size_t base_addr = 0x040000B0 + (12 * index);

    using src = integer_register<mmio<base_addr, uintptr_t>, 0xFFFFFFFF, const access_type*>;
    using dst = integer_register<mmio<base_addr + 4, uintptr_t>, 0xFFFFFFFF, access_type*>;
    using wordcount = integer_register<mmio<base_addr + 8, u16>, 0xFFFF, u16>;
    using control = integer_register<mmio<base_addr + 10, u16>, 0xFFFF, u16>;
};

#endif //DMA_H