#pragma once
#include <string>

// Scans /proc to find the PID attacking our file and kills it
void terminate_process(const std::string& target_file_path, double entropy_score);


