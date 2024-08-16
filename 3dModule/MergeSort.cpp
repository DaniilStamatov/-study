#include <vector>
#include <iostream>


template<typename Iterator>
void merge(Iterator begin, Iterator mid, Iterator right) {
	int n1 = mid - begin + 1;
	int n2 = right - mid;

	std::vector<int> leftArr(n1);
	std::vector<int> rightArr(n2);

	for (int i = 0; i < n1; i++) {
		leftArr[i] = *(begin + i);
	}

	for (int i = 0; i < n2; i++) {
		rightArr[i] = *(mid + i);
	} 

	int i = 0, j = 0, k = 0;
	while (i < n1 && j < n2) {
		if (leftArr[i] <= rightArr[j]) {
			*(begin + k) = leftArr[i];
			i++;
		}
		else {
			*(begin + k) = leftArr[j];
			j++;
		}
		k++;
	}

	while (i < n1) {
		*(begin + k) = leftArr[i];
		i++;
		k++;
	}

	while (j < n2) {
		*(begin + k) = leftArr[j];
		j++;
		k++;
	}
}

template<typename Iterator>
void MergeSort(Iterator begin, Iterator end) {
	if (begin < end) {
		std::vector<int>::iterator mid = begin + (end - begin) / 2;

		MergeSort(begin, mid);
		MergeSort(mid, end);

		merge(begin, mid, end);
	}
}


template<typename Iterator>
void PrintRange(Iterator begin, Iterator end) {
	for (auto it = begin; it != end; ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;
}

int main() {
	std::vector<int> test_vector = { 12, 11, 13, 5, 6, 7 };
	std::cout << "Original array: ";

	MergeSort(test_vector.begin(), test_vector.end());

	std::cout << "Sorted array: ";
	PrintRange(test_vector.begin(), test_vector.end());

	return 0;
}