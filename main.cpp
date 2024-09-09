#include <iostream>
#include <string>
#include <vector>

class HashTable {
private:
    struct Slot {
        std::string key;
        enum Status { NEVER_USED, TOMBSTONE, OCCUPIED } status;
        Slot() : status(NEVER_USED) {}
    };
    
    std::vector<Slot> table;

    int hash(std::string key) const {
        return key.back() - 'a';
    }

public:
    HashTable() : table(26) {}

    void insert(std::string key) {
        int index = hash(key);
        while (true) {
            if (table.at(index).status != Slot::OCCUPIED || table.at(index).key == key) {
                table.at(index).key = key;
                table.at(index).status = Slot::OCCUPIED;
                break;
            }
            index = (index + 1) % 26;
        }
    }

    void remove(std::string key) {
        int index = hash(key);
        while (table.at(index).status != Slot::NEVER_USED) {
            if (table.at(index).status == Slot::OCCUPIED && table.at(index).key == key) {
                table.at(index).status = Slot::TOMBSTONE;
                break;
            }
            index = (index + 1) % 26;
        }
    }

    void print() {
        for (auto slot : table) {
            if (slot.status == Slot::OCCUPIED) {
                std::cout << slot.key << " ";
            }
        }
        std::cout << std::endl;
    }
};

int main() {
    HashTable ht;
    std::string input;
    
    std::getline(std::cin, input);
    
    size_t pos = 0;
    std::string token;
    while ((pos = input.find(" ")) != std::string::npos) {
        token = input.substr(0, pos);
        if (token.at(0) == 'A') {
            ht.insert(token.substr(1));
        } else if (token.at(0) == 'D') {
            ht.remove(token.substr(1));
        }
        input.erase(0, pos + 1);
    }
    
    if (!input.empty()) {
        if (input.at(0) == 'A') {
            ht.insert(input.substr(1));
        } else if (input.at(0) == 'D') {
            ht.remove(input.substr(1));
        }
    }
    
    ht.print();
    
    return 0;
}