#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <variant>

namespace json {

    class Node;
    // —охраните объ€влени€ Dict и Array без изменени€
    using Dict = std::map<std::string, Node>;
    using Array = std::vector<Node>;

    // Ёта ошибка должна выбрасыватьс€ при ошибках парсинга JSON
    class ParsingError : public std::runtime_error {
    public:
        using runtime_error::runtime_error;
    };

    class Node {
    public:
        /* –еализуйте Node, использу€ std::variant */
        using Value = std::variant<std::nullptr_t, int, double, std::string, bool, Array, Dict>;
        Node() = default;
        Node(Array array);
        Node(Dict map);
        Node(int value);
        Node(std::string value);
        Node(bool value);
        Node(std::nullptr_t);
        Node(double value);

        bool IsInt() const;
        bool IsDouble() const; 
        bool IsPureDouble() const; 
        bool IsBool() const;
        bool IsString() const;
        bool IsNull() const;
        bool IsArray() const;
        bool IsDict() const;

        int AsInt() const;
        bool AsBool() const;
        double AsDouble() const;
        const std::string& AsString() const;
        const Array& AsArray() const;
        const Dict& AsMap() const;

       
        bool operator==(const Node& rhs) const;
        bool operator!=(const Node& rhs) const;
        Value GetValue() const {
            return value_;
        }
    private:
        Value value_;
    };

    class Document {
    public:
        explicit Document(Node root);

        const Node& GetRoot() const;

    private:
        Node root_;
    };

    Document Load(std::istream& input);

    struct ValuePrinter {
        std::ostream& out;
        void operator()(Array value);
        void operator()(Dict value);
        void operator()(bool value);
        void operator()(int value);
        void operator()(double value);
        void operator()(std::string value);
        void operator()(std::nullptr_t value);

    };
    void Print(const Document& doc, std::ostream& output);

}  // namespace json