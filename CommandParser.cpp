/**
 * @file CommandParser.cpp
 * @brief 简易命令解析器实现：读取与分词。
 */
#include "CommandParser.h"
#include <iostream>
#include <sstream>
#include <algorithm>

/** @brief 将一行输入解析为命令与参数。 */
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

/** @brief 读取用户输入并解析；若输入结束返回 quit。 */
Command CommandParser::getCommand() {
    std::cout << "> ";
    std::string inputLine;
    if (!std::getline(std::cin, inputLine)) {
        // 处理输入结束 (例如 Ctrl+D)
        return {"quit", {}};
    }
    return parse(inputLine);
}