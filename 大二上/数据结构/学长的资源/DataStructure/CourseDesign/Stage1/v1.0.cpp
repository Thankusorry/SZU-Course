#include<bits/stdc++.h>
#include <stdexcept>
#include"sys/stat.h"
#include "fcntl.h"
#include "unistd.h"

using std::string;

const int KVDB_OK = 0;
const int KVDB_INVALID_AOF_PATH = 1;
const int KVDB_INVALID_KEY = 2;
const int KVDB_INVALID_VALUE = 3;
const int KVDB_NO_SPACE_LEFT_ON_DEVICES = 4;
const int KVDB_NO_FILE = 5;
const int KVDB_FILE_WRITE_ERROR = 6;
const int KVDB_IO_ERROR = 7;

class KVDBHandler {
private:
    int database_address;
    string database_path;
public:

    KVDBHandler(const string& database_path): database_address(-1), database_path(database_path) {}
    ~KVDBHandler() {
        if (0 <= database_address)
            close(database_address);
    }
    int OpenKVDB() {
        this->database_address = ::open(this->database_path.c_str(), O_CREAT | O_RDWR, S_IRWXU);
        if (0 > this->database_address) {
            printf("Failed to open DB file. [file='%s' res=%d]", this->database_path.c_str(), this->database_address);
        }
        return KVDB_OK;
    }

    int Set(const string& key, const string& value) {
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
        int key_length = key.length();
        int value_length = value.length();
        size_t ret = ::write(database_address, &key_length, sizeof(int));
        if (sizeof(key) != ret) {
            printf("Failed to write file. [fd=%d write_size=%u ret_size=%u]\n", database_address, key_length, ret);
            return KVDB_FILE_WRITE_ERROR;
        }
        ret = ::write(database_address, &value_length, sizeof(int));
        if (sizeof(value) != ret) {
            printf("Failed to write file. [fd=%d write_size=%u ret_size=%u\n", database_address, value_length, ret);
            return KVDB_FILE_WRITE_ERROR;
        }
        ret = ::write(database_address, &key, key.length() );
        if (key.length() != ret) {
            printf("Failed to write file. [fd=%d write_size=%u ret_size=%u]\n", database_address, key.length(), ret);
            return KVDB_FILE_WRITE_ERROR;
        }
        ret = ::write(database_address, &value, value.length());
        if (value.length() != ret) {
            printf("Failed to write file. [fd=%d write_size=%u ret_size=%u]\n", database_address, value.length(), ret);
            return KVDB_FILE_WRITE_ERROR;
        }
        if (0 <= database_address)
            close(database_address);
        return KVDB_OK;
    }

    int Del(const std::string& key) {
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
        while (end != cur) {
            ::read(database_address, &key_length, sizeof(int));
            ::read(database_address, &value_length, sizeof(int));
            ::read(database_address, &key_test, key_length);
            if (key == key_test && -1 == value_length) {
                printf("KVDB_IO_ERROR-DOUBLE DELETED!!!\n");
                return KVDB_IO_ERROR;
            } else {
                if (-1 == value_length)
                    value_length = 0;
                lseek(database_address, value_length, SEEK_CUR);
            }
            cur = lseek(database_address, 0, SEEK_CUR);
        }
        key_length = key.length();
        value_length = -1;
        size_t ret = ::write(database_address, &key_length, sizeof(int));
        if (sizeof(int) != ret) {
            printf("Failed to write file. [fd=%d write_size=%u ret_size=%u]\n", database_address, key_length, ret);
            return KVDB_FILE_WRITE_ERROR;
        }
        ret = ::write(database_address, &value_length, sizeof(int));
        if (sizeof(int) != ret) {
            printf("Failed to write file. [fd=%d write_size=%u ret_size=%u\n", database_address, value_length, ret);
            return KVDB_FILE_WRITE_ERROR;
        }
        ret = ::write(database_address, &key, key.length() );
        if (key.length() != ret) {
            printf("Failed to write file. [fd=%d write_size=%u ret_size=%u]\n", database_address, key.length(), ret);
            return KVDB_FILE_WRITE_ERROR;
        }
        if (0 <= database_address)
            close(database_address);
        return KVDB_OK;
    }

    int Get(const string& key, string& value) {
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
        string value_test;
        bool found = false;
        while (end != cur) {
            ::read(database_address, &key_length, sizeof(int));
            ::read(database_address, &value_length, sizeof(int));
            ::read(database_address, &key_test, key_length);
            if (key == key_test) {
                if (-1 != value_length) {
                    ::read(database_address, &value_test, value_length);
                    found = true;
                }

                else {
                    printf("KVDB_IO_ERROR-DELETED!!!\n");
                    return KVDB_IO_ERROR;
                }
            } else {
                if (-1 == value_length)
                    value_length = 0;
                lseek(database_address, value_length, SEEK_CUR);
            }
            cur = lseek(database_address, 0, SEEK_CUR);
        }
        if (found) {
            value = value_test;
            return KVDB_OK;
        }
        printf("KVDB_IO_ERROR-NO FOUND\n");
        if (0 <= database_address)
            close(database_address);
        return KVDB_IO_ERROR;
    }

    int Purge() {
        OpenKVDB();
        int database_address_temp =::open("temp.txt", O_CREAT | O_RDWR, S_IRWXU);
        if (0 > database_address_temp) {
            printf("Failed to open DB file. [file=temp.txt res=%d]", database_address_temp);
        }

        const off_t end = lseek(database_address, 0, SEEK_END);
        off_t cur = lseek(database_address, 0, SEEK_SET);
        size_t key_length = 0;
        int value_length = 0;
        string key;
        string value;
        while (end != cur) {
            bool still_exit = true;
            ::read(database_address, &key_length, sizeof(int));
            ::read(database_address, &value_length, sizeof(int));
            ::read(database_address, &key, key_length);
            if (-1 != value_length) {
                ::read(database_address, &value, value_length);
            }
            off_t curcur = lseek(database_address, 0, SEEK_CUR);
            size_t keykey_length = 0;
            int valuevalue_length = 0;
            string keykey;
            string valuevalue;
            while (end != curcur) {
                ::read(database_address, &keykey_length, sizeof(int));
                ::read(database_address, &valuevalue_length, sizeof(int));
                ::read(database_address, &keykey, key_length);
                if (key == keykey && -1 == valuevalue_length) {
                    still_exit = false;
                    break;
                } else {
                    if (-1 == valuevalue_length)
                        valuevalue_length = 0;
                    ::read(database_address, &valuevalue, valuevalue_length);
                }
                curcur = lseek(database_address, 0, SEEK_CUR);
            }
            if (still_exit) {
                size_t ret = ::write(database_address_temp, &key_length, sizeof(int));
                if (sizeof(key) != ret) {
                    printf("Failed to write file. [fd=%d write_size=%u ret_size=%u]\n", database_address_temp, key_length, ret);
                    return KVDB_FILE_WRITE_ERROR;
                }
                ret = ::write(database_address_temp, &value_length, sizeof(int));
                if (sizeof(value) != ret) {
                    printf("Failed to write file. [fd=%d write_size=%u ret_size=%u\n", database_address_temp, value_length, ret);
                    return KVDB_FILE_WRITE_ERROR;
                }
                ret = ::write(database_address_temp, &key, key.length() );
                if (key.length() != ret) {
                    printf("Failed to write file. [fd=%d write_size=%u ret_size=%u]\n", database_address_temp, key.length(), ret);
                    return KVDB_FILE_WRITE_ERROR;
                }
                ret = ::write(database_address_temp, &value, value.length());
                if (value.length() != ret) {
                    printf("Failed to write file. [fd=%d write_size=%u ret_size=%u]\n", database_address_temp, value.length(), ret);
                    return KVDB_FILE_WRITE_ERROR;
                }
            }
            cur = lseek(database_address, 0, SEEK_CUR);
        }
        database_address =::open(this->database_path.c_str(), O_TRUNC, S_IRWXU);

        const off_t endend = lseek(database_address_temp, 0, SEEK_END);
        off_t curcur = lseek(database_address_temp, 0, SEEK_SET);
        while (endend != curcur) {
            ::read(database_address_temp, &key_length, sizeof(int));
            ::read(database_address_temp, &value_length, sizeof(int));
            ::read(database_address_temp, &key, key_length);
            ::read(database_address_temp, &value, value_length);
            size_t ret = ::write(database_address, &key_length, sizeof(int));
            if (sizeof(key) != ret) {
                printf("Failed to write file. [fd=%d write_size=%u ret_size=%u]\n", database_address, key_length, ret);
                return KVDB_FILE_WRITE_ERROR;
            }
            ret = ::write(database_address, &value_length, sizeof(int));
            if (sizeof(value) != ret) {
                printf("Failed to write file. [fd=%d write_size=%u ret_size=%u\n", database_address, value_length, ret);
                return KVDB_FILE_WRITE_ERROR;
            }
            ret = ::write(database_address, &key, key.length() );
            if (key.length() != ret) {
                printf("Failed to write file. [fd=%d write_size=%u ret_size=%u]\n", database_address, key.length(), ret);
                return KVDB_FILE_WRITE_ERROR;
            }
            ret = ::write(database_address, &value, value.length());
            if (value.length() != ret) {
                printf("Failed to write file. [fd=%d write_size=%u ret_size=%u]\n", database_address, value.length(), ret);
                return KVDB_FILE_WRITE_ERROR;
            }
            curcur = lseek(database_address_temp, 0, SEEK_CUR);
        }

        database_address_temp =::open("temp.txt", O_TRUNC);
        if (0 <= database_address_temp)
            close(database_address_temp);

        if (0 <= database_address)
            close(database_address);
        return KVDB_OK;
    }
};
