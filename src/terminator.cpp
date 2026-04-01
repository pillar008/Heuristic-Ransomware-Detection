#include "terminator.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <csignal>
#include <algorithm>
#include <unistd.h>
#include <chrono>
#include <ctime>

namespace fs = std::filesystem;

// --- FEATURE 1: FORENSICS LOGGER ---
void log_event(const std::string& target, double entropy, pid_t pid, const std::string& p_name, bool success) {
    std::ofstream log_file("hids_alerts.log", std::ios::app);
    if (!log_file.is_open()) return;

    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::string time_str = std::ctime(&now_time);
    time_str.pop_back(); 

    log_file << "[" << time_str << "] ALERT | Target: " << target 
             << " | Entropy: " << entropy 
             << " | Malware PID: " << pid 
             << " (" << p_name << ") | Terminated: " << (success ? "YES" : "NO") << "\n";
    
    log_file.close();
    std::cout << "      [+] Forensics: Attack logged securely to hids_alerts.log\n";
}

// --- FEATURE 2: AUTO-RESTORE ---
void restore_decoy(const std::string& target_file_path) {
    std::cout << "      [*] Auto-Restore: Purging encrypted decoy...\n";
    fs::remove(target_file_path); 

    std::string dummy_text = 
        "This is a confidential document regarding internal network configurations.\n"
        "User: admin, Password: Password123!\n"
        "Database IP: 192.168.1.100, Port: 3306\n"
        "Please ensure this file is never deleted or encrypted.\n";
    
    std::string full_text = "";
    for(int i = 0; i < 50; i++) full_text += dummy_text;

    std::ofstream file(target_file_path);
    file << full_text;
    file.close();
    
    std::cout << "      [SUCCESS] Trap Reset: Fresh decoy deployed and ready.\n";
}

// --- THE MAIN ENFORCER LOGIC ---
void terminate_process(const std::string& target_file_path, double entropy_score) {
    std::cout << "      [*] Enforcer activated: Crawling /proc to find the attacker...\n";
    
    pid_t malicious_pid = -1;
    std::string process_name = "Unknown";

    for (const auto& entry : fs::directory_iterator("/proc")) {
        if (!entry.is_directory()) continue;
        
        std::string pid_str = entry.path().filename().string();
        bool is_pid = !pid_str.empty() && std::all_of(pid_str.begin(), pid_str.end(), ::isdigit);
        if (!is_pid) continue;

        std::string fd_dir = entry.path().string() + "/fd";
        std::error_code ec;
        if (!fs::exists(fd_dir, ec)) continue;

        try {
            for (const auto& fd_entry : fs::directory_iterator(fd_dir, ec)) {
                if (ec) continue;
                std::error_code sym_ec;
                fs::path target = fs::read_symlink(fd_entry.path(), sym_ec);
                
                if (!sym_ec && target.string() == target_file_path) {
                    malicious_pid = std::stoi(pid_str);
                    std::ifstream comm_file(entry.path().string() + "/comm");
                    if (comm_file.is_open()) {
                        std::getline(comm_file, process_name);
                    }
                    break;
                }
            }
        } catch (...) { continue; }

        if (malicious_pid != -1) break;
    }

    if (malicious_pid != -1) {
        std::cout << "      [!] Found Match! Process '" << process_name << "' (PID: " << malicious_pid << ") is attacking.\n";
        
        bool kill_success = (kill(malicious_pid, SIGKILL) == 0);
        if (kill_success) {
            std::cout << "      [SUCCESS] THREAT NEUTRALIZED! Process " << malicious_pid << " terminated.\n";
        } else {
            std::cerr << "      [FAIL] Could not terminate process " << malicious_pid << ".\n";
        }
        
        log_event(target_file_path, entropy_score, malicious_pid, process_name, kill_success);
        restore_decoy(target_file_path);

    } else {
        std::cout << "      [-] Could not locate active PID. The process may have exited.\n";
    }
}
