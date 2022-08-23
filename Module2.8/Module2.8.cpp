#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <memory.h>

template<class ForwardIt>
void uninitialized_default_construct(ForwardIt first, ForwardIt last)
{
	using Value = typename std::iterator_traits<ForwardIt>::value_type;
	ForwardIt current = first;
	try {
		for (; current != last; ++current) {
			::new (const_cast<void*>(static_cast<const volatile void*>(
				std::addressof(*current)))) Value;
		}
	}
	catch (...) {
		//std::destroy(first, current);
		for (auto i = first; i != current; i++) {
			delete(i);
		}
		throw;
	}
}

template<class T>
constexpr void destroy_at(T* p)
{
	if constexpr (std::is_array_v<T>)
		for (auto& elem : *p)
			(destroy_at)(std::addressof(elem));
	else
		p->~T();
}

// Процедура для преобразования в двоичную кучу поддерева с корневым узлом i, что является
// индексом в arr[]. n - размер кучи

void heapify(int arr[], int n, int i)
{
	int largest = i;
	// Инициализируем наибольший элемент как корень
	int l = 2 * i + 1; // левый = 2*i + 1
	int r = 2 * i + 2; // правый = 2*i + 2

 // Если левый дочерний элемент больше корня
	if (l < n && arr[l] > arr[largest])
		largest = l;

	// Если правый дочерний элемент больше, чем самый большой элемент на данный момент
	if (r < n && arr[r] > arr[largest])
		largest = r;

	// Если самый большой элемент не корень
	if (largest != i)
	{
		std::swap(arr[i], arr[largest]);

		// Рекурсивно преобразуем в двоичную кучу затронутое поддерево
		heapify(arr, n, largest);
	}
}

// Основная функция, выполняющая пирамидальную сортировку
void heapSort(int arr[], int n)
{
	// Построение кучи (перегруппируем массив)
	for (int i = n / 2 - 1; i >= 0; i--)
		heapify(arr, n, i);

	// Один за другим извлекаем элементы из кучи
	for (int i = n - 1; i >= 0; i--)
	{
		// Перемещаем текущий корень в конец
		std::swap(arr[0], arr[i]);

		// вызываем процедуру heapify на уменьшенной куче
		heapify(arr, i, 0);
	}
}

/* Вспомогательная функция для вывода на экран массива размера n*/
void printArray(int arr[], int n)
{
	for (int i = 0; i < n; ++i)
		std::cout << arr[i] << " ";
	std::cout << "\n";
}

void Perest(int arr[], int num)
{

	bool badStatus = false;

	for (int i = 0; i < num; i++) {
		for (int j = i; j < num; j++) {
			if ((arr[i] > num || arr[i] <= 0) || (i != j && arr[i] == arr[j]) || (arr[i] < 0)) {
				badStatus = true;
				break;
			}
		}
		if (badStatus) {
			std::cout << "NO" << std::endl << arr[i];

		}
	}
	std::cout << "YES";
}

class S {
public:
	std::string m{ "Message string." };
	S() {
		//every element creation takes 2 seconds
		std::cout << "Construction started.\n";
		std::chrono::seconds sec(2);
		std::this_thread::sleep_for(sec);
		std::cout << "Constructed.\n";
	};

	~S() {
		std::cout << "Destroyed.\n";
	};
};


int main()
{
	//allocate memory for 5 message
	constexpr int n{ 5 };
	alignas(alignof(S)) char arr[n * sizeof(S)];

	//get pointers to the first and first-after-last elements
	auto first{ reinterpret_cast<S*>(arr) };
	auto last{ first + n };

	auto count{ 0 };
	//initialize "on acquisition" idea)
	for (auto it{ first }; it != last; ++it) {
		uninitialized_default_construct(it, it + 1);
		std::cout << count++ << ' ' << it->m << '\n';
		destroy_at(it);
	}

	int arr1[] = { 12, 11, 13, 5, 6, 7 };
	int n1 = 6;

	heapSort(arr1, n1);

	std::cout << "Sorted array is \n";
	printArray(arr1, n1);

	int arr2[] = { 1,2,4 };
	int arr3[] = { 5,8 };

	int ptrarray[5];

	int i = 0;
	for (; i != (sizeof(arr2) / sizeof(*arr2)); i++)
	{
		ptrarray[i] = arr2[i];
	}

	for (int j = 0; j != (sizeof(arr3) / sizeof(*arr3)); j++, i++)
	{
		ptrarray[i] = arr3[j];
	}

	Perest(ptrarray, sizeof(ptrarray) / sizeof(*ptrarray));

	return 0;
}