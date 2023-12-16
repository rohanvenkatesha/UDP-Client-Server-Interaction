#!/bin/bash

# Kill the server
fuser -k -n udp 12345

# Directory containing test case files
TEST_CASE_DIR="testcases"

# Directory to store log files
LOG_DIR="testlogs"

# Function to run the client-server application with a test case file
run_test_case() {
    local test_case_file="$1"
    local client_log_file="$LOG_DIR/test_log_client_$(basename "$test_case_file").log"
    local server_log_file="$LOG_DIR/test_log_server_$(basename "$test_case_file").log"

    mkdir -p "$LOG_DIR"  # Create the log directory if it doesn't exist

    echo "[$(date '+%Y-%m-%d %H:%M:%S')] Running test case from file: $test_case_file" >> "$client_log_file"

    ./server &  # Start server in the background
    sleep 1

    ./client "$test_case_file" >> "$client_log_file"  # Redirect client output to the specific log file
    sleep 1

    #fuser -k -n udp 12345  # Stop the server using fuser

    echo "[$(date '+%Y-%m-%d %H:%M:%S')] Client Test case completed" >> "$client_log_file"
    echo "[$(date '+%Y-%m-%d %H:%M:%S')] Server Test case completed" >> "$server_log_file"
}

# Iterate through test case files in the directory
for test_case_file in "$TEST_CASE_DIR"/*; do
    if [ -f "$test_case_file" ]; then
        run_test_case "$test_case_file"
    fi
done


# Kill the server after all test cases are completed
fuser -k -n udp 12345

echo "All test cases completed, and the server is killed"
