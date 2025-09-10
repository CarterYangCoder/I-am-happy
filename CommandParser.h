#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <string>
#include <vector>
#include <functional>
#include <map>

// 命令结构体，用于存储解析后的命令和参数
struct Command {
    std::string command;
    std::vector<std::string> args;
};

class CommandParser {
public:
    // 从输入流读取并解析命令 
    Command getCommand();

private:
    // 解析字符串输入
    Command parse(const std::string& input);
};

#endif // COMMANDPARSER_H