// Copyright (c) ET,INC

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

void interpret(std::vector<std::string> &tokens) {
    std::map<std::string, std::string> variables;
    
    for (const auto &token : tokens) {
        
        
        // 处理变量声明
        if (token.find("变量 ") == 0) {
            size_t space_pos = token.find(' ', 3);
            if (space_pos != std::string::npos) {
                std::string var_name = token.substr(3, space_pos - 3);
                std::string var_value = token.substr(space_pos + 1);
                variables[var_name] = var_value;
                continue;
            }
        }
        
        // 处理条件判断
        if (token.find("如果 ") == 0) {
            size_t then_pos = token.find(" 那么 ");
            if (then_pos != std::string::npos) {
                std::string condition = token.substr(3, then_pos - 3);
                std::string action = token.substr(then_pos + 4);
                
                // 简单条件判断实现
                if (condition == "真" || variables.find(condition) != variables.end()) {
                    tokens.push_back(action);
                } else if (token.find("否则") != std::string::npos) {
                    size_t else_pos = token.find("否则 ");
                    if (else_pos != std::string::npos) {
                        std::string else_action = token.substr(else_pos + 3);
                        tokens.push_back(else_action);
                    }
                }
                continue;
            }
        }
        
        // 处理打印语句
        if (token.find("打印(") != std::string::npos) {
            size_t start = token.find('"') + 1;
            size_t end = token.rfind('"');
            if (start != std::string::npos && end != std::string::npos && start < end) {
                std::string content = token.substr(start, end - start);
                std::cout << "Output: " << content << std::endl;
            } else {
                std::cerr << "错误: 令牌中的字符串格式无效: " << token << std::endl;
            }
        }
    }
}

#include <windows.h>

int main(int argc, char *argv[]) {
    SetConsoleOutputCP(CP_UTF8);
    if (argc < 2 || (argc == 2 && (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help"))) {
        std::cerr << "用法: z++ <file.zpp>" << std::endl;
        std::cerr << "选项:" << std::endl;
        std::cerr << "  -h, --help    显示帮助信息" << std::endl;
        std::cerr << "  -v, --version 显示版本信息" << std::endl;
        std::cerr << "  -l, --license 显示许可证信息" << std::endl;
        std::cerr << "  -a, --author  显示作者信息" << std::endl;
        return 1;
    }
    if (argc == 2 && (std::string(argv[1]) == "-l" || std::string(argv[1]) == "--license")) {
        std::cerr << "MIT License" << std::endl;
        std::cerr << "Copyright (c) Eternity-Sky" << std::endl;
        return 0;
    }
    if (argc == 2 && (std::string(argv[1]) == "-a" || std::string(argv[1]) == "--author")) {
        std::cerr << "作者: Eternity-Sky" << std::endl;
        return 0;
    }
    
    if (argc == 2 && (std::string(argv[1]) == "-v" || std::string(argv[1]) == "--version")) {
        std::cerr << "Z++ 编译器 版本 0.1.0" << std::endl;
    std::cerr << "Copyright © Eternity-Sky" << std::endl;
    std::cerr << "EEEEE TTTTT" << std::endl;
    std::cerr << "E       T" << std::endl;
    std::cerr << "EEE     T" << std::endl;
    std::cerr << "E       T" << std::endl;
    std::cerr << "EEEEE   T" << std::endl;
    std::cerr << "欢迎访问ETOJ.XYZ-OJ系统" << std::endl;
        return 0;
    }

    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "无法打开文件: " << argv[1] << std::endl;
        return 1;
    }

    std::string line;
    std::vector<std::string> tokens;
    while (std::getline(file, line)) {
        tokens.push_back(line);
    }

    interpret(tokens);
    return 0;
}