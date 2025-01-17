#include <stdexcept>

#include "fcntl.h"
#include "unistd.h"

#include "file_handler.h"

FileHandler::FileHandler() : _fd(-1) {
}

FileHandler::~FileHandler() {
    if (0 <= this->_fd) {
        close(this->_fd);
    }
}

// 打开文件
int FileHandler::open(const std::string& file_path) {
    this->_fd = ::open(file_path.c_str(), O_CREAT | O_RDWR, S_IRWXU); 
    if (0 > this->_fd) {
        printf(
                "Failed to open DB file. [file='%s' res=%d]", 
                file_path.c_str(), 
                this->_fd);
        return Result::IO_ERROR;
    }
    return Result::OK;
}

// 输出文本到文件
int FileHandler::write(const std::string& data) {
    if (0 > this->_fd) {
        printf("Init error.\n");
        return Result::INIT_ERROR;
    }

    if (0 > lseek(this->_fd, 0, SEEK_END)) {
        printf("Failed to seek file. [fd=%d]", this->_fd);
        return Result::IO_ERROR;
    }

    size_t size = data.length();
    size_t ret = ::write(this->_fd, (char*)data.c_str(), size);
    if (size != ret) { 
        printf(
                "Failed to write file. [fd=%d write_size=%ld ret_size=%ld]\n",
                this->_fd,
                size,
                ret);
        return Result::IO_ERROR;
    }

    return Result::OK;
}

// 读取文件内容，并输出到屏幕
int FileHandler::read(std::string* data) {
    if (0 > this->_fd) {
        printf("Init error.\n");
        return Result::INIT_ERROR;
    }
    if (NULL == data) {
        printf("Invalid parameter 'data'.\n");
        return Result::PARAM_ERROR;
    }

    off_t end = lseek(this->_fd, 0, SEEK_END);
    if (0 > end) {
        printf("Failed to seek file. [fd=%d]", this->_fd);
        return Result::IO_ERROR;
    }

    if (0 > lseek(this->_fd, 0, SEEK_SET)) {
        printf("Failed to seek file. [fd=%d]", this->_fd);
        return Result::IO_ERROR;
    }

    try {
        data->resize(end);
    } catch (const std::bad_alloc& e) {
        printf("Failed to malloc. [size=%ld]\n", end);
        return Result::MALLOC_ERROR;
    } catch (const std::length_error& e) {
        printf("Wrong length. [size=%ld]\n", end);
        return Result::MALLOC_ERROR;
    }

    size_t ret = ::read(this->_fd, (char*)data->data(), end);
    if (end != ret) {
        printf(
                "Failed to read file. [fd=%d read_size=%ld ret_size=%ld]\n",
                this->_fd,
                end,
                ret);
        return Result::IO_ERROR;
    }

    return Result::OK;
}

