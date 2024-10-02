#include <iostream>

using namespace std;
class AnyStorageBase {
public:
    virtual ~AnyStorageBase() {}
    virtual void Print(std::ostream& out) const = 0;
};

template <typename T>
class AnyStorage : public AnyStorageBase {
private:
    T data_;
public:
     template<typename S>
    AnyStorage(S&& data)
        : data_(forward<S>(data))
    {}

	void Print(std::ostream& out) const override {
		out << data_ << std::endl;
	}

};


class Any {
private:
	std::unique_ptr<AnyStorageBase> val_;
public:

	template <typename S>
	Any(S&& value) {
		using Initial = std::remove_reference_t<S>;
		val_ = std::make_unique<AnyStorage<Initial>>(std::forward<S>(value));
	}

	void Print(std::ostream& out) const {
        val_->Print(out);
	}
};


class Dumper {
public:
    Dumper() {
        std::cout << "construct"sv << std::endl;
    }
    ~Dumper() {
        std::cout << "destruct"sv << std::endl;
    }
    Dumper(const Dumper&) {
        std::cout << "copy"sv << std::endl;
    }
    Dumper(Dumper&&) {
        std::cout << "move"sv << std::endl;
    }
    Dumper& operator=(const Dumper&) {
        std::cout << "= copy"sv << std::endl;
        return *this;
    }
    Dumper& operator=(Dumper&&) {
        std::cout << "= move"sv << std::endl;
        return *this;
    }
};



ostream& operator<<(ostream& out, const Any& arg) {
    arg.Print(out);
    return out;
}

ostream& operator<<(ostream& out, const Dumper&) {
    return out;
}
//
//int main() {
//    Any any_int(42);
//    Any any_string("abc"s);
//
//    // операция вывода Any в поток определена чуть выше в примере
//    cout << any_int << endl << any_string << endl;
//
//    Any any_dumper_temp{ Dumper() };
//
//    Dumper auto_dumper;
//    Any any_dumper_auto(auto_dumper);
//}