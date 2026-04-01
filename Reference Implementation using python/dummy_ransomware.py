import os
import time

target_file = os.path.expanduser("~/Heuristic-Ransomware-Detection/decoys/admin_passwords.txt")

print(f"[*] Dummy Ransomware Started. PID: {os.getpid()}")
print(f"[*] Attempting to encrypt: {target_file}")

# Open the file and hold it open while we "encrypt" it
try:
    with open(target_file, 'wb') as f:
        # Write high entropy random bytes
        f.write(os.urandom(4096))
        print("[*] Data written. Holding file handle open for 10 seconds...")
        # Sleep so the HIDS has time to catch us and kill this process
        time.sleep(10)
        print("[-] If you see this, the HIDS failed to kill me!")
except Exception as e:
    print(f"[!] Error: {e}")
