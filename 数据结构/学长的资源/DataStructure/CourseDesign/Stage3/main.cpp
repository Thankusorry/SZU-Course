#include<iostream>
#include <stdexcept>
#include<sys/stat.h>
#include "fcntl.h"
#include <unistd.h>
#include<unordered_map>
#include<algorithm>
#include<ctime>
#include<vector>

using std::string;

const int KVDB_ERROR = -1;
const int KVDB_OK = 0;
const int KVDB_INVALID_AOF_PATH = 1;
const int KVDB_INVALID_KEY = 2;
const int KVDB_INVALID_VALUE = 3;
const int KVDB_NO_SPACE_LEFT_ON_DEVICES = 4;
const int KVDB_NO_FILE = 5;
const int KVDB_FILE_WRITE_ERROR = 6;
const int KVDB_IO_ERROR = 7;
const int INDEX_OK = 9;
const int INDEX_ERROR = 10;

struct Survival {
    long long time = 0;
    string key;

    bool operator<(const Survival &a) const {
        return time > a.time;
    }
};

struct LRU_Node {
    string value;
    LRU_Node *prior = NULL;
    LRU_Node *next = NULL;
};

class LRU {
    std::unordered_map<std::string, LRU_Node *> cache;
    LRU_Node *head = NULL;
    LRU_Node *tail = NULL;
    int volume = 3;
    int count = 0;
public:
    LRU(int volume = 10) : volume(volume) {}

    ~LRU() {}

    void Set(const std::string &key, const std::string &value) {
        if (cache[key] == NULL)
            cache[key] = new LRU_Node();
        cache[key]->value = value;
        if (head) {
            head->prior = cache[key];
            cache[key]->next = head;
        }
        head = cache[key];
        if (tail == NULL)
            tail = cache[key];
        if (++count > volume) {
            LRU_Node *temp = tail->prior;
            delete tail;
            tail = temp;
        }
    }

    bool Get(const std::string &key, std::string &value) {
        if (cache.find(key) == cache.end())
            return false;
        value = cache[key]->value;
        if (cache[key] == head)
            return true;
        if (cache[key] != tail && cache[key] != head) {
            cache[key]->next->prior = cache[key]->prior;
            cache[key]->prior->next = cache[key]->next;
        } else {
            tail = tail->prior;
        }
        cache[key]->next = head;
        head = cache[key];
        return true;
    }

    bool Del(const std::string &key) {
        if (cache.find(key) == cache.end())
            return false;
        if(cache[key]==head){
            head=head->next;
        }else if(cache[key]==tail){
            tail=tail->prior;
        }
        delete cache[key];
        cache.erase(key);
        return true;
    }
};

class KVDBHandler {
public:
    int database_address = -1;
    string database_path;
    std::unordered_map<std::string, size_t> index;
    std::vector<Survival> life;
    LRU LRUCache;
    KVDBHandler(const string &database_path) : database_path(database_path) {
        OpenKVDB();
        LoadIndex();
    }

    ~KVDBHandler() {
        CloseKVDB();
    }

    int LoadIndex() {
        index.clear();
        const off_t end = lseek(database_address, 0, SEEK_END);
        off_t cur = lseek(database_address, 0, SEEK_SET);
        size_t key_length = 0;
        int value_length = 0;
        string key;
        while (end != cur) {
            read(database_address, &key_length, sizeof(key_length));
            key.resize(key_length);
            read(database_address, (char *) key.data(), key_length);
            index[key] = lseek(database_address, 0, SEEK_CUR);
            read(database_address, &value_length, sizeof(value_length));
            if (value_length == -1)
                value_length = 0;
            cur = lseek(database_address, value_length, SEEK_CUR);
        }
        return INDEX_OK;
    }

    int OpenKVDB() {
        this->database_address = open(this->database_path.c_str(), O_CREAT | O_RDWR, S_IRWXU);
        if (0 > this->database_address) {
            printf("Failed to open DB file. [file='%s' res=%d]", this->database_path.c_str(), this->database_address);
            return KVDB_ERROR;
        }
        return KVDB_OK;
    }

    int CloseKVDB() {
        if (0 <= database_address) {
            close(database_address);
            return KVDB_OK;
        }
        return KVDB_ERROR;
    }

    int Set(const string &key, const string &value) {
        for (int i = 0; i < life.size(); i++) {        // refresh the life of the key
            if (life[i].key == key)
                life.erase(life.begin() + i);
        }
        LRUCache.Set(key,value);         // load LRUCache
        size_t key_length = key.length();     // write in the file
        int value_length = value.length();
        write(database_address, &key_length, sizeof(key_length));
        write(database_address, (char *) key.c_str(), key.length());
        index[key] = lseek(database_address, 0, SEEK_CUR);
        write(database_address, &value_length, sizeof(value_length));
        write(database_address, (char *) value.c_str(), value.length());
        return KVDB_OK;
    }

    int Del(const std::string &key) {
        if (index.find(key) == index.end()) {     // check index
            printf("Sorry, No Found!\n");
            return KVDB_IO_ERROR;
        }
        index.erase(key);                // refresh index
        for (int i = 0; i < life.size(); i++) {     //refresh the key_life
            if (life[i].key == key){
                life.erase(life.begin() + i);
            }
        }
        LRUCache.Del(key);         // refresh LRUCache
        size_t key_length = 0;     // write in the file
        int value_length = 0;
        key_length = key.length();
        value_length = -1;
        write(database_address, &key_length, sizeof(key_length));
        write(database_address, (char *) key.c_str(), key.length());
        write(database_address, &value_length, sizeof(value_length));
        return KVDB_OK;
    }

    int Get(const string &key, string &value) {
        if (index.find(key) == index.end()) {      // check index
            printf("Sorry, No Found!\n");
            return KVDB_IO_ERROR;
        }
        time_t current_time;
        time(&current_time);
        while (!life.empty() && life[0].time < current_time) {    // check key_life
            Del(life[0].key);
        }
        if (index.find(key) == index.end()) {      // check index after clear the dying
            printf("Sorry, what you want to find has been deleted!\n");
            return KVDB_IO_ERROR;
        }
        if(LRUCache.Get(key,value))  // search in the LRUCache
            return KVDB_OK;
        lseek(database_address, index[key], SEEK_SET);
        int value_length = 0;
        read(database_address, &value_length, sizeof(value_length));
        value.resize(value_length);
        read(database_address, (char *) value.data(), value_length);
        return KVDB_OK;
    }

    int Clear() {
        CloseKVDB();
        if (unlink(database_path.c_str()) < 0) {
            perror(database_path.c_str());
            return KVDB_ERROR;
        }
        return KVDB_OK;
    }

    int Purge();

    int Expire(const string &key, const int &n) {
        if (index.find(key) == index.end()) {
            printf("Sorry, No Found!\n");
            return KVDB_IO_ERROR;
        }
        time_t seconds;
        time(&seconds);
        seconds += n;
        Survival key_life = {seconds, key};
        for (int i = 0; i < life.size(); i++) {
            if (life[i].key == key) {
                life[i] = key_life;
                std::sort(life.begin(), life.end());
                return KVDB_OK;
            }
        }
        life.push_back(key_life);
        std::sort(life.begin(), life.end());
        return KVDB_OK;
    }

    void show() {

    }

};

int KVDBHandler::Purge() {
    KVDBHandler temp("temp.txt");
    for (auto &[key, offset]: index) {
        string value;
        Get(key, value);
        temp.Set(key, value);
    }
    Clear();
    temp.CloseKVDB();
    rename("temp.txt", database_path.c_str());
    CloseKVDB();
    return KVDB_OK;
}

string &Standardize(string &todo) {
    for (auto &it: todo)
        if (std::isupper(it))
            it = it + 'a' - 'A';
    return todo;
}

int main() {
    string todo, key, value;
    KVDBHandler test("test.txt");
    std::cout << "I am Green.3.0, your personal database. " << std::endl << "What can I do for you?" << std::endl;
    while (std::cin >> todo) {
        todo = Standardize(todo);
        if (todo == "set") {
            std::cin >> key >> value;
            if (test.Set(key, value) == KVDB_OK)
                std::cout << "Key: " << key << std::endl << "value: " << value << std::endl;
        } else if (todo == "get") {
            std::cin >> key;
            if (test.Get(key, value) == KVDB_OK)
                std::cout << "The value of " << key << " is " << value << std::endl;
        } else if (todo == "del") {
            std::cin >> key;
            if (test.Del(key) == KVDB_OK)
                std::cout << key << " is gone successfully." << std::endl;
        } else if (todo == "purge") {
            if (test.Purge() == KVDB_OK)
                std::cout << "Purge is successful." << std::endl;
        } else if (todo == "clear") {
            std::cout << "DO you want to clear your database? yes/NO?" << std::endl;
            std::cin >> todo;
            if (todo == "yes" && test.Clear() == KVDB_OK)
                std::cout << "Clear is successful." << std::endl;
        } else if (todo == "expire") {
            int time;
            std::cin >> key >> time;
            if (test.Expire(key, time) == KVDB_OK)
                std::cout << "The value of " << key << " will die after " << time << " seconds." << std::endl;
        } else if (todo == "show") {
            test.show();
        } else std::cout << "Sorry, please input the right instruction." << std::endl;
    }
    return 0;
}