import math
import os

def calculate_entropy(file_path):
    """
    Reads a file in binary mode and calculates its Shannon Entropy.
    Returns a float between 0.0 (no randomness) and 8.0 (total randomness/encrypted).
    """
    try:
        # Check if file is empty to avoid division by zero
        file_size = os.path.getsize(file_path)
        if file_size == 0:
            return 0.0

        # Read the file's raw bytes
        with open(file_path, 'rb') as f:
            byte_data = f.read()

        # Create an array to count the occurrence of each of the 256 possible bytes
        byte_counts = [0] * 256
        for byte in byte_data:
            byte_counts[byte] += 1

        # Apply the Shannon Entropy Formula: H(X) = -sum(P(x) * log2(P(x)))
        entropy = 0.0
        for count in byte_counts:
            if count > 0:
                probability = count / file_size
                entropy -= probability * math.log2(probability)

        return entropy

    except Exception as e:
        print(f"      [ERR] Analyzer failed to read file: {e}")
        return 0.0
