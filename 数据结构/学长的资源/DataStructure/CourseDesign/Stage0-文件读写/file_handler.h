#pragma once
#include <string>
class FileHandler {
	public:
		explicit FileHandler();
		~FileHandler();
		// 打开文件
		int open(const std::string& file_path);
		// 输出文本到文件
		int write(const std::string& data);
		// 读取文件内容
		int read(std::string* data);
	private:
		int _fd;
};
