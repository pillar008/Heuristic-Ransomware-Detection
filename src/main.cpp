#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <cstdlib>
#include "sentinel.hpp" // <-- Linking your Kernel Hook

namespace fs = std::filesystem;

void generate_decoys(const std::string& target_dir) {
    std::cout << "[*] Initializing Native C++ Decoy Factory at: " << target_dir << "\n";

    if (!fs::exists(target_dir)) {
        fs::create_directories(target_dir);
    }

    std::vector<std::string> decoy_files = {
        "admin_passwords.txt",
        "financial_budget_2026.csv",
        "ssh_keys_backup.txt"
    };

    std::string dummy_text = 
        "This is a confidential document regarding internal network configurations.\n"
        "User: admin, Password: Password123!\n"
        "Database IP: 192.168.1.100, Port: 3306\n"
        "Please ensure this file is never deleted or encrypted.\n";
    
    std::string full_text = "";
    for(int i = 0; i < 50; i++) {
        full_text += dummy_text;
    }

    for (const auto& filename : decoy_files) {
        fs::path filepath = fs::path(target_dir) / filename;
        
        if (!fs::exists(filepath)) {
            std::ofstream file(filepath);
            file << full_text;
            file.close();
            std::cout << "    [+] Deployed Honeyfile: " << filename << "\n";
        } else {
            std::cout << "    [-] Honeyfile already exists: " << filename << "\n";
        }
    }
    std::cout << "[*] Native Decoy deployment complete.\n\n";
}

int main() {
    std::cout << "===================================================\n";
    std::cout << " HEURISTIC RANSOMWARE DETECTION (C++ CORE)\n";
    std::cout << "===================================================\n\n";

    const char* home_dir = getenv("HOME");
    if (home_dir == nullptr) {
        std::cerr << "[!] Error: Could not locate HOME directory.\n";
        return 1;
    }

    std::string target_dir = std::string(home_dir) + "/Heuristic-Ransomware-Detection/decoys";
    
    // 1. Deploy the traps
    generate_decoys(target_dir);

    // 2. Activate the kernel-level sensor
    start_watchdog(target_dir);

    return 0;
}
