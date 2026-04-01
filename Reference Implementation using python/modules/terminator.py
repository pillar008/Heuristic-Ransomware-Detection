import psutil
import os
import signal

def terminate_process(target_file_path):
    """
    Scans running processes to find the one holding an open file handle
    to the target file, and forcefully terminates it.
    """
    print(f"      [*] Enforcer activated: Searching for PID attacking {target_file_path}...")
    
    malicious_pid = None
    
    # Iterate through all running processes
    for proc in psutil.process_iter(['pid', 'name']):
        try:
            # Check if the process has open files
            open_files = proc.open_files()
            if open_files:
                for file in open_files:
                    if file.path == target_file_path:
                        malicious_pid = proc.info['pid']
                        process_name = proc.info['name']
                        print(f"      [!] Found Match! Process '{process_name}' (PID: {malicious_pid}) is attacking.")
                        break
        except (psutil.NoSuchProcess, psutil.AccessDenied, psutil.ZombieProcess):
            # Ignore OS processes we don't have permission to read
            pass
        
        if malicious_pid:
            break
            
    if malicious_pid:
        try:
            # Send the kill signal directly to the kernel
            os.kill(malicious_pid, signal.SIGKILL)
            print(f"      [SUCCESS] THREAT NEUTRALIZED! Process {malicious_pid} terminated.")
        except Exception as e:
            print(f"      [FAIL] Could not terminate process {malicious_pid}: {e}")
    else:
        print(f"      [-] Could not locate active PID. The process may have already exited.")
