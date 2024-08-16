#include <vector>
#include <iostream>
#include <numeric>
#include <algorithm>

using namespace std;
template<typename T>
class Stack {
public:
	void Push(T value) {
		stack.push_back(value);
	}

	const T& Peek() const{
		return stack.back();
	}

	void Pop() {
		if (!IsEmpty()) {
			stack.pop_back();
		}
	}

	size_t Size() const{
		return stack.size();
	}

	bool IsEmpty() const {
		return stack.empty();
	}

	void Print() const {
		if (!IsEmpty()) {
			for (size_t i = 0; i < stack.size(); i++) {
				std::cout << stack[i] << " ";
			}
			std::cout << std::endl;
		}
		
	}
private:
	std::vector<T> stack;

	T min;
};

template <typename Type>
class StackMin {
public:
	void Push(const Type& element) {
		elements_.Push(element);
		if (min_elements_.IsEmpty() || element <= min_elements_.Peek()) {
			min_elements_.Push(element);
		}
		// �������� ���������� ������
	}
	void Pop() {
		Type element = elements_.Peek();
		elements_.Pop();
		if (element == min_elements_.Peek()) {
			min_elements_.Pop();
		}
		// �������� ���������� ������
	}
	const Type& Peek() const {
		return elements_.Peek();
	}
	Type& Peek() {
		return elements_.Peek();
	}
	void Print() const {
		// ������ ����� ������ �� ��������� �� �����,
		// �� ���� �� ��� �����, �� ������ ��� �����������
		elements_.Print();
	}
	uint64_t Size() const {
		return elements_.size();
	}
	bool IsEmpty() const {
		return elements_.IsEmpty();
	}
	const Type& PeekMin() const {
		return min_elements_.Peek();
		// �������� ���������� ������
	}
	Type& PeekMin() {
		return min_elements_.Peek();
		// �������� ���������� ������
	}
private:
	Stack<Type> elements_;
	Stack<Type> min_elements_;
};

template<typename T>
class SortedStack {
public:
	void Push(const T& element) {
		if (IsEmpty() || elements_.Peek() >= element) {
			elements_.Push(element);
		}
		else {
			T temp = element;
			while (!IsEmpty() && temp >= elements_.Peek()) {
				temp_stack_.Push(elements_.Peek());
				elements_.Pop();
			}

			elements_.Push(element);
			while (!temp_stack_.IsEmpty()) {
				temp = temp_stack_.Peek();
				elements_.Push(temp);
				temp_stack_.Pop();
			}
		}
	}

	void Pop() {
		elements_.Pop();
	}

	const T& Peek() const {
		return elements_.Peek();
	}

	size_t Size() const {
		return elements_.Size();
	}

	bool IsEmpty() const {
		return elements_.IsEmpty();
	}

	void Print() const {
		elements_.Print();
	}

private:
	Stack<T> elements_;
	Stack<T> temp_stack_;
};


template<typename T>
class Queue {
private:
	Stack<T> stack1_;
	Stack<T> stack2_;

public:
	bool isEmpty() const {
		return stack1_.IsEmpty() && stack2_.IsEmpty();
	}

	void Push(const T& element) {
		stack1_.Push(element);
		// �������� ����������
	}
	void Pop() {
		if (stack2_.IsEmpty()) {
			while (!stack1_.IsEmpty()) {
				stack2_.Push(stack1_.Peek());
				stack1_.Pop();
			}
			
		}
		stack2_.Pop();
		// �������� ����������
	}
	const T& Front() {
		if (stack2_.IsEmpty()) {
			while (!stack1_.IsEmpty()) {
				stack2_.Push(stack1_.Peek());
				stack1_.Pop();
			}
		}
		return stack2_.Peek();
		// �������� ����������
	}
	const T& Front() const {
		if (stack2_.IsEmpty()) {
			while (!stack1_.IsEmpty()) {
				stack2_.Push(stack1_.Peek());
				stack1_.Pop();
			}
		}
		return stack2_.Peek();
		// �������� ����������
	}
	uint64_t Size() const {
		return stack1_.Size() + stack2_.Size();
		// �������� ����������
	}
};

template<typename Iterator>
void PrintRange(Iterator begin, Iterator end) {
	for (auto it = begin; it != end; ++it) {
		cout << *it << " ";
	}
	cout << endl;
}




int main() {
	Queue<int> queue;
	vector<int> values(5);

	// ��������� ������ ��� ������������ �������
	iota(values.begin(), values.end(), 1);
	// ������������ ��������
	random_shuffle(values.begin(), values.end());

	PrintRange(values.begin(), values.end());

	cout << "��������� �������"s << endl;

	// ��������� ������� � ������� ������� � ������ �������
	for (int i = 0; i < 5; ++i) {
		queue.Push(values[i]);
		cout << "����������� ������� "s << values[i] << endl;
		cout << "������ ������� ������� "s << queue.Front() << endl;
	}

	cout << "�������� �������� �� �������"s << endl;

	// ������� ������� � ������ ������� � ����������� �������� �� ������
	while (!queue.isEmpty()) {
		// ������� ����� ��������� ��������� �������, � ����� �����������,
		// ��� ��� �������� Front �� ������ ������� �� ����������
		cout << "����� �������� ������� "s << queue.Front() << endl;
		queue.Pop();
	}
	return 0;
}