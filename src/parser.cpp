#include <sinaps/parser.hpp>
#include <deque>
#include <sstream>

namespace sinaps::parser {

    SINAPS_API void PatternASTLeaf::print() {
        if (next != nullptr) {
            next->print();
        }
    }

    SINAPS_API void PatternASTBranch::print() {
        if (left != nullptr) {
            left->print();
        }
        printf(", ");
        if (right != nullptr) {
            right->print();
        }
    }

    SINAPS_API void PatternAST::print() const {
        if (root != nullptr) {
            root->print();
        }
    }

    SINAPS_API void ByteNode::print() {
        printf("0x%02X", byte);
        if (next != nullptr) {
            printf(" > ");
            next->print();
        }
    }

    bool eat(const std::vector<lexer::pattern_token> &tokens, size_t &index, lexer::pattern_token_type type) {
        if (index < tokens.size() && tokens[index].type == type) {
            index++;
            return true;
        }
        return false;
    }

#define UNWRAP_RESULT(result) if (result.isErr()) { return nullptr; } return result.val();

    PatternASTNode* decostruct(const std::vector<lexer::pattern_token> &tokens, size_t index) {
        if (index >= tokens.size()) {
            return nullptr;
        }

        switch (tokens[index].type) {
            case lexer::pattern_token_type::BYTE: {
                auto result = ByteNode::tryParse(tokens, index);
                UNWRAP_RESULT(result);
            }
            case lexer::pattern_token_type::QUESTION: {
                auto result = WildcardNode::tryParse(tokens, index);
                UNWRAP_RESULT(result);
            }
            case lexer::pattern_token_type::DOLLAR: {
                auto result = CursorNode::tryParse(tokens, index);
                UNWRAP_RESULT(result);
            }
            case lexer::pattern_token_type::L_PAREN: {
                auto result = GroupNode::tryParse(tokens, index + 1);
                UNWRAP_RESULT(result);
            }
            case lexer::pattern_token_type::NOT: {
                auto result = NotNode::tryParse(tokens, index + 1);
                UNWRAP_RESULT(result);
            }
            default:
                return nullptr;
        }
    }

    SINAPS_API bool ByteNode::check(void *address) {
        return *(uint8_t *) address == byte;
    }

    SINAPS_API Result<ByteNode *> ByteNode::tryParse(const std::vector<lexer::pattern_token> &tokens, size_t index) {
        auto node = new ByteNode(index, tokens[index].value);
        node->next = decostruct(tokens, index + 1);
        return Result<ByteNode *>::ok(node);
    }

    SINAPS_API void WildcardNode::print() {
        printf("Wildcard");
        if (next != nullptr) {
            printf(" > ");
            next->print();
        }
    }

    SINAPS_API bool WildcardNode::check(void *address) {
        return true;
    }

    SINAPS_API Result<WildcardNode *> WildcardNode::tryParse(const std::vector<lexer::pattern_token> &tokens, size_t index) {
        auto node = new WildcardNode(index);
        node->next = decostruct(tokens, index + 1);
        return Result<WildcardNode *>::ok(node);
    }

    SINAPS_API bool CursorNode::check(void *address) {
        return true;
    }

    SINAPS_API void CursorNode::print() {
        printf("Cursor");
        if (next != nullptr) {
            printf(" > ");
            next->print();
        }
    }

    SINAPS_API Result<CursorNode *> CursorNode::tryParse(const std::vector<lexer::pattern_token> &tokens, size_t index) {
        auto node = new CursorNode(index);
        node->next = decostruct(tokens, index + 1);
        return Result<CursorNode *>::ok(node);
    }

    SINAPS_API bool GroupNode::check(void *address) {
        return child->check(address);
    }

    SINAPS_API void GroupNode::print() {
        printf("(");
        child->print();
        printf(")");
        if (next != nullptr) {
            printf(" > ");
            next->print();
        }
    }

    SINAPS_API Result<GroupNode *> GroupNode::tryParse(const std::vector<lexer::pattern_token> &tokens, size_t index) {
        auto node = new GroupNode(index);

        node->child = decostruct(tokens, index);
        if (node->child == nullptr) {
            return Result<GroupNode *>::fail("Expected pattern");
        }

        node->next = decostruct(tokens, node->child->getLastIndex() + 2);
        return Result<GroupNode *>::ok(node);
    }

    SINAPS_API bool NotNode::check(void *address) {
        return !child->check(address);
    }

    SINAPS_API void NotNode::print() {
        printf("Not(");
        child->print();
        printf(")");
        if (next != nullptr) {
            printf(" > ");
            next->print();
        }
    }

    SINAPS_API Result<NotNode *> NotNode::tryParse(const std::vector<lexer::pattern_token> &tokens, size_t index) {
        auto node = new NotNode(index);

        node->child = decostruct(tokens, index);
        if (node->child == nullptr) {
            return Result<NotNode *>::fail("Expected a token after '^'");
        }

        node->next = decostruct(tokens, node->child->getLastIndex() + 1);
        return Result<NotNode *>::ok(node);
    }

    SINAPS_API Result<PatternAST> generateAst(const std::vector<lexer::pattern_token> &tokens) {
        if (tokens.empty()) {
            return Result<PatternAST>::fail("Empty pattern");
        }

        PatternAST ast;
        ast.root = decostruct(tokens, 0);

        return Result<PatternAST>::ok(ast);
    }

    SINAPS_API Result<PatternAST> parsePattern(const std::string &pattern) {
        auto tokens = lexer::lexPattern(pattern);
        if (tokens.isErr()) {
            return Result<PatternAST>::fail(tokens.err());
        }
        return generateAst(tokens.val());
    }
}