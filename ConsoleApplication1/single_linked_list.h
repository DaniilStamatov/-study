#pragma once
#include <cassert>
#include <cstddef>
#include <string>
#include <utility>
#include <algorithm>
template<typename Type>
class SingleLinkedList {
	struct Node {
		Node() = default;
		Node(const Type& value, Node* next) : value_(value), next_(next) {

		}

		Type value_;
		Node* next_ = nullptr;
	};


	template<typename ValueType>
	class BasicIterator {
        // Класс списка объявляется дружественным, чтобы из методов списка
        // был доступ к приватной области итератора
        friend class SingleLinkedList;

        // Конвертирующий конструктор итератора из указателя на узел списка
        explicit BasicIterator(Node* node) : node_(node) {

            // Реализуйте конструктор самостоятельно
        }

	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = Type;
		using difference_type = std::ptrdiff_t;
		using pointer = ValueType*;
		using reference = ValueType&;

        BasicIterator() = default;

        // Конвертирующий конструктор/конструктор копирования
        // При ValueType, совпадающем с Type, играет роль копирующего конструктора
        // При ValueType, совпадающем с const Type, играет роль конвертирующего конструктора
        BasicIterator(const BasicIterator<Type>& other) noexcept : node_(other.node_){
        }

        // Чтобы компилятор не выдавал предупреждение об отсутствии оператора = при наличии
        // пользовательского конструктора копирования, явно объявим оператор = и
        // попросим компилятор сгенерировать его за нас
        BasicIterator& operator=(const BasicIterator& rhs) = default;

        // Оператор сравнения итераторов (в роли второго аргумента выступает константный итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        // Оператор проверки итераторов на неравенство
        // Противоположен !=
        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return this->node_ != rhs.node_;
        }

        // Оператор сравнения итераторов (в роли второго аргумента итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        // Оператор проверки итераторов на неравенство
        // Противоположен !=
        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return this->node_ != rhs.node_;
        }

        // Оператор прединкремента. После его вызова итератор указывает на следующий элемент списка
        // Возвращает ссылку на самого себя
        // Инкремент итератора, не указывающего на существующий элемент списка, приводит к неопределённому поведению
        BasicIterator& operator++() noexcept {
            if (node_ == nullptr) {
                throw std::runtime_error("Incrementing an invalid iterator");
            }
            node_ = node_->next_;
            return *this;
        }

        // Оператор постинкремента. После его вызова итератор указывает на следующий элемент списка
        // Возвращает прежнее значение итератора
        // Инкремент итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        BasicIterator operator++(int) noexcept {
            if (node_ == nullptr) {
                throw std::runtime_error("Incrementing an invalid iterator");
            }

            auto old_value(*this);
            ++(*this);
            return old_value;
        }

        // Операция разыменования. Возвращает ссылку на текущий элемент
        // Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        [[nodiscard]] reference operator*() const noexcept {
            assert(node_ != nullptr);

            return node_->value_;
        }

        // Операция доступа к члену класса. Возвращает указатель на текущий элемент списка
        // Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        [[nodiscard]] pointer operator->() const noexcept {
            assert(node_ != nullptr);

            return &node_->value_;
        }

    private:
        Node* node_ = nullptr;
    };
	
public:

    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    SingleLinkedList(std::initializer_list<Type> values) {
        Assign(values);
    }

    SingleLinkedList(const SingleLinkedList& other) :size_(0), head_() {
        assert( head_.next_ == nullptr && size_ == 0);
        Assign(other);
        // Реализуйте конструктор самостоятельно
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        assert(&rhs != this);
        if (rhs.IsEmpty()) Clear();
        SingleLinkedList copy(rhs);
        swap(copy);
        return *this;
    }

	SingleLinkedList() : size_(0), head_(){
		
	}

	~SingleLinkedList() {
		Clear();
	}

	void Clear() {
		while (head_.next_ != nullptr) {
			Node* front_node = head_.next_;
			head_.next_ = front_node->next_;
			delete front_node;
		}
		size_ = 0;
	}

	[[nodiscard]] Iterator begin() noexcept { 
        Iterator it(head_.next_);
        return it;
        //return Iterator{head_.next_};
	}
	[[nodiscard]] Iterator end() noexcept {
        return Iterator{ nullptr };
		
	}
	// Константные версии begin/end для обхода списка без возможности модификации его элементов
	[[nodiscard]] ConstIterator begin() const noexcept {
        return cbegin();
	}
	[[nodiscard]] ConstIterator end() const noexcept { 
        return cend();
	}
	// Методы для удобного получения константных итераторов у неконстантного контейнера
	[[nodiscard]] ConstIterator cbegin() const noexcept { 
        return ConstIterator{ head_.next_ };
	}

    // Методы для удобного получения константных итераторов у неконстантного контейнера
	[[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator{ nullptr };
	}

    // Возвращает итератор, указывающий на позицию перед первым элементом односвязного списка.
   // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator{ &head_ };
        // Реализуйте самостоятельно
    }

    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator{const_cast<Node*>(&head_)};
        // Реализуйте самостоятельно
    }

    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return cbefore_begin();
        // Реализуйте самостоятельно
    }

    /*
     * Вставляет элемент value после элемента, на который указывает pos.
     * Возвращает итератор на вставленный элемент
     * Если при создании элемента будет выброшено исключение, список останется в прежнем состоянии
     */
    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        assert(pos.node_ != nullptr);
        Node* node = new Node(value, pos.node_->next_);
        pos.node_->next_ = node;
        ++size_;
        return Iterator{ node };
        // Заглушка. Реализуйте метод самостоятельно
    }

    // Удаляет первый элемента непустого списка за время O(1)
    void PopFront() noexcept {
        assert(!IsEmpty());

        Node* new_head = head_.next_->next_;
        delete head_.next_;
        head_.next_ = new_head;
        --size_;
        // Реализуйте метод самостоятельно
    }

    /*
     * Удаляет элемент, следующий за pos.
     * Возвращает итератор на элемент, следующий за удалённым
     */
    Iterator EraseAfter(ConstIterator pos) noexcept {
        assert(!IsEmpty());
        assert(pos.node_ != nullptr);

        auto temp = pos.node_->next_->next_;
        delete pos.node_->next_;
        pos.node_->next_ = temp;

        --size_;
        return Iterator{ pos.node_->next_ };
        // Заглушка. Реализуйте метод самостоятельно
        return {};
    }

	size_t GetSize() const {
		return size_;
	}

	bool IsEmpty() const {
		return size_ == 0;
	}

    void swap(SingleLinkedList& other) noexcept {
        std::swap(this->head_.next_, other.head_.next_);
        std::swap(this->size_, other.size_);
        // Реализуйте обмен содержимого списков самостоятельно
    }

    void PushFront(const Type& value) {
        head_.next_ = new Node(value, head_.next_);
        ++size_;
    }

private:

    template<typename T>
    void Assign(T& elem) {
        SingleLinkedList elem_copy;
        SingleLinkedList tmp_reverse;

        // первый цикл вставляет элементы в обратном порядке
        for (auto it = elem.begin(); it != elem.end(); ++it) {
            tmp_reverse.PushFront(*it);
        }
        // второй цикл вставляет элементы в нужном для обмена порядке
        for (auto it = tmp_reverse.begin(); it != tmp_reverse.end(); ++it) {
            elem_copy.PushFront(*it);
        }

        swap(elem_copy);
    }

	Node head_;
	size_t size_;
};

