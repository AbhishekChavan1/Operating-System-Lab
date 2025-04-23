#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdio>
using namespace std;

bool file_exists(const string &filename) {
    ifstream file(filename);
    return file.good();
}

void execute_cp(const string &command) {
    stringstream ss(command);
    string cmd, src, dest;
    ss >> cmd >> src >> dest;
    if (cmd != "cp") {
        cerr << "Error: Invalid command. Use 'cp source_file destination_file'.\n";
        return;
    }
    if (src.empty() || dest.empty()) {
        cerr << "Error: Missing source or destination file.\n";
        return;
    }
    if (!file_exists(src)) {
        cerr << "Error: Source file '" << src << "' does not exist.\n";
        return;
    }
    ifstream src_file(src, ios::binary);
    ofstream dest_file(dest, ios::binary);
    if (!src_file.is_open() || !dest_file.is_open()) {
        cerr << "Error: Failed to open source or destination file.\n";
        return;
    }
    dest_file << src_file.rdbuf(); // Copy contents
    cout << "File copied successfully from '" << src << "' to '" << dest << "'.\n";
}

void execute_grep(const string &command) {
    stringstream ss(command);
    string cmd, pattern, file;
    ss >> cmd >> ws;
    getline(ss, pattern, ' '); // Extract pattern
    getline(ss, file);         // Extract file name
    if (cmd != "grep") {
        cerr << "Error: Invalid command. Use 'grep pattern filename'.\n";
        return;
    }
    if (pattern.empty() || file.empty()) {
        cerr << "Error: Missing pattern or file name.\n";
        return;
    }
    if (!file_exists(file)) {
        cerr << "Error: File '" << file << "' does not exist.\n";
        return;
    }
    ifstream infile(file);
    string line;
    bool found = false;
    while (getline(infile, line)) {
        if (line.find(pattern) != string::npos) {
            cout << line << endl;
            found = true;
        }
    }
    if (!found) {
        cerr << "Error: Pattern '" << pattern << "' not found in file '" << file << "'.\n";
    }
}

int main() {
    int choice;
    pid_t pid;
    while (true) {
        cout << "\nMenu:\n1. fork\n2. wait\n3. execlp\n4. exit\n5. cp (copy file)\n";
        cout << "6. grep (search pattern in file)\n7. getpid and getppid\n8. Quit\nEnter your choice: ";
        cin >> choice;
        cin.ignore(); 
        switch(choice) {
            case 1: pid = fork();
                    if (pid == 0) {
                        cout << "Child process created. PID: " << getpid() << ", PPID: " << getppid() << "\n";
                    } else if (pid > 0) {
                        cout << "Parent process. PID: " << getpid() << ", Child PID: " << pid << "\n";
                    } else {
                        cerr << "Fork failed!\n";
                    }
                    break;
            case 2: pid = fork();
                    if (pid == 0) {
                        cout << "Child process running. PID: " << getpid() << "\n";
                        exit(0);
                    } else {
                        int status;
                        wait(&status);
                        cout << "Child process completed. Exit status: " << WEXITSTATUS(status) << "\n";
                    }
                    break;
            case 3: pid = fork();
                    if (pid == 0) {
                        cout << "Executing ls command with execlp.\n";
                        execlp("ls", "ls", "-l", nullptr);
                        cerr << "execlp failed!\n";
                    } else {
                        wait(nullptr);
                    }
                    break;
            case 4: cout << "Exiting the program.\n";
                    exit(0);
            case 5: { string command;
                    cout << "Enter the cp command (e.g., 'cp source.txt destination.txt'): ";
                    getline(cin, command);
                    execute_cp(command);
                    break;
                }
            case 6: { string command;
                    cout << "Enter the grep command (e.g., 'grep pattern filename'): ";
                    getline(cin, command);
                    execute_grep(command);
                    break;  }
            case 7: cout << "Current PID: " << getpid() << "\n";
                    cout << "Parent PID: " << getppid() << "\n";
                    break;
            case 8: cout << "Quitting the program.\n";
                    return 0;
            default:cerr << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}