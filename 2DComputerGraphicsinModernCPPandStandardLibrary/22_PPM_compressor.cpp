// Compile: clear && clang++ -std=c++20 -lz 22_PPM_compressor.cpp -o 22_PPM_compressor
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdint>
#include <string>
#include <filesystem>
#include <zlib.h>

class CPPM {
	private:
		std::ifstream file;
    	std::string m_filename="";
    	std::string m_writename="";
    	std::string m_suffix="";
    	std::vector<uint8_t> input;
    	std::vector<uint8_t> output;
    	uint32_t metadata[3];

		uint32_t crc32Calculation(const std::vector<uint8_t>& data) {
		    // Compute the CRC over the buffer
		    uint32_t crc = crc32(0, data.data(), data.size());
		    return crc;
		}

		uint32_t inflate() {
		    uLongf uncompressed_size = input.size();
		    int ret;
		    
		    // Increase the buffer size in a loop until uncompress succeeds or the buffer size exceeds a maximum value
		    for (int i = 0; i < 10; ++i) {
		        uncompressed_size *= 5;
		        output.resize(uncompressed_size);
		        ret = uncompress(output.data(), &uncompressed_size, input.data(), input.size());
		        
		        if (ret == Z_OK) {
		            output.resize(uncompressed_size);  // Resize to the actual uncompressed size
		            return uncompressed_size;
		        }
		    }
		    
		    std::cerr << "Inflation failed with error code: " << ret << std::endl;
		    return {};
		}

		uint32_t deflate() {
		    uLongf compressed_size = compressBound(input.size());
		    output.resize(compressed_size);
		    if (compress2(output.data(), &compressed_size, input.data(), input.size(), Z_BEST_COMPRESSION) != Z_OK) {
		        std::cerr << "Deflation failed." << std::endl;
		        return {};
		    }
		    output.resize(compressed_size);
		    return compressed_size;
		}

		bool readFileToVector() {
		    // Get file size
		    file.seekg(0, std::ios::end);
		    std::streamsize size = file.tellg();
		    file.seekg(0, std::ios::beg);

		    // Resize the vector and read the file into it
		    input.resize(size);
		    if (!file.read(reinterpret_cast<char*>(input.data()), size)) {
		    	close();
		        return false;
		    }
		    close();
		    return true;
		}

		bool writeVectorToFile() {
			std::ofstream ofs(m_writename+m_suffix, std::ios::binary);

		    // Check if the file was opened successfully
		    if (!ofs.is_open()) {
		        return false;
		    }

		    // Write the data to the file
		    ofs.write(reinterpret_cast<const char*>(output.data()), output.size());

		    // Close the file
		    ofs.close();

		    return true;
		}

		void concatenateData() {
		    // Reserve enough space in data
		    std::vector<uint8_t> data;
		    data.reserve(3 * sizeof(uint32_t) + output.size());

		    // Append metadata to data
		    for (int i = 0; i < 3; ++i) {
		        data.push_back(static_cast<uint8_t>(metadata[i] >> 24 & 0xFF));
		        data.push_back(static_cast<uint8_t>(metadata[i] >> 16 & 0xFF));
		        data.push_back(static_cast<uint8_t>(metadata[i] >> 8 & 0xFF));
		        data.push_back(static_cast<uint8_t>(metadata[i] & 0xFF));
		    }

		    // Append to output
		    data.insert(data.end(), output.begin(), output.end());
		    output.clear();
		    output.assign(data.begin(), data.end());
		}

		void extractDataFromVector() {
		    // Check if input has at least 12 bytes to extract metadata
		    if (input.size() < 12) {
		        std::cerr << "Extracting Data from Vector failed." << std::endl;
		        exit(1);
		    }

		    // Extract metadata
		    for (int i = 0; i < 3; ++i) {
		        metadata[i] = 0;
		        metadata[i] |= static_cast<uint32_t>(input[i * 4]) << 24;
		        metadata[i] |= static_cast<uint32_t>(input[i * 4 + 1]) << 16;
		        metadata[i] |= static_cast<uint32_t>(input[i * 4 + 2]) << 8;
		        metadata[i] |= static_cast<uint32_t>(input[i * 4 + 3]);
		    }

		    // Extract remaining data
		    std::vector<uint8_t> data;
		    data.assign(input.begin() + 12, input.end());
		    input.clear();
		    input.assign(data.begin(), data.end());
		}

		std::string filenameWithoutExtension(const std::string& path) {
		    std::filesystem::path p(path);
		    return p.stem().string();
		}

		std::string getFileSuffix(const std::string& path) {
		    std::filesystem::path p(path);
		    return p.extension().string();
		}

		bool compareFileSuffixes(const std::string& suffix) {
			if (suffix.compare(m_suffix) == 0) {
				return true;
			}
			return false;
		}

		bool fileExists(const std::string& filename) {
		    return std::filesystem::exists(std::filesystem::path(filename));
		}

		bool isP6() {
			std::string magic;
			if (input.size() >= 2) {
		        magic = std::string(1, static_cast<char>(input[0])) + std::string(1, static_cast<char>(input[1]));
		    }
			if ((magic.compare("P6") == 0)) {
				return true;
			}
			return false;
		}

		bool isCPPM() {
			std::string magic;
			if (input.size() >= 4) {
		        magic = std::string(1, static_cast<char>(input[0])) + 
				        std::string(1, static_cast<char>(input[1])) + 
				        std::string(1, static_cast<char>(input[2])) + 
				        std::string(1, static_cast<char>(input[3]));
		    }
			if ((magic.compare("CPPM") == 0)) {
				return true;
			}
			return false;
		}

		void close() {
	        if (file.is_open()) {
	            file.close();
	        }
	    }

	public:
		CPPM(const std::string& inFilename, const std::string& outFilename)
		: file(inFilename, std::ios::binary)
	    {
	        if (!file.is_open()) {
	            throw std::ios_base::failure("Failed to open file");
	        }
	        m_filename=filenameWithoutExtension(inFilename);
	        m_suffix=getFileSuffix(inFilename);
	        if (outFilename.empty()) {
	        	m_writename=m_filename;
	        } else {
	        	m_writename=filenameWithoutExtension(outFilename);
	        }
	        uint32_t magic = 0;
		    magic |= (static_cast<uint32_t>('C') << 24);
		    magic |= (static_cast<uint32_t>('P') << 16);
		    magic |= (static_cast<uint32_t>('P') << 8);
		    magic |= static_cast<uint32_t>('M');
		    metadata[0]=magic;

		    if (fileExists(inFilename)) {
		        if (!readFileToVector()) {
		        	std::cerr << "Reading file failed." << std::endl;
			        exit(1);
		        }
	    	}

	        if (compareFileSuffixes(".ppm")) {
	        	if (isP6()) {
	        		std::cout << "Compressing: " << m_filename << ".ppm" << std::endl;
	        		m_suffix=".cppm";
	        		uint32_t size=deflate();
	        		metadata[1]=size;
	        		metadata[2]=crc32Calculation(output);
	        		concatenateData();
	        		if (!writeVectorToFile()) {
	        			std::cerr << "Writing file failed." << std::endl;
		        		exit(1);
	        		}
	        		std::cout << "Writing: " << m_writename << ".cppm" << std::endl;
	        	}
	        }

	        if (compareFileSuffixes(".cppm")) {
	        	if (isCPPM()) {
	        		std::cout << "Decompressing: " << m_filename << ".cppm" << std::endl;
	        		m_suffix=".ppm";
	        		extractDataFromVector();
	        		uint32_t crc32=crc32Calculation(input);
	        		if ((magic == metadata[0]) && (input.size() == metadata[1]) && (crc32 == metadata[2])) {
	        			uint32_t size=inflate();
	        			if (!writeVectorToFile()) {
		        			std::cerr << "Writing file failed." << std::endl;
			        		exit(1);
		        		}
	        		}
	        		std::cout << "Writing: " << m_writename << ".ppm" << std::endl;
	        	}
	        }

	        close();
	        input.clear();
	        output.clear();
	    }
};

int main(int argc, const char * argv[]) {
	std::cout << "Usage: ./22_PPM_compressor <inputName>.<ppm/cppm> (optional: <outputName>.<ppm/cppm>)" << std::endl;
	if (argc == 2) {
		std::string fname = argv[1];
		CPPM cppm(fname,"");
	} else if (argc == 3) {
		std::string fname = argv[1];
		std::string wname = argv[2];
		CPPM cppm(fname,wname);
	}
	
	return 0;
}