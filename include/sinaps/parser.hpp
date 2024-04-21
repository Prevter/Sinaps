#pragma once

#include "platform.hpp"
#include "extras.hpp"
#include "lexer.hpp"

namespace sinaps::parser {

    /// === Pattern AST === ///

    class SINAPS_API PatternASTNode {
    public:
        virtual ~PatternASTNode() = default;

        /// @brief Used for debugging purposes to print the AST
        virtual void print() = 0;

        /// @brief Checks the current node against a given address
        virtual bool check(void *address) = 0;

        /// @brief Get the last index of the pattern
        virtual size_t getLastIndex() { return index; }

        size_t index{};
    };

    /// @brief A leaf node (a node with one child)
    class SINAPS_API PatternASTLeaf : public PatternASTNode {
    public:
        void print() override;

        PatternASTNode *next{};

        size_t getLastIndex() override {
            if (next == nullptr) {
                return index;
            }
            return next->getLastIndex();
        }
    };

    /// @brief A branch node (a node with two children)
    class SINAPS_API PatternASTBranch : public PatternASTNode {
    public:
        void print() override;

        PatternASTNode *left{};
        PatternASTNode *right{};

        size_t getLastIndex() override {
            if (right == nullptr) {
                return left->getLastIndex();
            }
            return right->getLastIndex();
        }
    };

    /// @brief The root node of the AST for patterns
    class SINAPS_API PatternAST {
    public:
        PatternAST() = default;

        ~PatternAST() = default;

        PatternASTNode *root{};

        /// @brief Print the AST
        void print() const;
    };

    /// === Nodes === ///

    class SINAPS_API ByteNode : public PatternASTLeaf {
    public:
        explicit ByteNode(size_t index, uint8_t byte) : byte(byte) {
            this->index = index;
        }

        void print() override;

        bool check(void *address) override;

        static Result<ByteNode*> tryParse(const std::vector<lexer::pattern_token> &tokens, size_t index);

        uint8_t byte;
    };

    class SINAPS_API WildcardNode : public PatternASTLeaf {
    public:
        explicit WildcardNode(size_t index) {
            this->index = index;
        }

        void print() override;

        static Result<WildcardNode*> tryParse(const std::vector<lexer::pattern_token> &tokens, size_t index);

        bool check(void *address) override;
    };

    class SINAPS_API CursorNode : public PatternASTLeaf {
    public:
        explicit CursorNode(size_t index) {
            this->index = index;
        }

        void print() override;

        static Result<CursorNode*> tryParse(const std::vector<lexer::pattern_token> &tokens, size_t index);

        bool check(void *address) override;
    };

    class SINAPS_API GroupNode : public PatternASTLeaf {
    public:
        explicit GroupNode(size_t index) {
            this->index = index;
        }

        void print() override;

        static Result<GroupNode*> tryParse(const std::vector<lexer::pattern_token> &tokens, size_t index);

        bool check(void *address) override;

        size_t getLastIndex() override {
            return child->getLastIndex() + 1;
        }

        PatternASTNode* child{};
    };

    class SINAPS_API NotNode : public PatternASTLeaf {
    public:
        explicit NotNode(size_t index) {
            this->index = index;
        }

        void print() override;

        static Result<NotNode*> tryParse(const std::vector<lexer::pattern_token> &tokens, size_t index);

        bool check(void *address) override;

        size_t getLastIndex() override {
            return child->getLastIndex();
        }

        PatternASTNode* child{};
    };

    /// === Functions === ///

    /// @brief Generate an Abstract Syntax Tree from tokens
    /// @param tokens The tokens to generate the AST from
    /// @return The generated AST
    SINAPS_API Result<PatternAST> generateAst(const std::vector<lexer::pattern_token> &tokens);

    /// @brief Parse a pattern from a string
    /// @param pattern The pattern to parse
    /// @return The parsed pattern
    SINAPS_API Result<PatternAST> parsePattern(const std::string &pattern);

}