#!/bin/bash

# Create 'build' directory if it doesn't exist
if [ ! -d "build" ]; then
  mkdir build
fi

# Navigate into 'build' directory
cd build

# Clean previous build artifacts
make clean

# Generate build files with CMake
cmake ..

# Compile the project
make

# Change into the 'bin' directory where executables are located
cd bin

# Start an interactive loop for listing and running programs
while true; do
  # Display available programs
  echo "Available programs:"
  ls -1

  # Prompt user for program name or exit command
  echo "Enter the name of the program to run (or 'exit' to quit):"
  read program_name

  # Convert input to lowercase for case-insensitive comparison
  lower_input=$(echo "$program_name" | tr 'A-Z' 'a-z')

  # Check if the user wants to exit
  if [ "$lower_input" = "exit" ]; then
    echo "Exiting."
    break
  fi

  # Verify if the specified program exists and is executable
  if [ -f "$program_name" ] && [ -x "$program_name" ]; then
    # List available network interfaces
    echo "Available network interfaces:"
    ip -o link show | awk -F': ' '{print $2}'

    # Prompt user to select a network interface
    echo "Enter the network interface you want to use (e.g., eth0):"
    read net_interface

    # Optional: Validate if the entered interface exists
    if ip link show "$net_interface" > /dev/null 2>&1; then
      # Run the program with the chosen network interface
      ./$(basename "$program_name") "$net_interface"
    else
      echo "Network interface '$net_interface' not found. Please try again."
    fi
  else
    # Inform user if program not found or not executable
    echo "Program '$program_name' not found or not executable."
  fi
done