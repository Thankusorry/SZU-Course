#include <iostream>
#include <string>
#include "file_handler.h"
int main(int, char**) {
	FileHandler file_handler;
	// 打开文件
	if (0 != file_handler.open("helloworld.txt")) {
		printf("[ERROR] Failed to open file.\n");
	}
	// 向文件写入 "hello, world!"
	if (0 != file_handler.write("hello, world!")) {
		printf("[ERROR] Failed to write.\n");
	}
	// 读取文件内容，并输出到屏幕
	std::string data;
	file_handler.read(&data);
	std::cout << data << std::endl;
	return 0;
}
