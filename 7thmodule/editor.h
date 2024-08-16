#include <string>
#include <vector>
#include <list>

class Editor {
public:
    Editor() {

    }
    // сдвинуть курсор влево
    void Left() {
        if (it_ == text_.begin() || text_.empty()) return;
        --it_;
    }
    // сдвинуть курсор вправо 
    void Right() {
        if (it_ == text_.end() || text_.empty()) return;
        ++it_;
    }
    // вставить символ token
    void Insert(char token) {
         text_.insert(it_, token);
    }
    // вырезать не более tokens символов, начиная с текущей позиции курсора
    void Cut(size_t tokens = 1) {
        if (tokens > static_cast<size_t>(std::distance(it_, text_.end()))) {
            tokens = static_cast<size_t>(std::distance(it_, text_.end()));
        }
        auto cut = std::next(it_, tokens);
        buffer_.assign(it_, cut);
        it_ = text_.erase(it_, cut);
    }
    // cкопировать не более tokens символов, начиная с текущей позиции курсора
    void Copy(size_t tokens = 1) {
        if (tokens > static_cast<size_t>(std::distance(it_, text_.end()))) {
            tokens = static_cast<size_t>(std::distance(it_, text_.end()));
        }

        buffer_.assign(it_, std::next(it_, tokens));
    }
    // вставить содержимое буфера в текущую позицию курсора
    void Paste() {
        text_.insert(it_, buffer_.begin(), buffer_.end());
       // buffer_.clear();
    }
    // получить текущее содержимое текстового редактора
    std::string GetText() const {
        return{ text_.begin(), text_.end() };
    }

private:
    std::list<char> text_;
    std::list<char> buffer_;
    std::list<char>::iterator it_ = text_.begin();
};