#include "deps.hpp"

#include "cmdgen.hpp"
#include "shell.hpp"
#include "shellcmd.hpp"

namespace env {

class CmdHelp : public Command {
public:
    CmdHelp(Shell &shell) : Command("help", "h", "Show all commands"), holder(shell) {}
    int execute() override {
        const std::vector<std::shared_ptr<Command>> &commands = holder.getCommands();
        if(commands.empty()) {
            holder.print("Sorry, but there is no one registered command");
            return -1;
        }
        std::stringstream ss;
        ss << "Commands:" << std::endl;
        for(const std::shared_ptr<Command> &cmd : commands) {
            CmdInfo info = cmd->info();
            std::cout << " ";
            ss.width(5);
            ss.setf(std::ios::left);
            ss << info.shortcut;
            ss.width(10);
            ss << info.name << "  - ";
            ss << info.description << std::endl;
        }
        holder.print(ss.str());
        return 0;
    }
private:
    Shell &holder;
};

class CmdExit : public Command {
public:
    CmdExit() : Command("exit", "e", "Exit shell") {}
    int execute() override {
        exit(0);
        return 0;
    }
};

class CmdHelloWorld : public Command {
public:
    CmdHelloWorld() : Command("hello_world", "hw", "Print string 'Hello world' into console") {}

    int execute() override {
        printf("Hello world!\n");
        return 0;
    }
};

class CmdEcho : public Command {
public:
    CmdEcho(Shell &shell)
        : Command("echo", "", "Just echo. Nothing more, but a lil less"), holder(shell) {}
    int execute() override {
        holder.print(params);
        params.clear();
        return 0;
    }
    bool operator==(const std::string &other) override {
        if(other.empty()) {
            return false;
        }
        size_t space_pos = other.find(' ');
        if(space_pos == std::string::npos) {
            return Command::operator==(other);
        }
        std::string cmdname = other.substr(0, space_pos);
        if(!Command::operator==(cmdname)) {
            return false;
        }
        params = other.substr(space_pos + 1);
        return true;
    }
private:
    Shell &holder;
    std::string params;
};


CommandPtr CommandGenerator::gen(CommandType type) {
    switch(type) {
        case CommandType::Exit: return CommandPtr(new CmdExit);
        case CommandType::HelloWorld: return CommandPtr(new CmdHelloWorld);
    }
    return nullptr;
}

CommandPtr CommandGenerator::gen(CommandType type, Shell &holder) {
    switch(type) {
        case CommandType::Help: return CommandPtr(new CmdHelp(holder));
        case CommandType::Echo: return CommandPtr(new CmdEcho(holder));
    }
    return nullptr;
}

} // namespace env
