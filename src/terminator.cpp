#include "terminator.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <csignal>
#include <algorithm>
#include <unistd.h>

namespace fs = std::filesystem;

void terminate_process(const std::string& target_file_path) {
    std::cout << "      [*] Enforcer activated: Crawling /proc to find the attacker...\n";
    
    pid_t malicious_pid = -1;
    std::string process_name = "Unknown";

    // Iterate through all folders in the /proc directory
    for (const auto& entry : fs::directory_iterator("/proc")) {
        if (!entry.is_directory()) continue;
        
        std::string pid_str = entry.path().filename().string();
        
        // Check if the directory name is purely numeric (which means it's a Process ID)
        bool is_pid = !pid_str.empty() && std::all_of(pid_str.begin(), pid_str.end(), ::isdigit);
        if (!is_pid) continue;

        std::string fd_dir = entry.path().string() + "/fd";
        
        // We might not have permission to read all process file descriptors (e.g., root processes)
        std::error_code ec;
        if (!fs::exists(fd_dir, ec)) continue;

        try {
            // Read every open file descriptor for this specific process
            for (const auto& fd_entry : fs::directory_iterator(fd_dir, ec)) {
                if (ec) continue;
                
                std::error_code sym_ec;
                fs::path target = fs::read_symlink(fd_entry.path(), sym_ec);
                
                // If the file descriptor points to our decoy, we caught the malware!
                if (!sym_ec && target.string() == target_file_path) {
                    malicious_pid = std::stoi(pid_str);
                    
                    // Grab the actual name of the malware from /proc/[pid]/comm
                    std::ifstream comm_file(entry.path().string() + "/comm");
                    if (comm_file.is_open()) {
                        std::getline(comm_file, process_name);
                    }
                    break;
                }
            }
        } catch (...) {
            // Silently ignore processes we don't have permission to scan
            continue;
        }

        if (malicious_pid != -1) break;
    }

    if (malicious_pid != -1) {
        std::cout << "      [!] Found Match! Process '" << process_name << "' (PID: " << malicious_pid << ") is attacking.\n";
        
        // Send the native SIGKILL signal directly to the Linux Kernel
        if (kill(malicious_pid, SIGKILL) == 0) {
            std::cout << "      [SUCCESS] THREAT NEUTRALIZED! Process " << malicious_pid << " terminated.\n";
        } else {
            std::cerr << "      [FAIL] Could not terminate process " << malicious_pid << " (Permission denied?).\n";
        }
    } else {
        std::cout << "      [-] Could not locate active PID. The process may have exited before trace completed.\n";
    }
}
