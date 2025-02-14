#include <iostream>
#include <fstream>
#include <unordered_map>
#include <iomanip>
#include <sstream>
#include <functional>

using namespace std;

struct User {
    string username;
    string password_hash;
    double balance;
};

unordered_map<string, User> users; // In-memory storage for users

// Function to hash passwords using std::hash
string hashPassword(const string &password) {
    hash<string> hasher;
    size_t hashed_value = hasher(password);
    stringstream ss;
    ss << hex << hashed_value;
    return ss.str();
}

// Function to save users to a file
void saveUsers() {
    ofstream file("users.txt");
    for (const auto &pair : users) {
        file << pair.second.username << " " << pair.second.password_hash << " " << pair.second.balance << "\n";
    }
    file.close();
}

// Function to load users from a file
void loadUsers() {
    ifstream file("users.txt");
    if (!file.is_open()) return;
    User user;
    while (file >> user.username >> user.password_hash >> user.balance) {
        users[user.username] = user;
    }
    file.close();
}

// Function to register a new user
void registerUser() {
    User newUser;
    cout << "Enter username: ";
    cin >> newUser.username;
    if (users.find(newUser.username) != users.end()) {
        cout << "User already exists! Try logging in.\n";
        return;
    }
    cout << "Enter password: ";
    string password;
    cin >> password;
    newUser.password_hash = hashPassword(password);
    newUser.balance = 0;
    users[newUser.username] = newUser;
    saveUsers();
    cout << "User registered successfully!\n";
}

// Function to login
User* loginUser() {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;
    
    if (users.find(username) == users.end() || users[username].password_hash != hashPassword(password)) {
        cout << "Invalid credentials!\n";
        return nullptr;
    }
    cout << "Login successful!\n";
    return &users[username];
}

// Function to deposit money
void deposit(User *user) {
    double amount;
    cout << "Enter deposit amount: ";
    cin >> amount;
    if (amount <= 0) {
        cout << "Invalid amount!\n";
        return;
    }
    user->balance += amount;
    saveUsers();
    cout << "Deposit successful! New balance: " << fixed << setprecision(2) << user->balance << "\n";
}

// Function to withdraw money
void withdraw(User *user) {
    double amount;
    cout << "Enter withdrawal amount: ";
    cin >> amount;
    if (amount <= 0 || amount > user->balance) {
        cout << "Invalid amount or insufficient balance!\n";
        return;
    }
    user->balance -= amount;
    saveUsers();
    cout << "Withdrawal successful! New balance: " << fixed << setprecision(2) << user->balance << "\n";
}

// Function to check balance
void checkBalance(User *user) {
    cout << "Current balance: " << fixed << setprecision(2) << user->balance << "\n";
}

int main() {
    loadUsers();
    int choice;
    User *loggedInUser = nullptr;
    
    while (true) {
        cout << "\n1. Register\n2. Login\n3. Exit\nEnter your choice: ";
        cin >> choice;
        if (choice == 1) {
            registerUser();
        } else if (choice == 2) {
            loggedInUser = loginUser();
            if (loggedInUser) {
                while (true) {
                    cout << "\n1. Deposit\n2. Withdraw\n3. Check Balance\n4. Logout\nEnter your choice: ";
                    cin >> choice;
                    if (choice == 1) deposit(loggedInUser);
                    else if (choice == 2) withdraw(loggedInUser);
                    else if (choice == 3) checkBalance(loggedInUser);
                    else if (choice == 4) break;
                    else cout << "Invalid choice!\n";
                }
            }
        } else if (choice == 3) {
            cout << "Exiting...\n";
            break;
        } else {
            cout << "Invalid choice!\n";
        }
    }
    return 0;
}
/*#include <iostream>
#include <fstream>
#include <unordered_map>
#include <iomanip>
#include <openssl/sha.h> // OpenSSL library for hashing

using namespace std;

struct User {
    string username;
    string password_hash;
    double balance;
};

unordered_map<string, User> users; // In-memory storage for users

// Function to hash passwords using SHA-256
string hashPassword(const string &password) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *)password.c_str(), password.length(), hash);
    string hashed;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        hashed += to_string(hash[i]);
    }
    return hashed;
}

// Function to save users to a file
void saveUsers() {
    ofstream file("users.txt");
    for (const auto &pair : users) {
        file << pair.second.username << " " << pair.second.password_hash << " " << pair.second.balance << "\n";
    }
    file.close();
}

// Function to load users from a file
void loadUsers() {
    ifstream file("users.txt");
    if (!file.is_open()) return;
    User user;
    while (file >> user.username >> user.password_hash >> user.balance) {
        users[user.username] = user;
    }
    file.close();
}

// Function to register a new user
void registerUser() {
    User newUser;
    cout << "Enter username: ";
    cin >> newUser.username;
    if (users.find(newUser.username) != users.end()) {
        cout << "User already exists! Try logging in.\n";
        return;
    }
    cout << "Enter password: ";
    string password;
    cin >> password;
    newUser.password_hash = hashPassword(password);
    newUser.balance = 0;
    users[newUser.username] = newUser;
    saveUsers();
    cout << "User registered successfully!\n";
}

// Function to login
User* loginUser() {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;
    
    if (users.find(username) == users.end() || users[username].password_hash != hashPassword(password)) {
        cout << "Invalid credentials!\n";
        return nullptr;
    }
    cout << "Login successful!\n";
    return &users[username];
}

// Function to deposit money
void deposit(User *user) {
    double amount;
    cout << "Enter deposit amount: ";
    cin >> amount;
    if (amount <= 0) {
        cout << "Invalid amount!\n";
        return;
    }
    user->balance += amount;
    saveUsers();
    cout << "Deposit successful! New balance: " << fixed << setprecision(2) << user->balance << "\n";
}

// Function to withdraw money
void withdraw(User *user) {
    double amount;
    cout << "Enter withdrawal amount: ";
    cin >> amount;
    if (amount <= 0 || amount > user->balance) {
        cout << "Invalid amount or insufficient balance!\n";
        return;
    }
    user->balance -= amount;
    saveUsers();
    cout << "Withdrawal successful! New balance: " << fixed << setprecision(2) << user->balance << "\n";
}

// Function to check balance
void checkBalance(User *user) {
    cout << "Current balance: " << fixed << setprecision(2) << user->balance << "\n";
}

int main() {
    loadUsers();
    int choice;
    User *loggedInUser = nullptr;
    
    while (true) {
        cout << "\n1. Register\n2. Login\n3. Exit\nEnter your choice: ";
        cin >> choice;
        if (choice == 1) {
            registerUser();
        } else if (choice == 2) {
            loggedInUser = loginUser();
            if (loggedInUser) {
                while (true) {
                    cout << "\n1. Deposit\n2. Withdraw\n3. Check Balance\n4. Logout\nEnter your choice: ";
                    cin >> choice;
                    if (choice == 1) deposit(loggedInUser);
                    else if (choice == 2) withdraw(loggedInUser);
                    else if (choice == 3) checkBalance(loggedInUser);
                    else if (choice == 4) break;
                    else cout << "Invalid choice!\n";
                }
            }
        } else if (choice == 3) {
            cout << "Exiting...\n";
            break;
        } else {
            cout << "Invalid choice!\n";
        }
    }
    return 0;
}*/