#include "CommandParser.h"
#include "Utility.h"


CommandParser::Node::Node(const Node &node) : lexeme(node.lexeme)
{
    this->children.reserve(node.children.size());

    for (const auto* child : node.children) {
        if (child != nullptr)
            this->children.push_back(new Node(*child));
        else
            this->children.push_back(nullptr);
    }
}

CommandParser::Node::Node(Node &&node) noexcept
{
    this->lexeme = std::move(node.lexeme);
    this->children = std::move(node.children);
}

CommandParser::Node& CommandParser::Node::operator=(Node node)
{
    swap(*this, node);
    return *this;
}

CommandParser::Node* CommandParser::Node::findChild(const std::string &lexeme) const
{
    // check directly
    for (auto* child : children)
        if (child && (child->lexeme == lexeme))
            return child;

    // check for args
    for (auto* child : children)
        if (child && (child->isFixedArg() || child->isVariableArg()))
            return child;

    return nullptr;
}

CommandParser::Node* CommandParser::Node::addChild(const Node &node)
{
    auto* child = new Node(node);
    children.push_back(child);
    return child;
}

bool CommandParser::Node::removeChild(const Node &node)
{
    for (auto iter = children.begin(); iter != children.end(); ++iter)
    {
        if (*iter && (*iter)->lexeme == node.lexeme)
        {
            delete *iter;
            children.erase(iter);
            return true;
        }
    }

    return false;
}

CommandParser::Node::~Node()
{
    for (const auto child : children)
        delete child;
}

std::vector<std::string> CommandParser::getRegisteredCommands() const
{
    std::vector<std::string> commands;
    getRegisteredCommandsHelper(&root, commands);
    return commands;
}

void CommandParser::getRegisteredCommandsHelper(const Node *current, std::vector<std::string> &commands, const std::string& currentPath) const
{
    if (current == nullptr)
        return;

    if (current->isTerminal())
        commands.push_back(currentPath);

    for (const auto child: current->getChildren())
    {
        std::string branch;

        if (!currentPath.empty()) branch += " ";
        branch += child->getLexeme();

        getRegisteredCommandsHelper(child, commands, currentPath + branch);
    }
}

void CommandParser::registerCommand(const std::string &text, const CommandAction& action)
{
    const std::vector<std::string> tokens = split(text, ' ');
    Node* current = &root;

    for (const auto& token : tokens)
    {
        auto child = current->findChild(token);

        if (child == nullptr)
            child = current->addChild(Node(token));

        current = child;
    }

    current->setAction(action);
}

Command* CommandParser::parse(const std::string &text) const
{
    const std::vector<std::string> tokens = split(text, ' ');
    std::vector<std::string> args;

    const Node* current = &root;
    size_t pos = 0;
    for (const auto& token : tokens)
    {
        pos = text.find(token, pos);

        current = current->findChild(token);
        if (current == nullptr) return nullptr;

        if (current->isFixedArg())
        {
            args.push_back(token);
            pos += token.length();
        }
        else if (current->isVariableArg())
        {
            args.push_back(text.substr(pos));
            break;
        }
    }

    const CommandAction action = current->getAction();
    return action ? action(args) : nullptr;
}

void swap(CommandParser::Node &node1, CommandParser::Node &node2) noexcept
{
    using std::swap;

    swap(node1.lexeme, node2.lexeme);
    swap(node1.children, node2.children);
}
