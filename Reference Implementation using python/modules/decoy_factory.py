import os

def generate_decoys(target_dir):
    """
    Deploys low-entropy 'Honeyfiles' into the target directory.
    """
    print(f"[*] Initializing Decoy Factory at: {target_dir}")
    
    if not os.path.exists(target_dir):
        os.makedirs(target_dir)

    # Tempting file names for ransomware to target
    decoy_files = [
        "admin_passwords.txt",
        "financial_budget_2026.csv",
        "ssh_keys_backup.txt"
    ]

    # Low entropy dummy data (highly structured, repetitive text)
    dummy_text = (
        "This is a confidential document regarding internal network configurations.\n"
        "User: admin, Password: Password123!\n"
        "Database IP: 192.168.1.100, Port: 3306\n"
        "Please ensure this file is never deleted or encrypted.\n"
    ) * 50 # Multiply by 50 to give the file some weight

    for filename in decoy_files:
        filepath = os.path.join(target_dir, filename)
        
        if not os.path.exists(filepath):
            with open(filepath, 'w') as f:
                f.write(dummy_text)
            print(f"    [+] Deployed Honeyfile: {filename}")
        else:
            print(f"    [-] Honeyfile already exists: {filename}")
            
    print("[*] Decoy deployment complete.\n")
