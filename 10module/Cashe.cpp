#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

template <typename Key, typename Value, typename ValueFactoryFn>
class Cache {
private:
    std::unordered_map<Key, std::weak_ptr<Value>> values_;
    ValueFactoryFn factory_;

public:
    // ������ ��� ������������������ �������� ��������, ��������� �� ���������
    Cache() = default;

    // ������ ���, ������������������ �������, �������� ���� �������,
    // ��������� (��� ������ operator() �������� �� ��� �����)
    // ������� ������ ������� shared_ptr<Value> ���� unique_ptr<Value>.
    // ������ �������������:
    // shared_ptr<Value> value = value_factory(key);
    explicit Cache(ValueFactoryFn value_factory): factory_(value_factory) {
        
        // ���������� ����������� ��������������
    }

    // ���������� �������������� �������� �� �����. ���� �������� ����������� ��� ��� �������,
    // ��� ������ ���� ������� � ������� ������� � ��������� � ����.
    // ���� �� ������ ��� ������� ������, �� ������ ���� ����� �� ����
    std::shared_ptr<Value> GetValue(const Key& key) {
        if (!values_.count(key))//!values_.count(key)
        {
            auto value = factory_(key);
            values_[key] = std::weak_ptr{ value };
            return value;
        }
        auto wp = values_.at(key);
        if (values_.at(key).expired()) {
            auto sp = factory_(key);
            return sp;
        }
        return values_.at(key).lock();
        // ��������. ���������� ����� ��������������
    }
    
};

// ������ �������, ������������ � ����
class Object {
public:
    explicit Object(std::string id)
        : id_(std::move(id))  //
    {
        using namespace std;
        cout << "Object '"sv << id_ << "' has been created"sv << endl;
    }

    const std::string& GetId() const {
        return id_;
    }

    ~Object() {
        using namespace std;
        cout << "Object '"sv << id_ << "' has been destroyed"sv << endl;
    }

private:
    std::string id_;
};

using ObjectPtr = std::shared_ptr<Object>;

struct ObjectFactory {
    ObjectPtr operator()(std::string id) const {
        return std::make_shared<Object>(std::move(id));
    }
};

void Test1() {
    using namespace std;
    ObjectPtr alice1;
    // ���, �������� Object, ����������� ��� ������ ObjectFactory,
    // ������ � ������� ��������������� �� ����� ���� string
    Cache<string, Object, ObjectFactory> cache;

    // ��������� ������� Alice � Bob
    alice1 = cache.GetValue("Alice");
    auto bob = cache.GetValue("Bob");
    // ������ ��������� ��� ������ ������� � ����������� id
    assert(alice1 != bob);
    assert(alice1->GetId() == "Alice");
    assert(bob->GetId() == "Bob");

    // ��������� ������ ������� Alice ������ ������� ������������ ������
    auto alice2 = cache.GetValue("Alice");
    assert(alice1 == alice2);

    // ��������� alice_wp ������ �� ������ ������� Alice
    weak_ptr alice_wp{ alice1 };
    alice1.reset();
    assert(!alice_wp.expired());
    cout << "---"sv << endl;
    alice2.reset();
    // ������ Alice ����� �����, ��� ��� �� ���� ������ �� ��������� shared_ptr
    assert(alice_wp.expired());
    cout << "---"sv << endl;
    // ������ Bob ����� �����, ����� ���������� ��������� bob

    alice1 = cache.GetValue("Alice");  // ������ 'Alice' ����� ������ ������
    cout << "---"sv << endl;
}

struct Book {
    Book(std::string title, std::string content)
        : title(std::move(title))
        , content(std::move(content)) {
    }

    std::string title;
    std::string content;
};

// �������������� ������, ����������� ����� �� unordered_map
class BookLoader {
public:
    using BookStore = std::unordered_map<std::string, std::string>;

    // ��������� ����������� ������ �� ��������� ���� � ������ �� ����������-������� ��������
    explicit BookLoader(const BookStore& store, size_t& load_count): store_(store), load_count_(load_count) {

        // ���������� ����������� ��������������
    }

    // ��������� ����� �� ��������� �� � �������� � ���������� ���������
    // � ������ �������� �������� (����� ���� � ���������)
    // ����� ��������� �������� �������� �������� load_count, ����������� � �����������, �� 1.
    // ���� ����� � ��������� �����������, ����� ��������� ���������� std::out_of_range,
    // � ������� �� �����������
    std::shared_ptr<Book> operator()(const std::string& title) const {
        if (!store_.count(title)) {
            throw std::out_of_range("�� ����������");
        }
        auto sp = std::make_shared<Book>(title, store_.at(title));
        load_count_++;

        return sp;
    }

private:
    const std::unordered_map<std::string, std::string>& store_;
    size_t& load_count_;
    // �������� ����������� ������ �/��� ������
};

void Test2() {
    using namespace std;
    // ��������� ����.
    BookLoader::BookStore books{
        {"Sherlock Holmes"s,
         "To Sherlock Holmes she is always the woman. I have seldom heard him mention her under any other name."s},
        {"Harry Potter"s, "Chapter 1. The boy who lived. ...."s},
    };
    using BookCache = Cache<string, Book, BookLoader>;

    size_t load_count = 0;
    // ������ ���, ������� ����� ������������� BookLoader ��� �������� ���� �� ��������� books
    BookCache book_cache{ BookLoader{books, load_count} };

    // ����������� ����� ������ ��������� ������ �� ���������
    auto book1 = book_cache.GetValue("Sherlock Holmes"s);
    assert(book1);
    assert(book1->title == "Sherlock Holmes"s);
    assert(book1->content == books.at(book1->title));
    assert(load_count == 1);

    // ��������� ������ ����� ������ ���������� �������������� ��������
    auto book2 = book_cache.GetValue("Sherlock Holmes"s);
    assert(book2);
    assert(book1 == book2);
    assert(load_count == 1);

    weak_ptr<Book> weak_book{ book1 };
    assert(!weak_book.expired());
    book1.reset();
    book2.reset();
    // ����� ���������, ��� ������ �� �� ��������� ��������� ��������� ��� ����
    assert(weak_book.expired());

    book1 = book_cache.GetValue("Sherlock Holmes"s);
    assert(load_count == 2);
    assert(book1);

    try {
        book_cache.GetValue("Fifty Shades of Grey"s);
        // BookLoader �������� ���������� ��� ������� ��������� �������������� �����
        assert(false);
    }
    catch (const std::out_of_range&) {
        /* �� ���������. ����� ����� ��� � �������������� */
    }
    catch (...) {
        cout << "Unexpected exception"sv << endl;
    }
    // ������� �������� �� ������ ����������, ��� ��� ����� ��� � ���������
    assert(load_count == 2);

    // ��������� ����� � ���������
    books["Fifty Shades of Grey"s] = "I scowl with frustration at myself in the mirror..."s;

    try {
        auto book = book_cache.GetValue("Fifty Shades of Grey"s);
        // ������ ����� ������ ���� ������� �������
        assert(book->content == books.at("Fifty Shades of Grey"s));
    }
    catch (...) {
        assert(false);
    }
    // ������� �������� ������ ����������, ��� ��� ����� ���� � ���������
    assert(load_count == 3);
}

//int main() {
//    Test1();
//    Test2();
//}