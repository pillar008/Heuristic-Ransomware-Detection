import os
from modules.decoy_factory import generate_decoys
from modules.sentinel import start_watchdog

# Define the absolute path to our trap folder
DECOY_DIR = os.path.expanduser("~/Heuristic-Ransomware-Detection/decoys")

if __name__ == "__main__":
    print("===================================================")
    print(" HEURISTIC RANSOMWARE DETECTION SYSTEM")
    print("===================================================\n")
    
    # Step 1: Plant the traps
    generate_decoys(DECOY_DIR)
    
    # Step 2: Start the security cameras
    start_watchdog(DECOY_DIR)
