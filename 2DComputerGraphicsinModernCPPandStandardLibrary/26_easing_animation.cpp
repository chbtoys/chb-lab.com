// Compile: clear && clang++ -std=c++20 -lz 26_easing_animation.cpp -o 26_easing_animation
#include <cmath>
#include <vector>
#include <tuple>
#include <fstream>
#include <iostream>
#include <zlib.h>
#include <arpa/inet.h>
#include "ppmpp.hpp"

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

namespace ease {
	class Easing {
	public:
		Easing() {}

	    // easeInSine
	    double easeInSine(double t) {
		    return 1.0 - std::cos((t * M_PI) / 2.0);
		}

		// easeInCubic
		double easeInCubic(double t) {
		    return t * t * t;
		}

		// easeInQuint
		double easeInQuint(double t) {
		    return t * t * t * t * t;
		}

		// easeInCirc
		double easeInCirc(double t) {
		    return 1.0 - std::sqrt(1.0 - std::pow(t, 2.0));
		}

		// easeInElastic
		double easeInElastic(double t) {
		    const double c4 = (2 * M_PI) / 3;

		    if (t == 0) {
		        return 0;
		    } else if (t == 1) {
		        return 1;
		    } else {
		        return -std::pow(2, 10 * t - 10) * std::sin((t * 10 - 10.75) * c4);
		    }
		}

		// easeOutSine
		double easeOutSine(double t) {
		    return std::sin((t * M_PI) / 2.0);
		}

		// easeOutCubic
		double easeOutCubic(double t) {
		    return 1.0 - std::pow(1.0 - t, 3.0);
		}

		// easeOutQuint
		double easeOutQuint(double t) {
		    return 1.0 - std::pow(1.0 - t, 5.0);
		}

		// easeOutCirc
		double easeOutCirc(double t) {
		    return std::sqrt(1 - std::pow(t - 1, 2.0));
		}

		// easeOutElastic
		double easeOutElastic(double t) {
		    const double c4 = (2 * M_PI) / 3;

		    if (t == 0) {
		        return 0;
		    } else if (t == 1) {
		        return 1;
		    } else {
		        return std::pow(2, -10 * t) * std::sin((t * 10 - 0.75) * c4) + 1;
		    }
		}

		// easeInOutSine
		double easeInOutSine(double t) {
		    return - (std::cos(M_PI * t) - 1) / 2.0;
		}

		// easeInOutCubic
		double easeInOutCubic(double t) {
		    if (t < 0.5) {
		        return 4 * t * t * t;
		    } else {
		        return 1 - std::pow(-2 * t + 2, 3) / 2.0;
		    }
		}

		// easeInOutQuint
		double easeInOutQuint(double t) {
		    if (t < 0.5) {
		        return 16 * t * t * t * t * t;
		    } else {
		        return 1 - std::pow(-2 * t + 2, 5) / 2.0;
		    }
		}

		// easeInOutCirc
		double easeInOutCirc(double t) {
		    if (t < 0.5) {
		        return (1 - std::sqrt(1 - std::pow(2 * t, 2))) / 2.0;
		    } else {
		        return (std::sqrt(1 - std::pow(-2 * t + 2, 2)) + 1) / 2.0;
		    }
		}

	    // easeInOutElastic
		double easeInOutElastic(double t) {
		    const double c5 = (2 * M_PI) / 4.5;

		    if (t == 0.0) {
		        return 0.0;
		    } else if (t == 1.0) {
		        return 1.0;
		    } else if (t < 0.5) {
		        return -std::pow(2, 20 * t - 10) * std::sin((20 * t - 11.125) * c5) / 2;
		    } else {
		        return std::pow(2, -20 * t + 10) * std::sin((20 * t - 11.125) * c5) / 2 + 1.0;
		    }
		}

		// easeInQuad
		double easeInQuad(double t) {
		    return t * t;
		}

		// easeInQuart
		double easeInQuart(double t) {
		    return t * t * t * t;
		}

		// easeInExpo
		double easeInExpo(double t) {
		    if (t == 0) {
		        return 0;
		    } else {
		        return std::pow(2, 10 * t - 10);
		    }
		}

		// easeInBack
		double easeInBack(double t) {
		    const double c1 = 1.70158;
		    const double c3 = c1 + 1;

		    return c3 * t * t * t - c1 * t * t;
		}

		// easeInBounce
		double easeInBounce(double t) {
		    return 1 - easeOutBounce(1 - t);
		}

		// easeOutQuad
		double easeOutQuad(double t) {
		    return 1 - (1 - t) * (1 - t);
		}

		// easeOutQuart
		double easeOutQuart(double t) {
		    return 1 - std::pow(1 - t, 4);
		}

		// easeOutExpo
		double easeOutExpo(double t) {
		    if (t == 1) {
		        return 1;
		    } else {
		        return 1 - std::pow(2, -10 * t);
		    }
		}

		// easeOutBack
		double easeOutBack(double t) {
		    const double c1 = 1.70158;
		    const double c3 = c1 + 1;

		    return 1 + c3 * std::pow(t - 1, 3) + c1 * std::pow(t - 1, 2);
		}

		// easeOutBounce
		double easeOutBounce(double x) {
		    const double n1 = 7.5625;
		    const double d1 = 2.75;

		    if (x < 1 / d1) {
		        return n1 * x * x;
		    } else if (x < 2 / d1) {
		        x -= 1.5 / d1;
		        return n1 * x * x + 0.75;
		    } else if (x < 2.5 / d1) {
		        x -= 2.25 / d1;
		        return n1 * x * x + 0.9375;
		    } else {
		        x -= 2.625 / d1;
		        return n1 * x * x + 0.984375;
		    }
		}

		// easeInOutQuad
		double easeInOutQuad(double t) {
		    if (t < 0.5) {
		        return 2 * t * t;
		    } else {
		        return 1 - std::pow(-2 * t + 2, 2) / 2.0;
		    }
		}

		// easeInOutQuart
		double easeInOutQuart(double t) {
		    if (t < 0.5) {
		        return 8 * t * t * t * t;
		    } else {
		        return 1 - std::pow(-2 * t + 2, 4) / 2.0;
		    }
		}

		// easeInOutExpo
		double easeInOutExpo(double t) {
		    if (t == 0) {
		        return 0;
		    } else if (t == 1) {
		        return 1;
		    } else if (t < 0.5) {
		        return std::pow(2, 20 * t - 10) / 2.0;
		    } else {
		        return (2 - std::pow(2, -20 * t + 10)) / 2.0;
		    }
		}

		// easeInOutBack
		double easeInOutBack(double t) {
		    const double c1 = 1.70158;
		    const double c2 = c1 * 1.525;

		    if (t < 0.5) {
		        return (std::pow(2 * t, 2) * ((c2 + 1) * 2 * t - c2)) / 2.0;
		    } else {
		        return (std::pow(2 * t - 2, 2) * ((c2 + 1) * (t * 2 - 2) + c2) + 2) / 2.0;
		    }
		}

		// easeInOutBounce
		double easeInOutBounce(double t) {
		    if (t < 0.5) {
		        return (1 - easeOutBounce(1 - 2 * t)) / 2;
		    } else {
		        return (1 + easeOutBounce(2 * t - 1)) / 2;
		    }
		}
	};
}

void getRawData(ppm::Image& image, std::vector<uint8_t>& raw_data) {
    int width = image.getWidth();
    int height = image.getHeight();

    // Resize to accommodate the filter byte for each row
    raw_data.resize(width * height * 3 + height);

    for (int y = 0; y < height; ++y) {
        // Insert the filter byte at the start of each row
        raw_data[y * (width * 3 + 1)] = 0; // Filter byte

        for (int x = 0; x < width; ++x) {
            // Get the pixel from the image
            ppm::Pixel pixel = image.getPixel(x, y);
            
            // Map the pixel's RGB values to the range [0, 255]
            uint8_t r = static_cast<uint8_t>(std::get<0>(pixel) * 255.0);
            uint8_t g = static_cast<uint8_t>(std::get<1>(pixel) * 255.0);
            uint8_t b = static_cast<uint8_t>(std::get<2>(pixel) * 255.0);
            
            // Calculate the index in the raw data array, accounting for the filter byte
            int index = y * (width * 3 + 1) + x * 3 + 1;
            raw_data[index]     = r; // Red component
            raw_data[index + 1] = g; // Green component
            raw_data[index + 2] = b; // Blue component
        }
    }
}

int main() {
    constexpr int width = 120; // Overall image width
    constexpr int height = 120; // Overall image height
    constexpr int circleDiameter = 5; // Diameter of the circle
    constexpr int marginX = 10; // X margin for the circle's horizontal center
    constexpr int marginY = 10; // Y margin for the circle's starting vertical position
    constexpr int frames = 60; // Total number of frames in the animation

    int startX = width / 2; // X position for the center of the circle
    int startY = marginY + (circleDiameter / 2); // Y starting position
    int endY = marginY + (100 - (circleDiameter / 2)); // Y ending position
    int rangeY = startY - endY; // Vertical range for the circle movement
    std::vector<uint8_t> raw_data; // Vector to store the raw pixel data
    ppm::Image image(width, height);

    ease::Easing easing;

    try {
        APNG apng("26_animated_circle.png", width, height, frames, 0, 1, 30, 0, 0); // Initialize APNG

        for (int frame = 0; frame < frames; ++frame) {
            double t = static_cast<double>(frame) / (frames - 1); // Normalized time value for easing function
            double easedT = easing.easeOutBounce(t); // Get eased time value

            // Calculate the circle's vertical position based on easedT
            int circleY = startY - static_cast<int>(easedT * rangeY);
            image.setAllPixels(ppm::Pixel(0.0, 0.0, 0.0)); // Set background to black

            // Draw a white filled circle at the calculated position
            ppm::Point pt=std::make_tuple(startX, circleY);
            image.drawFilledCircle(pt, circleDiameter / 2, ppm::Pixel(1.0, 1.0, 1.0));

            getRawData(image,raw_data);

            bool is_first_frame = (frame == 0);
            apng.write_frame(raw_data, is_first_frame);
        }

        apng.close(); // Finalize the APNG file

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    std::cout << "Animation complete: 26_animated_circle.png" << std::endl;

    return 0;
}