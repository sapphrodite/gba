#include <stdint.h>
#include <vector>
#include <string>
#include <cstdio>
#include <fstream>

#include <libpng/png.h>
#include <unordered_map>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

template <typename T>
struct vec2D {
    vec2D() = default;
    vec2D(T x_in, T y_in ) : x(x_in), y(y_in) {}
    T x, y;
};

struct color {
    u8 r, g, b, a;
    color() = default;
    color(u8 r_in, u8 g_in, u8 b_in, u8 a_in) : r(r_in), g(g_in), b(b_in), a(a_in) {}
};

bool operator==(const color& lhs, const color& rhs) {
    return lhs.r == rhs.r
           && lhs.g == rhs.g
           && lhs.b == rhs.b;
}

template<> struct std::hash<color> {
    std::size_t operator()(color const& c) const noexcept {
        return (c.a << 24) || (c.r << 16) || (c.g << 8) || (c.b);
    }
};

std::vector<u8> load_image_from_png(std::string filename, std::vector<u8>& data, vec2D<unsigned>& image_size, int& bpp) {
    FILE *fp = fopen(filename.c_str(), "rb");

    char header[8];    // 8 is the maximum size that can be checked
    fread(header, 1, 8, fp);  // This advances the read pointer - do not remove

    png_struct* png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_info* info_ptr = png_create_info_struct(png_ptr);

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);
    png_read_update_info(png_ptr, info_ptr);

    image_size = {png_get_image_width(png_ptr, info_ptr), png_get_image_height(png_ptr, info_ptr)};
    bpp = png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGB ? 3 : 4;
    int row_bytes = image_size.x * bpp;
    data = std::vector<u8>(image_size.x * image_size.y * bpp);

    // What in the actual fuck is this? thanks libpng
    // libpng writes data to locations given by a series of row pointers, potentially discontinuous.
    // To read into a predefined contiguous buffer, pointer arithmetic can get the address of every row
    std::vector<png_byte*> ptr_storage(image_size.y);
    for (int y = 0; y < image_size.y; y++) {
        ptr_storage[y] = data.data() + (row_bytes * y);
    }
    png_read_image(png_ptr, ptr_storage.data());

    // This *should* free both previously created structs...
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);

    return data;
}

constexpr bool export_4bpp = true;


int main(int argc, char** argv) {
    std::vector<u8> data;
    vec2D<unsigned> image_size;
    int bpp;
    load_image_from_png(argv[1], data, image_size, bpp);

    if ((image_size.x * image_size.y * bpp != data.size())
        || (image_size.x % 8 != 0 || image_size.y % 8 != 0)) {
        throw std::exception();
    }

    std::vector<u16> palette;
    std::unordered_map<color, int> palette_lookup;
    std::vector<u8> output(image_size.x * image_size.y / (export_4bpp ? 2 : 1), 0);

    vec2D<u8> num_tiles(image_size.x / 8, image_size.y / 8);
    for (int tile = 0; tile < (num_tiles.x * num_tiles.y); tile++) {
        vec2D<u8> current_tile(tile % num_tiles.x, tile / num_tiles.x);
        size_t dst_idx = tile * (64 / (export_4bpp ? 2 : 1));
        bool write_highnibble = false;

        for (int y = current_tile.y * 8; y < (current_tile.y + 1) * 8; y++) {
            for (int x = current_tile.x * 8; x < (current_tile.x + 1) * 8; x++) {
                size_t src_idx = (x + (y * image_size.x)) * bpp;
                color c(data[src_idx], data[src_idx + 1], data[src_idx + 2], 0);

                if (palette_lookup.find(c) == palette_lookup.end()) {
                    palette_lookup.emplace(c, palette.size());
                    u16 gba_color = int(32.0 * c.b / 255.0) << 10 | int(32.0 * c.g / 255.0) << 5 | int(32.0 * c.r / 255.0);
                    palette.emplace_back(gba_color);
                }

                u8 palette_index = palette_lookup[c];
                if (write_highnibble) {
                    palette_index <<= 4;
                }

                output[dst_idx] |= palette_index;

                if (export_4bpp) {
                    write_highnibble = !write_highnibble;
                }

                if (!write_highnibble) {
                    dst_idx++;
                }
            }
        }
    }

    std::ofstream f1(std::string(argv[2]) + ".tex", std::ios::binary | std::ios::out);
    f1.write((const char*) output.data(), output.size());

    std::ofstream f2(std::string(argv[2]) + ".pal", std::ios::binary | std::ios::out);
    f2.write((const char*) palette.data(), palette.size() * 2);
}