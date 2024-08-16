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
        // ����� ������ ����������� �������������, ����� �� ������� ������
        // ��� ������ � ��������� ������� ���������
        friend class SingleLinkedList;

        // �������������� ����������� ��������� �� ��������� �� ���� ������
        explicit BasicIterator(Node* node) : node_(node) {

            // ���������� ����������� ��������������
        }

	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = Type;
		using difference_type = std::ptrdiff_t;
		using pointer = ValueType*;
		using reference = ValueType&;

        BasicIterator() = default;

        // �������������� �����������/����������� �����������
        // ��� ValueType, ����������� � Type, ������ ���� ����������� ������������
        // ��� ValueType, ����������� � const Type, ������ ���� ��������������� ������������
        BasicIterator(const BasicIterator<Type>& other) noexcept : node_(other.node_){
        }

        // ����� ���������� �� ������� �������������� �� ���������� ��������� = ��� �������
        // ����������������� ������������ �����������, ���� ������� �������� = �
        // �������� ���������� ������������� ��� �� ���
        BasicIterator& operator=(const BasicIterator& rhs) = default;

        // �������� ��������� ���������� (� ���� ������� ��������� ��������� ����������� ��������)
        // ��� ��������� �����, ���� ��� ��������� �� ���� � ��� �� ������� ������ ���� �� end()
        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        // �������� �������� ���������� �� �����������
        // �������������� !=
        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return this->node_ != rhs.node_;
        }

        // �������� ��������� ���������� (� ���� ������� ��������� ��������)
        // ��� ��������� �����, ���� ��� ��������� �� ���� � ��� �� ������� ������ ���� �� end()
        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        // �������� �������� ���������� �� �����������
        // �������������� !=
        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return this->node_ != rhs.node_;
        }

        // �������� ��������������. ����� ��� ������ �������� ��������� �� ��������� ������� ������
        // ���������� ������ �� ������ ����
        // ��������� ���������, �� ������������ �� ������������ ������� ������, �������� � �������������� ���������
        BasicIterator& operator++() noexcept {
            if (node_ == nullptr) {
                throw std::runtime_error("Incrementing an invalid iterator");
            }
            node_ = node_->next_;
            return *this;
        }

        // �������� ��������������. ����� ��� ������ �������� ��������� �� ��������� ������� ������
        // ���������� ������� �������� ���������
        // ��������� ���������, �� ������������ �� ������������ ������� ������,
        // �������� � �������������� ���������
        BasicIterator operator++(int) noexcept {
            if (node_ == nullptr) {
                throw std::runtime_error("Incrementing an invalid iterator");
            }

            auto old_value(*this);
            ++(*this);
            return old_value;
        }

        // �������� �������������. ���������� ������ �� ������� �������
        // ����� ����� ��������� � ���������, �� ������������ �� ������������ ������� ������,
        // �������� � �������������� ���������
        [[nodiscard]] reference operator*() const noexcept {
            assert(node_ != nullptr);

            return node_->value_;
        }

        // �������� ������� � ����� ������. ���������� ��������� �� ������� ������� ������
        // ����� ����� ��������� � ���������, �� ������������ �� ������������ ������� ������,
        // �������� � �������������� ���������
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
        // ���������� ����������� ��������������
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
	// ����������� ������ begin/end ��� ������ ������ ��� ����������� ����������� ��� ���������
	[[nodiscard]] ConstIterator begin() const noexcept {
        return cbegin();
	}
	[[nodiscard]] ConstIterator end() const noexcept { 
        return cend();
	}
	// ������ ��� �������� ��������� ����������� ���������� � �������������� ����������
	[[nodiscard]] ConstIterator cbegin() const noexcept { 
        return ConstIterator{ head_.next_ };
	}

    // ������ ��� �������� ��������� ����������� ���������� � �������������� ����������
	[[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator{ nullptr };
	}

    // ���������� ��������, ����������� �� ������� ����� ������ ��������� ������������ ������.
   // �������������� ���� �������� ������ - ������� ������������� ������� � �������������� ���������
    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator{ &head_ };
        // ���������� ��������������
    }

    // ���������� ����������� ��������, ����������� �� ������� ����� ������ ��������� ������������ ������.
    // �������������� ���� �������� ������ - ������� ������������� ������� � �������������� ���������
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator{const_cast<Node*>(&head_)};
        // ���������� ��������������
    }

    // ���������� ����������� ��������, ����������� �� ������� ����� ������ ��������� ������������ ������.
    // �������������� ���� �������� ������ - ������� ������������� ������� � �������������� ���������
    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return cbefore_begin();
        // ���������� ��������������
    }

    /*
     * ��������� ������� value ����� ��������, �� ������� ��������� pos.
     * ���������� �������� �� ����������� �������
     * ���� ��� �������� �������� ����� ��������� ����������, ������ ��������� � ������� ���������
     */
    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        assert(pos.node_ != nullptr);
        Node* node = new Node(value, pos.node_->next_);
        pos.node_->next_ = node;
        ++size_;
        return Iterator{ node };
        // ��������. ���������� ����� ��������������
    }

    // ������� ������ �������� ��������� ������ �� ����� O(1)
    void PopFront() noexcept {
        assert(!IsEmpty());

        Node* new_head = head_.next_->next_;
        delete head_.next_;
        head_.next_ = new_head;
        --size_;
        // ���������� ����� ��������������
    }

    /*
     * ������� �������, ��������� �� pos.
     * ���������� �������� �� �������, ��������� �� ��������
     */
    Iterator EraseAfter(ConstIterator pos) noexcept {
        assert(!IsEmpty());
        assert(pos.node_ != nullptr);

        auto temp = pos.node_->next_->next_;
        delete pos.node_->next_;
        pos.node_->next_ = temp;

        --size_;
        return Iterator{ pos.node_->next_ };
        // ��������. ���������� ����� ��������������
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
        // ���������� ����� ����������� ������� ��������������
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

        // ������ ���� ��������� �������� � �������� �������
        for (auto it = elem.begin(); it != elem.end(); ++it) {
            tmp_reverse.PushFront(*it);
        }
        // ������ ���� ��������� �������� � ������ ��� ������ �������
        for (auto it = tmp_reverse.begin(); it != tmp_reverse.end(); ++it) {
            elem_copy.PushFront(*it);
        }

        swap(elem_copy);
    }

	Node head_;
	size_t size_;
};

