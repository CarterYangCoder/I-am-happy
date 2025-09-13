#include "CommandParser.h"
#include <iostream>
#include <sstream>
#include <algorithm>

Command CommandParser::parse(const std::string& input) {
    Command cmd;
    std::stringstream ss(input);
    std::string word;

    if (ss >> word) {
        // 将命令转为小写，方便匹配
        std::transform(word.begin(), word.end(), word.begin(), 
            [](unsigned char c){ return std::tolower(c); });
        cmd.command = word;
    }

    while (ss >> word) {
        cmd.args.push_back(word);
    }

    return cmd;
}

Command CommandParser::getCommand() {
    std::cout << "> ";
    std::string inputLine;
    if (!std::getline(std::cin, inputLine)) {
        // 处理输入结束 (例如 Ctrl+D)
        return {"quit", {}};
    }
    return parse(inputLine);
}