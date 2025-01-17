#include<bits/stdc++.h>
#include <stdexcept>
#include<sys/stat.h>
#include "fcntl.h"
#include <unistd.h>

using std::string;

const int KVDB_ERROR=-1;
const int KVDB_OK = 0;
const int KVDB_INVALID_AOF_PATH = 1;
const int KVDB_INVALID_KEY = 2;
const int KVDB_INVALID_VALUE = 3;
const int KVDB_NO_SPACE_LEFT_ON_DEVICES = 4;
const int KVDB_NO_FILE = 5;
const int KVDB_FILE_WRITE_ERROR = 6;
const int KVDB_IO_ERROR = 7;

class KVDBHandler {
public:
    int database_address;
    string database_path;
    KVDBHandler(const string &database_path) : database_address(-1), database_path(database_path) {}
    ~KVDBHandler() {
        if (0 <= database_address)
            close(database_address);
    }
    int OpenKVDB() {
        if (0 <= database_address)CloseKVDB();
        this->database_address = open(this->database_path.c_str(), O_CREAT | O_RDWR, S_IRWXU);
        if (0 > this->database_address) {
            printf("Failed to open DB file. [file='%s' res=%d]", this->database_path.c_str(), this->database_address);
        }
        return KVDB_OK;
    }
    int CloseKVDB() {
        if (0 <= database_address)
            close(database_address);
        return KVDB_OK;
    }
    int Set(const string &key, const string &value) {
        OpenKVDB();
        if (0 >= key.length() || key.find(' ') != string::npos ) {
            printf("KVDB_INVALID_KEY\n");
            return KVDB_INVALID_KEY;
        }
        if (0 >= value.length() || value.find(' ') != string::npos) {
            printf("KVDB_INVALID_VALUE\n");
            return KVDB_INVALID_VALUE;
        }
        if (0 > database_address) {
            printf("KVDB_NO_FILE\n");
            return KVDB_NO_FILE;
        }

        if (0 > lseek(database_address, 0, SEEK_END)) {
            printf("Failed to seek file. [fd=%d]", database_address);
            return KVDB_NO_FILE;
        }
        size_t key_length = key.length();
        size_t value_length = value.length();
        size_t ret = write(database_address, &key_length, sizeof(int));
        if (sizeof(int) != ret) {
            printf("Failed to write file. [fd=%d write_size=%u ret_size=%u]\n", database_address, sizeof(int), ret);
            return KVDB_FILE_WRITE_ERROR;
        }
        ret = write(database_address, &value_length, sizeof(int));
        if (sizeof(int) != ret) {
            printf("Failed to write file. [fd=%d write_size=%u ret_size=%u\n", database_address, sizeof(int), ret);
            return KVDB_FILE_WRITE_ERROR;
        }
        ret = write(database_address, (char*)key.c_str(), key.length());
        if (key.length() != ret) {
            printf("Failed to write file. [fd=%d write_size=%u ret_size=%u]\n", database_address, key.length(), ret);
            return KVDB_FILE_WRITE_ERROR;
        }
        ret = write(database_address, (char*)value.c_str(), value.length());
        if (value.length() != ret) {
            printf("Failed to write file. [fd=%d write_size=%u ret_size=%u]\n", database_address, value.length(), ret);
            return KVDB_FILE_WRITE_ERROR;
        }
        CloseKVDB();
        return KVDB_OK;
    }

    int Del(const std::string &key) {
        OpenKVDB();
        if (0 >= key.length() || key.find(' ') != string::npos ) {
            printf("KVDB_INVALID_KEY\n");
            return KVDB_INVALID_KEY;
        }

        off_t end = lseek(database_address, 0, SEEK_END);
        off_t cur = lseek(database_address, 0, SEEK_SET);
        size_t key_length = 0;
        int value_length = 0;
        string key_test;
        bool still_exist = true;
        while (end != cur) {
            read(database_address, &key_length, sizeof(int));
            read(database_address, &value_length, sizeof(int));
            key_test.resize(key_length);
            read(database_address, (char *) key_test.data(), key_length);
            if (key == key_test && -1 == value_length) {
                still_exist = false;
                value_length = 0;
            } else if (key == key_test) {
                still_exist = true;
            } else if (-1 == value_length) {
                value_length = 0;
            }
            cur = lseek(database_address, value_length, SEEK_CUR);
        }
        if (!still_exist) {
            printf("KVDB_IO_ERROR-DOUBLE DELETED!!!\n");
            return KVDB_IO_ERROR;
        }

        key_length = key.length();
        value_length = -1;
        size_t ret = write(database_address, &key_length, sizeof(int));
        if (sizeof(int) != ret) {
            printf("Failed to write file. [fd=%d write_size=%u ret_size=%u]\n", database_address, key_length, ret);
            return KVDB_FILE_WRITE_ERROR;
        }
        ret = write(database_address, &value_length, sizeof(int));
        if (sizeof(int) != ret) {
            printf("Failed to write file. [fd=%d write_size=%u ret_size=%u\n", database_address, value_length, ret);
            return KVDB_FILE_WRITE_ERROR;
        }
        ret = write(database_address, (char*)key.c_str(), key.length());
        if (key.length() != ret) {
            printf("Failed to write file. [fd=%d write_size=%u ret_size=%u]\n", database_address, key.length(), ret);
            return KVDB_FILE_WRITE_ERROR;
        }
        CloseKVDB();
        return KVDB_OK;
    }

    int Get(const string &key, string &value) {
        OpenKVDB();
        if (0 >= key.length() || key.find(' ') != string::npos ) {
            printf("KVDB_INVALID_KEY\n");
            return KVDB_INVALID_KEY;
        }

        const off_t end = lseek(database_address, 0, SEEK_END);
        off_t cur = lseek(database_address, 0, SEEK_SET);
        size_t key_length = 0;
        int value_length = 0;
        string key_test;
        string value_test;
        bool found = false;
        bool still_exixt = true;
        while (end != cur) {
            read(database_address, &key_length, sizeof(int));
            read(database_address, &value_length, sizeof(int));
            key_test.resize(key_length);
            read(database_address, (char *) key_test.data(), key_length);
            if (key == key_test && -1 == value_length) {
                found = false;
                still_exixt = false;
                value_length = 0;
            } else if (key == key_test) {
                found = true;
                still_exixt = true;
                value_test.resize(value_length);
                read(database_address, (char *) value_test.data(), value_length);
                value_length = 0;
            } else if (-1 == value_length) {
                value_length = 0;
            }
            cur = lseek(database_address, value_length, SEEK_CUR);
        }
        if (!still_exixt) {
            printf("KVDB_IO_ERROR-DELETED!!!\n");
            return KVDB_IO_ERROR;
        }
        if (found) {
            value = value_test;
            CloseKVDB();
            return KVDB_OK;
        } else {
            printf("KVDB_IO_ERROR-NO FOUND\n");
            CloseKVDB();
            return KVDB_IO_ERROR;
        }
    }

    int Clear() {
        CloseKVDB();
        if(unlink(database_path.c_str())<0){
            perror(database_path.c_str());
            return KVDB_ERROR;
        }
        return KVDB_OK;
    }

    int Purge();
};

int KVDBHandler::Purge() {
    OpenKVDB();
    KVDBHandler temp("temp.txt");
    const off_t end = lseek(database_address, 0, SEEK_END);
    off_t cur = lseek(database_address, 0, SEEK_SET);
    size_t key_length = 0;
    int value_length = 0;
    string key;
    string value;
    while (end != cur) {
        bool still_exit = true;
        read(database_address, &key_length, sizeof(int));
        read(database_address, &value_length, sizeof(int));
        key.resize(key_length);
        read(database_address, (char *) key.data(), key_length);
        if (-1 != value_length) {
            value.resize(value_length);
            read(database_address, (char *) value.data(), value_length);
        } else continue;
        off_t curcur = lseek(database_address, 0, SEEK_CUR);
        size_t keykey_length = 0;
        int valuevalue_length = 0;
        string keykey;
        string valuevalue;
        while (end != curcur) {
            read(database_address, &keykey_length, sizeof(int));
            read(database_address, &valuevalue_length, sizeof(int));
            keykey.resize(keykey_length);
            read(database_address, (char *) keykey.data(), key_length);
            if (key == keykey) {
                still_exit = false;
                break;
            } else if (-1 == valuevalue_length) {
                valuevalue_length = 0;
            }
            curcur = lseek(database_address, valuevalue_length, SEEK_CUR);
        }
        if (still_exit) {
            temp.Set(key, value);
        }
        cur = lseek(database_address, 0, SEEK_CUR);
    }
    Clear();
    const off_t temp_end= lseek(temp.database_address,0,SEEK_END);
    off_t temp_cur= lseek(temp.database_address,0,SEEK_SET);
    while(temp_end!=temp_cur){
        read(temp.database_address, &key_length, sizeof(int));
        read(temp.database_address, &value_length, sizeof(int));
        key.resize(key_length);
        read(temp.database_address, (char*)key.data(), key_length);
        value.resize(value_length);
        read(temp.database_address,(char*)value.data(),value_length);
        Set(key,value);
        temp_cur= lseek(temp.database_address,0,SEEK_CUR);
    }
    temp.Clear();
    CloseKVDB();
    return KVDB_OK;
}
int main(){
    string TODO,key,value;
    KVDBHandler test("test.txt");
    while(std::cin>>TODO){
        if(TODO=="Set"){
            std::cin>>key>>value;
            if(test.Set(key,value)==KVDB_OK)
                std::cout<<key<<':'<<value<<std::endl;
        }else if(TODO=="Get"){
            std::cin>>key;
            if(test.Get(key,value)==KVDB_OK)
                std::cout<<key<<':'<<value<<std::endl;
        }else if(TODO=="Del"){
            std::cin>>key;
            if(test.Del(key)==KVDB_OK)
                std::cout<<key<<" is gone successfully."<<std::endl;
        }else if(TODO=="Purge"){
            if(test.Purge()==KVDB_OK)
                std::cout<<"Purge is successful."<<std::endl;
        }else if(TODO=="Clear"){
            if(test.Clear()==KVDB_OK)
                std::cout<<"Clear is successful."<<std::endl;
        }else std::cout<<"please input the right instruction"<<std::endl;
    }
    return 0;
}