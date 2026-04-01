import time
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler
from modules.analyzer import calculate_entropy
from modules.terminator import terminate_process  # <-- NEW IMPORT

class DecoyMonitor(FileSystemEventHandler):
    def on_modified(self, event):
        if event.is_directory:
            return

        print(f"\n[!!!] ALERT: DECOY TRIGGERED!")
        print(f"      -> Target: {event.src_path}")
        
        entropy_score = calculate_entropy(event.src_path)
        print(f"      -> Current Entropy: {entropy_score:.4f}")
        
        if entropy_score >= 7.5:
            print(f"      -> [DANGER] HIGH ENTROPY! Encryption Detected.")
            print(f"      -> Action: Initiating Process Termination...")
            
            # --- TRIGGER THE KILL SWITCH ---
            terminate_process(event.src_path)
            
        elif entropy_score < 5.5:
            print(f"      -> [INFO] Low Entropy. Likely benign text edit.")
        else:
            print(f"      -> [WARN] Moderate Entropy. Monitoring closely.")
            
def start_watchdog(target_dir):
    print(f"[*] Activating Sentinel Watchdog...")
    print(f"[*] Securing Directory: {target_dir}")
    print(f"[*] Heuristic Engine is LIVE. Waiting for attacks... (Press Ctrl+C to stop)\n")

    event_handler = DecoyMonitor()
    observer = Observer()
    observer.schedule(event_handler, path=target_dir, recursive=False)
    observer.start()

    try:
        while True:
            time.sleep(1) 
    except KeyboardInterrupt:
        print("\n[*] Shutting down Sentinel...")
        observer.stop()
    observer.join()
