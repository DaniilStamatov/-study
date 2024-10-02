#include "json.h"

using namespace std;

namespace json {

    namespace {

        using Number = std::variant<int, double>;



        Node LoadNode(istream& input);

        Node LoadNull(istream& input) {
            const string nameNull = "null";
            for (size_t i = 0; i < nameNull.size(); i++) {
                if (nameNull.at(i) == input.get()) continue;
                else throw ParsingError("Null parsing error");
            }
            return {};
        }


        Node LoadNumber(std::istream& input) {
            using namespace std::literals;

            std::string parsed_num;

            // Считывает в parsed_num очередной символ из input
            auto read_char = [&parsed_num, &input] {
                parsed_num += static_cast<char>(input.get());
                if (!input) {
                    throw ParsingError("Failed to read number from stream"s);
                }
                };

            // Считывает одну или более цифр в parsed_num из input
            auto read_digits = [&input, read_char] {
                if (!std::isdigit(input.peek())) {
                    throw ParsingError("A digit is expected"s);
                }
                while (std::isdigit(input.peek())) {
                    read_char();
                }
                };

            if (input.peek() == '-') {
                read_char();
            }
            // Парсим целую часть числа
            if (input.peek() == '0') {
                read_char();
                // После 0 в JSON не могут идти другие цифры
            }
            else {
                read_digits();
            }

            bool is_int = true;
            // Парсим дробную часть числа
            if (input.peek() == '.') {
                read_char();
                read_digits();
                is_int = false;
            }

            // Парсим экспоненциальную часть числа
            if (int ch = input.peek(); ch == 'e' || ch == 'E') {
                read_char();
                if (ch = input.peek(); ch == '+' || ch == '-') {
                    read_char();
                }
                read_digits();
                is_int = false;
            }

            try {
                if (is_int) {
                    // Сначала пробуем преобразовать строку в int
                    try {
                        return std::stoi(parsed_num);
                    }
                    catch (...) {
                        // В случае неудачи, например, при переполнении,
                        // код ниже попробует преобразовать строку в double
                    }
                }
                return std::stod(parsed_num);
            }
            catch (...) {
                throw ParsingError("Failed to convert "s + parsed_num + " to number"s);
            }
        }

        Node LoadArray(istream& input) {
            Array result;

            for (char c; input >> c && c != ']';) {
                if (c != ',') {
                    input.putback(c);
                }
                result.push_back(LoadNode(input));
            }

            return Node(move(result));
        }

        std::string LoadString(istream& input) {
            auto it = std::istreambuf_iterator<char>(input);
            auto end = std::istreambuf_iterator<char>();
            std::string s;
            while (true) {
                if (it == end) {
                    throw ParsingError("String parsing error");
                }

                const char ch = *it;

                if (ch == '"') {
                    ++it;
                    break;
                }
                else if (ch == '\\') {
                    // Встретили начало escape-последовательности
                    ++it;
                    if (it == end) {
                        throw ParsingError("String parsing error");
                    }

                    const char escaped_char = *it;
                    // Обрабатываем одну из последовательностей: \\, \n, \t, \r, \"

                    switch (escaped_char) {
                    case 'n':
                        s.push_back('\n');
                        break;
                    case 't':
                        s.push_back('\t');
                        break;
                    case 'r':
                        s.push_back('\r');
                        break;
                    case '"':
                        s.push_back('"');
                        break;
                    case '\\':
                        s.push_back('\\');
                        break;
                    default:
                        // Встретили неизвестную escape-последовательность
                        throw ParsingError("Unrecognized escape sequence \\"s + escaped_char);
                    }
                }
                else if (ch == '\n' || ch == '\r') {
                    // Строковый литерал внутри- JSON не может прерываться символами \r или \n
                    throw ParsingError("Unexpected end of line"s);
                }
                else {
                    // Просто считываем очередной символ и помещаем его в результирующую строку
                    s.push_back(ch);
                }
                ++it;
            }

            return s;
        }

        Node LoadBool(istream& input) {
            char c = input.get();
            bool value = (c == 't');
            std::string name = value ? "true" : "false";
            for (size_t i = 1; i < name.size(); i++) {
                if (name[i] == input.get()) continue;
                else throw ParsingError("bool expected");
            }

            return Node(value);
        }

        Node LoadDict(istream& input) {
            Dict result;

            for (char c; input >> c && c != '}';) {
                if (c == ',') {
                    input >> c;
                }

                string key = LoadString(input);
                input >> c;
                result.insert({ move(key), LoadNode(input) });
            }

            return Node(move(result));
        }

        Node LoadNode(istream& input) {
            char c;
            input >> c;
            if (c == 'n') {
                input.putback(c);
                return LoadNull(input);
            }
            if (c == '[') {
                return LoadArray(input);
            }
            else if (c == '{') {
                return LoadDict(input);
            }
            else if (c == '"') {
                return LoadString(input);
            }
            else if (c == 't' || c == 'f')
            {
                input.putback(c);
                return LoadBool(input);
            }
            else {
                input.putback(c);
                return LoadNumber(input);
            }
        }
    }

    Node::Node(Array array)
        : value_(move(array)) {
    }

    Node::Node(bool value)
        : value_(value) {
    }

    Node::Node(std::nullptr_t)
        : value_(nullptr) {
    }

    Node::Node(double value) : value_(value)
    {
    }

    Node::Node(Dict map)
        : value_(move(map)) {
    }

    Node::Node(int value)
        : value_(value) {
    }

    Node::Node(string value)
        : value_(move(value)) {
    }

    bool Node::IsInt() const
    {
        return std::holds_alternative<int>(value_);
    }

    bool Node::IsDouble() const
    {
        return  std::holds_alternative<double>(value_) || std::holds_alternative<int>(value_);
    }

    bool Node::IsPureDouble() const
    {
        return std::holds_alternative<double>(value_);
    }

    bool Node::IsBool() const
    {
        return std::holds_alternative<bool>(value_);
    }

    bool Node::IsString() const
    {
        return std::holds_alternative<std::string>(value_);
    }

    const Array& Node::AsArray() const {
        if (!IsArray()) throw std::logic_error("Не массив");
        return std::get<Array>(value_);
    }

    const Dict& Node::AsMap() const {
        if (!IsDict()) throw std::logic_error("wrong type");

        return std::get<Dict>(value_);
    }

    int Node::AsInt() const {
        if (!IsInt()) throw std::logic_error("wrong type");

        return std::get<int>(value_);
    }

    bool Node::AsBool() const
    {
        if (!IsBool()) throw std::logic_error("wrong type");

        return std::get<bool>(value_);
    }

    double Node::AsDouble() const
    {
        if (!IsDouble()) throw std::logic_error("wrong type");

        if (IsInt()) return static_cast<double>(std::get<int>(value_));
        return std::get<double>(value_);
    }

    const string& Node::AsString() const {
        if (!IsString()) throw std::logic_error("wrong type");

        return std::get<std::string>(value_);
    }

    bool Node::operator==(const Node& rhs) const
    {
        return value_ == rhs.value_;

    }

    bool Node::operator!=(const Node& rhs) const
    {
        return !(value_ == rhs.value_);
    }

    Document::Document(Node root)
        : root_(move(root)) {
    }

    const Node& Document::GetRoot() const {
        return root_;
    }

    Document Load(istream& input) {
        return Document{ LoadNode(input) };
    }

 

    bool Node::IsNull() const
    {
        return std::holds_alternative<std::nullptr_t>(value_);
    }

    bool Node::IsArray() const
    {
        return std::holds_alternative<Array>(value_);
    }

    bool Node::IsDict() const
    {
        return std::holds_alternative<Dict>(value_);
    }

    void ValuePrinter::operator()(Array value)
    {
        out << '[';
        bool first = true;
        for (const auto& elem : value) {
            if (first) first = false;
            else out << ", ";
            std::visit( ValuePrinter{ out }, elem.GetValue());
        }
        out << ']';
    }

    void ValuePrinter::operator()(Dict value)
    {
        out << '{';
        bool first = true;
        for (const auto& [key, node]  : value) {
            if (first) first = false;
            else out << ", ";
            out << "\"" << key << "\":";
            std::visit(ValuePrinter{ out }, node.GetValue());
        }
        out << '}';
    }

    void ValuePrinter::operator()(bool value)
    {
        out << std::boolalpha << value;
    }

    void ValuePrinter::operator()(int value)
    {
        out << value;
    }

    void ValuePrinter::operator()(double value)
    {
        out << value;
    }

    void ValuePrinter::operator()(std::string value)
    {
        out << "\""sv;
        for (const char& c : value) {
            if (c == '\n') {
                out << "\\n"sv;
                continue;
            }
            if (c == '\r') {
                out << "\\r"sv;
                continue;
            }
            if (c == '\"') out << "\\"sv;
            if (c == '\t') {
                out << "\t"sv;
                continue;
            }
            if (c == '\\') out << "\\"sv;
            out << c;
        }
        out << "\""sv;
    }

    void ValuePrinter::operator()(std::nullptr_t value)
    {
        out << "null";
    }

    void Print(const Document& doc, std::ostream& output) {
        std::visit(ValuePrinter{ output }, doc.GetRoot().GetValue());
    }
}  // namespace json