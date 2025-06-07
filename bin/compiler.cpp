// Copyright (c) ET,INC

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

std::map<std::string, std::vector<std::string>> arrays;

void interpret(std::vector<std::string> &tokens) {
    std::map<std::string, std::string> variables;
    
    // 预处理: 去除注释
    // for (auto &token : tokens) {
    //     size_t comment_pos = token.find("//");
    //     if (comment_pos != std::string::npos) {
    //         token = token.substr(0, comment_pos);
    //     }
    // }
    
    for (const auto &token : tokens) {
        // 处理函数定义
        if (token.find("函数 ") == 0) {
            size_t open_paren_pos = token.find('(');
            size_t close_paren_pos = token.find(')');
            size_t do_pos = token.find(" 做");
            if (open_paren_pos != std::string::npos && close_paren_pos != std::string::npos && do_pos != std::string::npos) {
                std::string func_name = token.substr(3, open_paren_pos - 3);
                std::string params_str = token.substr(open_paren_pos + 1, close_paren_pos - open_paren_pos - 1);
                std::string body_str = token.substr(do_pos + 3);
                variables[func_name + "_params"] = params_str;
                variables[func_name + "_body"] = body_str;
                continue;
            }
        }
        
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
                std::string condition_str = token.substr(3, then_pos - 3);
                std::string action = token.substr(then_pos + 4);
                
                bool condition_result = false;
                // 简单条件判断实现
                if (condition_str == "真") {
                    condition_result = true;
                } else if (variables.count(condition_str)) {
                    condition_result = (variables[condition_str] == "真");
                } else {
                    // 尝试解析比较表达式
                    size_t op_pos;
                    std::string op;
                    if ((op_pos = condition_str.find(" > ")) != std::string::npos) {
                        op = ">";
                    } else if ((op_pos = condition_str.find(" < ")) != std::string::npos) {
                        op = "<";
                    } else if ((op_pos = condition_str.find(" == ")) != std::string::npos) {
                        op = "==";
                    } else if ((op_pos = condition_str.find(" != ")) != std::string::npos) {
                        op = "!=";
                    } else if ((op_pos = condition_str.find(" >= ")) != std::string::npos) {
                        op = ">=";
                    } else if ((op_pos = condition_str.find(" <= ")) != std::string::npos) {
                        op = "<=";
                    }

                    if (!op.empty()) {
                        std::string left_str = condition_str.substr(0, op_pos);
                        std::string right_str = condition_str.substr(op_pos + op.length());

                        // 尝试将左右操作数转换为数字
                        try {
                            double left_val = std::stod(left_str);
                            double right_val = std::stod(right_str);

                            if (op == ">") condition_result = (left_val > right_val);
                            else if (op == "<") condition_result = (left_val < right_val);
                            else if (op == "==") condition_result = (left_val == right_val);
                            else if (op == "!=") condition_result = (left_val != right_val);
                            else if (op == ">=") condition_result = (left_val >= right_val);
                            else if (op == "<=") condition_result = (left_val <= right_val);
                        } catch (const std::invalid_argument& e) {
                            // 如果不是数字，则按字符串比较
                            if (op == "==") condition_result = (left_str == right_str);
                            else if (op == "!=") condition_result = (left_str != right_str);
                            else {
                                std::cerr << "错误: 不支持的字符串比较操作符: " << op << std::endl;
                            }
                        }
                    }
                }

                if (condition_result) {
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
        
        // 处理循环结构
        if (token.find("当 ") == 0 && token.find(" 做") != std::string::npos) {
            size_t do_pos = token.find(" 做");
            std::string loop_condition_str = token.substr(3, do_pos - 3);
            std::string loop_body_str = "";
            size_t end_pos = token.find("结束");
            if (end_pos != std::string::npos) {
                loop_body_str = token.substr(do_pos + 3, end_pos - (do_pos + 3));
            }

            // 简单循环实现
            while (true) {
                bool condition_result = false;
                if (loop_condition_str == "真") {
                    condition_result = true;
                } else if (variables.count(loop_condition_str)) {
                    condition_result = (variables[loop_condition_str] == "真");
                } else {
                    // 尝试解析比较表达式
                    size_t op_pos;
                    std::string op;
                    if ((op_pos = loop_condition_str.find(" > ")) != std::string::npos) {
                        op = ">";
                    } else if ((op_pos = loop_condition_str.find(" < ")) != std::string::npos) {
                        op = "<";
                    } else if ((op_pos = loop_condition_str.find(" == ")) != std::string::npos) {
                        op = "==";
                    } else if ((op_pos = loop_condition_str.find(" != ")) != std::string::npos) {
                        op = "!=";
                    } else if ((op_pos = loop_condition_str.find(" >= ")) != std::string::npos) {
                        op = ">=";
                    } else if ((op_pos = loop_condition_str.find(" <= ")) != std::string::npos) {
                        op = "<=";
                    }

                    if (!op.empty()) {
                        std::string left_str = loop_condition_str.substr(0, op_pos);
                        std::string right_str = loop_condition_str.substr(op_pos + op.length());

                        try {
                            double left_val = std::stod(left_str);
                            double right_val = std::stod(right_str);

                            if (op == ">") condition_result = (left_val > right_val);
                            else if (op == "<") condition_result = (left_val < right_val);
                            else if (op == "==") condition_result = (left_val == right_val);
                            else if (op == "!=") condition_result = (left_val != right_val);
                            else if (op == ">=") condition_result = (left_val >= right_val);
                            else if (op == "<=") condition_result = (left_val <= right_val);
                        } catch (const std::invalid_argument& e) {
                            if (op == "==") condition_result = (left_str == right_str);
                            else if (op == "!=") condition_result = (left_str != right_str);
                            else {
                                std::cerr << "错误: 不支持的字符串比较操作符: " << op << std::endl;
                            }
                        }
                    }
                }

                if (condition_result) {
                    // 将循环体添加到tokens中进行解释
                    std::vector<std::string> loop_body_tokens;
                    // 简单的按行分割
                    size_t prev_pos = 0;
                    size_t newline_pos;
                    while ((newline_pos = loop_body_str.find('\n', prev_pos)) != std::string::npos) {
                        loop_body_tokens.push_back(loop_body_str.substr(prev_pos, newline_pos - prev_pos));
                        prev_pos = newline_pos + 1;
                    }
                    loop_body_tokens.push_back(loop_body_str.substr(prev_pos));

                    interpret(loop_body_tokens);
                } else {
                    break;
                }
            }
            continue;
        }
        
        // 处理打印语句
        if (token.find("打印(") != std::string::npos) {
            size_t start = token.find('(') + 1;
            size_t end = token.rfind(')');
            if (start != std::string::npos && end != std::string::npos && start < end) {
                std::string content = token.substr(start, end - start);
                if (content.length() >= 2 && content.front() == '"' && content.back() == '"') {
                    content = content.substr(1, content.length() - 2);
                }
                std::cout << "Output: " << content << std::endl;
            } else {
                std::cerr << "错误: 令牌中的字符串格式无效: " << token << std::endl;
            }
        }
        
        // 处理数组声明
        if (token.find("变量 ") == 0 && token.find("[") != std::string::npos) {
            size_t eq_pos = token.find("=");
            if (eq_pos != std::string::npos) {
                std::string var_name = token.substr(3, eq_pos - 3);
                std::string array_content = token.substr(eq_pos + 1);
                
                // 简单数组解析
                std::vector<std::string> elements;
                size_t current_pos = 0;
                while (current_pos < array_content.length()) {
                    size_t comma_pos = array_content.find(',', current_pos);
                    std::string element_str;
                    if (comma_pos == std::string::npos) {
                        element_str = array_content.substr(current_pos);
                    } else {
                        element_str = array_content.substr(current_pos, comma_pos - current_pos);
                    }
                    
                    // 去除首尾空格
                    size_t first = element_str.find_first_not_of(' ');
                    size_t last = element_str.find_last_not_of(' ');
                    if (std::string::npos == first) {
                        elements.push_back("");
                    } else {
                        elements.push_back(element_str.substr(first, (last - first + 1)));
                    }
                    
                    if (comma_pos == std::string::npos) {
                        break;
                    }
                    current_pos = comma_pos + 1;
                }
                arrays[var_name] = elements;
                continue;
            }
        }
        
        // 处理函数调用
        if (token.find("(") != std::string::npos && token.find(")") != std::string::npos) {
            size_t open_paren_pos = token.find('(');
            size_t close_paren_pos = token.find(')');
            if (open_paren_pos != std::string::npos && close_paren_pos != std::string::npos && open_paren_pos < close_paren_pos) {
                std::string func_name = token.substr(0, open_paren_pos);
                std::string args_str = token.substr(open_paren_pos + 1, close_paren_pos - open_paren_pos - 1);

                if (variables.count(func_name + "_body")) {
                    std::string func_body = variables[func_name + "_body"];
                    std::string func_params = variables[func_name + "_params"];

                    // 简单的参数替换
                    std::map<std::string, std::string> call_variables = variables;
                    std::vector<std::string> param_names;
                    size_t prev_pos = 0;
                    size_t comma_pos;
                    while ((comma_pos = func_params.find(',', prev_pos)) != std::string::npos) {
                        param_names.push_back(func_params.substr(prev_pos, comma_pos - prev_pos));
                        prev_pos = comma_pos + 1;
                    }
                    param_names.push_back(func_params.substr(prev_pos));

                    std::vector<std::string> arg_values;
                    prev_pos = 0;
                    while ((comma_pos = args_str.find(',', prev_pos)) != std::string::npos) {
                        arg_values.push_back(args_str.substr(prev_pos, comma_pos - prev_pos));
                        prev_pos = comma_pos + 1;
                    }
                    arg_values.push_back(args_str.substr(prev_pos));

                    for (size_t i = 0; i < param_names.size() && i < arg_values.size(); ++i) {
                        call_variables[param_names[i]] = arg_values[i];
                    }

                    // 执行函数体
                    std::vector<std::string> func_body_tokens;
                    prev_pos = 0;
                    size_t newline_pos;
                    while ((newline_pos = func_body.find('\n', prev_pos)) != std::string::npos) {
                        func_body_tokens.push_back(func_body.substr(prev_pos, newline_pos - prev_pos));
                        prev_pos = newline_pos + 1;
                    }
                    func_body_tokens.push_back(func_body.substr(prev_pos));

                    interpret(func_body_tokens);
                } else {
                    std::cerr << "错误: 未知函数: " << func_name << std::endl;
                }
                continue;
            }
        }
        
        // 处理未识别的令牌
        // if (variables.find(token) != variables.end()) {
        //     tokens.push_back(variables[token]);
        //     continue;
        // }
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
        size_t comment_pos = line.find("//");
        if (comment_pos != std::string::npos) {
            line = line.substr(0, comment_pos);
        }
        tokens.push_back(line);
    }

    interpret(tokens);
    return 0;
}