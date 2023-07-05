# Simple script to generate 3 txt file containing random ASCII characters, of given filesize in MB
import random
import string
import sys

def main():
    if len(sys.argv) != 2:
        print("Usage: python3 input_gen.py <file_size>")
        return

    # Generate a random ASCII character (both letters and numbers and symbols)
    ascii_characters = string.ascii_letters + string.digits + string.punctuation

    to_write = ""
    for i in range(int(sys.argv[1]) * 1024 ** 2):
        to_write += random.choice(ascii_characters)

    with open(f"{sys.argv[1]}MB.txt", 'w') as f:
        f.write(to_write)

if __name__ == "__main__":
    main()