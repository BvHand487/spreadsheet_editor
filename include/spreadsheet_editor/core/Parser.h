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
    constexpr static auto VARIABLE_KEYWORD = "<var>";

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

        [[nodiscard]] const std::string& getLexeme() const { return lexeme; };
        [[nodiscard]] const CommandAction& getAction() const { return action; };
        [[nodiscard]] const std::vector<Node*>& getChildren() const { return children; };

        void setLexeme(const std::string& lexeme) { this->lexeme = lexeme; };
        void setAction(const CommandAction &action) { this->action = action; };

        [[nodiscard]] bool isLeaf() const { return children.empty(); };
        [[nodiscard]] size_t childrenCount() const { return children.size(); };

        [[nodiscard]] Node* findChild(const std::string& lexeme) const;

        Node* addChild(const Node& node); // add child
        bool removeChild(const Node& node); // remove child

        ~Node();

        friend void swap(Node &node1, Node &node2) noexcept;
    };

    CommandParser() : root(ROOT_KEYWORD) {}

    CommandParser(const CommandParser &) = delete;
    CommandParser& operator= (const CommandParser &) = delete;

    void registerCommand(const std::string& text, const CommandAction& action);
    [[nodiscard]] Command* parse(const std::string& text) const;

private:
    Node root;
};

#endif //PARSER_H
