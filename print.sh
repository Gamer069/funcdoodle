#!/bin/zsh

# Define the root directory
root_directory="src/"

# Function to recursively print file names and contents
print_file_contents() {
    for file in "$1"/*; do
        if [[ -d "$file" ]]; then
            # If it's a directory, recursively call the function
            print_file_contents "$file"
        elif [[ -f "$file" ]]; then
            # If it's a file, print its name and contents
            echo "============= $(basename "$file") ==============="
            cat "$file"
            echo
        fi
    done
}

# Call the function with the root directory
print_file_contents "$root_directory"
