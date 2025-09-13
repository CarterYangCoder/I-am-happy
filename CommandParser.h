#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <string>
#include <vector>
#include <functional>
#include <map>

/**
 * @struct Command
 * @brief 命令数据：标准命令与参数列表。
 */
struct Command {
    std::string command;
    std::vector<std::string> args;
};

/**
 * @class CommandParser
 * @brief 从标准输入读取与解析命令行。
 */
class CommandParser {
public:
    /**
     * @brief 从输入流读取一行并解析为命令。
     * @return 解析结果（若 EOF 则返回 quit）
     */
    Command getCommand();

private:
    /**
     * @brief 将字符串解析为命令与参数。
     * @param input 一行输入
     * @return Command 解析结果
     */
    Command parse(const std::string& input);
};

#endif // COMMANDPARSER_H