#ifndef PARSER_H
#define PARSER_H

#include <functional>
#include <vector>

#include "Command.h"


class CommandParser
{
public:
    using CommandAction = std::function<Command*(const std::vector<std::string>&)>;

    constexpr static auto ROOT_KEYWORD = "root";
    constexpr static auto FIXED_ARG_KEYWORD = "<arg>";
    constexpr static auto VARIABLE_ARGS_KEYWORD = "...";

    class Node
    {
        std::string lexeme;
        CommandAction action = nullptr;
        std::vector<Node*> children;

    public:

        explicit Node(std::string lexeme) : lexeme(std::move(lexeme)) {}

        Node(const Node &node);
        Node(Node &&node) noexcept;

        Node& operator= (Node node); // copy-and-swap

        [[nodiscard]] const std::string& getLexeme() const { return lexeme; }
        [[nodiscard]] const CommandAction& getAction() const { return action; }
        [[nodiscard]] const std::vector<Node*>& getChildren() const { return children; }

        void setLexeme(const std::string& lexeme) { this->lexeme = lexeme; }
        void setAction(const CommandAction &action) { this->action = action; }

        [[nodiscard]] bool isFixedArg() const { return lexeme == FIXED_ARG_KEYWORD; }
        [[nodiscard]] bool isVariableArg() const { return lexeme == VARIABLE_ARGS_KEYWORD; }

        [[nodiscard]] bool isTerminal() const { return action != nullptr; } // checks if it's a valid ending for a command
        [[nodiscard]] bool isLeaf() const { return children.empty(); }
        [[nodiscard]] size_t childrenCount() const { return children.size(); }

        [[nodiscard]] Node* findChild(const std::string& lexeme) const;

        Node* addChild(const Node& node);
        bool removeChild(const Node& node);

        ~Node();

        friend void swap(Node &node1, Node &node2) noexcept;
    };

    CommandParser() : root(ROOT_KEYWORD) {}

    CommandParser(const CommandParser &) = delete;
    CommandParser& operator= (const CommandParser &) = delete;

    [[nodiscard]] std::vector<std::string> getRegisteredCommands() const;

    void registerCommand(const std::string& text, const CommandAction& action);
    [[nodiscard]] Command* parse(const std::string& text) const;

private:

    void getRegisteredCommandsHelper(const Node* current, std::vector<std::string>& commands, const std::string& currentPath = "") const;

    Node root;
};

#endif //PARSER_H
