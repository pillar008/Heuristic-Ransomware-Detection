#include "sentinel.hpp"
#include "terminator.hpp"
#include "analyzer.hpp" // <-- This must be here!
#include <iostream>
#include <iomanip>
#include <sys/inotify.h>
#include <unistd.h>
#include <limits.h>

#define MAX_EVENTS 1024
#define LEN_NAME 16
#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( MAX_EVENTS * ( EVENT_SIZE + LEN_NAME ))

void start_watchdog(const std::string& target_dir) {
    int fd, wd;
    char buffer[BUF_LEN];

    fd = inotify_init();
    if (fd < 0) return;

    wd = inotify_add_watch(fd, target_dir.c_str(), IN_MODIFY);
    if (wd == -1) return;

    std::cout << "[*] Activating Native Sentinel Watchdog...\n";
    std::cout << "[*] Securing Directory: " << target_dir << "\n";
    std::cout << "[*] Heuristic Engine LIVE (Kernel-Level). Waiting for attacks...\n\n";

    while (true) {
        int length = read(fd, buffer, BUF_LEN);
        if (length < 0) break;

        int i = 0;
        while (i < length) {
            struct inotify_event *event = (struct inotify_event *) &buffer[i];
            
            if (event->len && (event->mask & IN_MODIFY) && !(event->mask & IN_ISDIR)) {
                std::string full_path = target_dir + "/" + event->name;
                
                std::cout << "\n[!!!] KERNEL ALERT: DECOY TRIGGERED!\n";
                std::cout << "      -> Target: " << full_path << "\n";
                
                // --- THE NATIVE MATH ENGINE ---
                double entropy_score = calculate_entropy(full_path);
                
                std::cout << std::fixed << std::setprecision(4);
                std::cout << "      -> Current Entropy: " << entropy_score << "\n";
                
                if (entropy_score >= 7.5) {
                    std::cout << "      -> [DANGER] HIGH ENTROPY! Encryption Detected.\n";
                    std::cout << "      -> Action: Initiating Native Process Termination...\n";

                    // --- PULL THE TRIGGER ---
                    terminate_process(full_path);

                } else if (entropy_score < 5.5) {
                    std::cout << "      -> [INFO] Low Entropy. Likely benign text edit.\n";
                } else {
                    std::cout << "      -> [WARN] Moderate Entropy. Monitoring closely.\n";
                }
            }
            i += EVENT_SIZE + event->len;
        }
    }
    inotify_rm_watch(fd, wd);
    close(fd);
}
