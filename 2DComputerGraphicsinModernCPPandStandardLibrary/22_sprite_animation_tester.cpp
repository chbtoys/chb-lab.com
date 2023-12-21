// Compile: clear && clang++ -std=c++20 -lz 22_sprite_animation_tester.cpp -o 22_sprite_animation_tester
#include <iostream>
#include <vector>
#include <fstream>
#include <array>
#include <cstdint>
#include <string>
#include <zlib.h>
#include <arpa/inet.h>

class APNG {
private:
    std::ofstream file;
    uint32_t sequence_number = 0;
    std::string m_filename="";
    uint32_t m_width=0;
    uint32_t m_height=0;
    uint32_t m_num_frames=0;
    uint32_t m_num_plays=0;
    uint16_t m_delay_num=1;
    uint16_t m_delay_den=0;
    uint8_t m_dispose_op=0;
    uint8_t m_blend_op=0;

    std::vector<uint8_t> deflate(const std::vector<uint8_t>& raw_data) {
        uLongf compressed_size = compressBound(raw_data.size());
        std::vector<uint8_t> compressed_data(compressed_size);
        if (compress2(compressed_data.data(), &compressed_size, raw_data.data(), raw_data.size(), Z_BEST_COMPRESSION) != Z_OK) {
            std::cerr << "Deflation failed." << std::endl;
            return {};
        }
        compressed_data.resize(compressed_size);
        return compressed_data;
    }

    uint32_t crc32Calculation(const std::string& type, const std::vector<uint8_t>& data) {
        // Concatenate type and data into a single buffer
        std::vector<uint8_t> buffer(type.begin(), type.end());
        buffer.insert(buffer.end(), data.begin(), data.end());
        
        // Compute the CRC over the concatenated buffer
        uint32_t crc = crc32(0, buffer.data(), buffer.size());
        return htonl(crc);  // Return the CRC value directly with htonl
    }

    void write_chunk(const std::string& type, const std::vector<uint8_t>& data) {
        uint32_t length = htonl(data.size());
        file.write(reinterpret_cast<const char*>(&length), 4);
        file.write(type.c_str(), 4);

        if (!data.empty()) {
            file.write(reinterpret_cast<const char*>(data.data()), data.size());
        }

        uint32_t crc = crc32Calculation(type, data);
        file.write(reinterpret_cast<const char*>(&crc), 4);
        if (!file) {
            std::cerr << "Failed to write chunk length" << std::endl;
            return;
        }
    }

    void write_idat(std::vector<uint8_t>& raw_data) {
        std::vector<uint8_t> idat_chunk = deflate(raw_data);
        uint32_t* p = reinterpret_cast<uint32_t*>(idat_chunk.data());
        write_chunk("IDAT", idat_chunk);
    }

    void write_ihdr() {
        std::vector<uint8_t> ihdr_chunk(13, 0);
        uint32_t* p = reinterpret_cast<uint32_t*>(ihdr_chunk.data());
        *p++ = htonl(m_width);
        *p++ = htonl(m_height);
        ihdr_chunk[8] = 8;
        ihdr_chunk[9] = 2;
        write_chunk("IHDR", ihdr_chunk);
    }

    void write_actl() {
        std::vector<uint8_t> actl_chunk(8, 0);
        uint32_t* p = reinterpret_cast<uint32_t*>(actl_chunk.data());
        *p++ = htonl(m_num_frames);
        *p++ = htonl(m_num_plays);
        write_chunk("acTL", actl_chunk);
    }

    void write_fcTL() {
        std::vector<uint8_t> fctl_chunk(26, 0);
        uint32_t* p = reinterpret_cast<uint32_t*>(fctl_chunk.data());
        
        *p++ = htonl(sequence_number++);
        *p++ = htonl(m_width);
        *p++ = htonl(m_height);
        *p++ = htonl(0); // X Offset
        *p++ = htonl(0); // Y Offset

        uint16_t* p16 = reinterpret_cast<uint16_t*>(p);
        *p16++ = htons(m_delay_num ? m_delay_num : 1);  // Ensuring denominator is not zero
        *p16++ = htons(m_delay_den ? m_delay_den : 10);  // Ensuring denominator is not zero
        
        uint8_t* p8 = reinterpret_cast<uint8_t*>(p16);
        *p8++ = m_dispose_op;
        *p8++ = m_blend_op;

        write_chunk("fcTL", fctl_chunk);
    }

public:
    APNG(const std::string& filename, uint32_t width, uint32_t height, uint32_t num_frames, uint32_t num_plays, uint16_t delay_num, uint16_t delay_den, uint8_t dispose_op, uint8_t blend_op)
        : file(filename, std::ios::binary)
    {
        if (!file.is_open()) {
            throw std::ios_base::failure("Failed to open file");
        }
        m_filename=filename;
        m_width=width;
        m_height=height;
        m_num_frames=num_frames;
        m_num_plays=num_plays;
        m_delay_num=delay_num;
        m_delay_den=delay_den;
        m_dispose_op=dispose_op;
        m_blend_op=blend_op;
        const std::vector<uint8_t> png_signature = {137, 80, 78, 71, 13, 10, 26, 10};
        file.write(reinterpret_cast<const char*>(png_signature.data()), png_signature.size());
        
        write_ihdr();
        write_actl();
    }

    ~APNG() {
        close();
    }

    void write_frame(std::vector<uint8_t> raw_data, bool is_first_frame = false) {
        if (is_first_frame) {
            write_fcTL();
            write_idat(raw_data);
        } else {
            write_fcTL();
            
            std::vector<uint8_t> fdat_chunk = deflate(raw_data);
            fdat_chunk.insert(fdat_chunk.begin(), 4, 0);  // Make space for sequence number
            uint32_t* p = reinterpret_cast<uint32_t*>(fdat_chunk.data());
            *p = htonl(sequence_number++);
            
            write_chunk("fdAT", fdat_chunk);
        }
    }

    void close() {
        if (file.is_open()) {
            write_chunk("IEND", {});
            file.close();
        }
    }
};

std::vector<uint8_t> readPPMP6(const std::string& filename, uint32_t& width, uint32_t& height) {
    std::string magic;
    int max;

    std::ifstream in(filename, std::ifstream::binary);
    if (!in.is_open()) {
        std::cout << "Can't open " << filename << std::endl;
        exit(1);
    }

    in >> magic;
    in.seekg(1, std::ios::cur);
    while (in.peek() == '#') {
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    in >> width >> height >> max;
    if (max != 255) {
        std::cout << "Not 8 bit per RGB color." << std::endl;
        exit(1);
    }
    in >> std::ws; // skip any whitespace

    std::vector<uint8_t> data_with_filters;

    if (magic == "P6") {
        std::vector<uint8_t> data(width * height * 3);
        in.read((char*)data.data(), width * height * 3);

        // Insert filter bytes
        size_t scanline_length = width * 3;  // For 24-bit RGB images
        data_with_filters.reserve((scanline_length + 1) * height);  // +1 for filter byte

        for (int row = 0; row < height; ++row) {
            data_with_filters.push_back(0);  // Filter byte (0 for "None")
            auto start = data.begin() + row * scanline_length;
            auto end = start + scanline_length;
            data_with_filters.insert(data_with_filters.end(), start, end);
        }
    } else {
        std::cout << filename << " is not a P6 file." << std::endl;
        exit(1);
    }
    
    in.close();
    std::cout << "Reading: " << filename << std::endl;
    return data_with_filters;
}

int main() {
    try {
        std::vector<std::vector<uint8_t>> anim;
        uint32_t width=384;
        uint32_t height=384;
        
        // Reading frames into anim vector
        for (int i = 0; i < 16; ++i) {
            std::string zero = (i < 10 ? "0" : "");
            std::string filename = "./BattleSquadron/" + zero + std::to_string(i) + "_Overworld_Enemies.ppm";
            anim.push_back(readPPMP6(filename,width,height));
        }

        uint32_t frames=anim.size();

        APNG apng("22_sprite.png", width, height, frames, 0, 625, 10000, 1, 0); // width, height, num_frames, num_plays (0 == infinity) ...

        for (int frame = 0; frame < frames; frame++) {
            bool is_first_frame = (frame == 0);
            apng.write_frame(anim[frame], is_first_frame);
        }

        apng.close();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    std::cout << "Writing: 22_sprite.png" << std::endl;

    return 0;
}
