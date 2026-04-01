#include "analyzer.hpp"
#include <fstream>
#include <vector>
#include <cmath>
#include <iostream>

double calculate_entropy(const std::string& file_path) {
    // Open file in binary mode, starting at the end to get the size
    std::ifstream file(file_path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "      [ERR] Analyzer failed to open file.\n";
        return 0.0;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg); // Rewind to the beginning

    if (size == 0) return 0.0;

    // Read the entire file into a high-speed buffer
    std::vector<unsigned char> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        return 0.0;
    }

    // Array to hold the frequency of all 256 possible bytes
    std::vector<int> byte_counts(256, 0);
    for (unsigned char byte : buffer) {
        byte_counts[byte]++;
    }

    // Shannon Entropy Formula: H(X) = -sum(P(x) * log2(P(x)))
    double entropy = 0.0;
    for (int count : byte_counts) {
        if (count > 0) {
            double probability = static_cast<double>(count) / size;
            entropy -= probability * std::log2(probability);
        }
    }

    return entropy;
}
