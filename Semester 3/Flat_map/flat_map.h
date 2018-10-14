#ifndef FLAT_MAP_H
#define FLAT_MAP_H
#include <iostream>

template <typename Key, typename Value>
class flat_map {
public:
	class iterator : public std::iterator<std::bidirectional_iterator_tag, std::pair<Key, Value>> {
	public:
		iterator() = delete;
		iterator(std::pair<Key, Value> * ptr);
		iterator(const iterator& it);

		iterator& operator=(const iterator& it);

		std::pair<Key, Value>& operator*() const;

		iterator& operator++();

		iterator& operator--();

		iterator operator+(int n);

		iterator operator-(int n);

		//возвращает количество позиций между итераторами *this и it
		size_t operator-(const iterator& it);

		friend bool operator<(const iterator& it1, const iterator& it2) {
			return (it1.var - it2.var) < 0;
		}
		friend bool operator>(const iterator& it1, const iterator& it2) {
			return (it1.var - it2.var) > 0;
		}
		friend bool operator<=(const iterator& it1, const iterator& it2) {
			return (it1.var - it2.var) <= 0;
		}
		friend bool operator>=(const iterator& it1, const iterator& it2) {
			return (it1.var - it2.var) >= 0;
		}

		friend bool operator==(const iterator& a, const iterator& b) {
			return (a.var == b.var);
		}
		friend bool operator!=(const iterator& a, const iterator& b) {
			return (a.var != b.var);
		}
	private:
		std::pair<Key, Value> * var = nullptr;
	};

	iterator begin() const;
	iterator end() const;

	flat_map();
	flat_map(size_t size);

	// example: flat_map<Key, Value> a = b;
	flat_map(const flat_map<Key, Value>& b);

	// Move constructor
	/* Example:
		flat_map<Key, Value> a1;
		flat_map<Key, Value> a2 = std::move(a1);*/
	flat_map(flat_map<Key, Value>&& a);

	// Move assignment operator
	/* Example:
		flat_map<Key, Value> a1;
		flat_map<Key, Value> a2;
		a1 = std::move(a2);*/
	flat_map<Key, Value>& operator=(flat_map<Key, Value>&& a);

	void swap(flat_map<Key, Value>& b);
	flat_map<Key, Value>& operator=(const flat_map<Key, Value>& b);

	// Очищает контейнер.
	void clear();

	// Проверка наличия значения по заданному ключу.
	bool contains(const Key& k) const;

	// Возвращает значение по ключу. Небезопасный метод.
	// В случае отсутствия ключа в контейнере, следует вставить в контейнер
	// значение, созданное конструктором по умолчанию и вернуть ссылку на него. 
	Value& operator[](const Key& k);

	// Возвращает значение по ключу. Бросает исключение при неудаче.
	Value& at(const Key& k);
	const Value& at(const Key& k) const;

	// Удаляет элемент по заданному ключу.
	bool erase(const Key& k);

	bool insert(const Key& k, const Value& val);
	size_t size() const;
	bool empty() const;
	~flat_map();
	template <typename Key1, typename Value1> friend bool operator==(const flat_map<Key1, Value1> & a, const flat_map<Key1, Value1> & b);
	template <typename Key1, typename Value1> friend bool operator!=(const flat_map<Key1, Value1> & a, const flat_map<Key1, Value1> & b);

private:
	size_t capacity = 0;
	size_t now_size = 0;
	std::pair<Key, Value> * arr = nullptr;

	// Поиск точного совпадения
	iterator correct_search(const Key& k, iterator left, iterator right) const;

	//Поиск подходящего индекса в случае, если искомого ключа нет
	iterator search_idx(const Key& k, iterator left, iterator right);
	void resize();
};

template <typename Key, typename Value>
flat_map<Key, Value>::iterator::iterator(std::pair<Key, Value> * ptr) : var(ptr) {
}

template <typename Key, typename Value>
flat_map<Key, Value>::iterator::iterator(const typename flat_map<Key, Value>::iterator& it) : var(it.var) {
}

template <typename Key, typename Value>
typename flat_map<Key, Value>::iterator& flat_map<Key, Value>::iterator::operator=(const typename flat_map<Key, Value>::iterator& it) {
	var = it.var;
	return *this;
}

template <typename Key, typename Value>
std::pair<Key, Value>& flat_map<Key, Value>::iterator::operator*() const {
	return *var;
}

template <typename Key, typename Value>
typename flat_map<Key, Value>::iterator& flat_map<Key, Value>::iterator::operator++() {
	var++;
	return *this;
}

template <typename Key, typename Value>
typename flat_map<Key, Value>::iterator& flat_map<Key, Value>::iterator::operator--() {
	var--;
	return *this;
}

template <typename Key, typename Value>
typename flat_map<Key, Value>::iterator flat_map<Key, Value>::iterator::operator+(int n) {
	auto it = *this;
	it.var += n;
	return it;
}

template <typename Key, typename Value>
typename flat_map<Key, Value>::iterator flat_map<Key, Value>::iterator::operator-(int n) {
	auto it = *this;
	it.var -= n;
	return it;
}

//возвращает количество позиций между итераторами *this и it
template <typename Key, typename Value>
size_t flat_map<Key, Value>::iterator::operator-(const typename flat_map<Key, Value>::iterator& it) {
	return abs(var - it.var);
}

template <typename Key, typename Value>
typename flat_map<Key, Value>::iterator flat_map<Key, Value>::begin() const {
	return iterator(arr);
}

template <typename Key, typename Value>
typename flat_map<Key, Value>::iterator flat_map<Key, Value>::end() const {
	return iterator(arr + now_size);
}

// Поиск точного совпадения
template <typename Key, typename Value>
typename flat_map<Key, Value>::iterator flat_map<Key, Value>::correct_search(const Key& k, iterator left, iterator right) const {
	if (left > right) {
		return end();
	}
	auto it = left + (left - right)/2;
	if (k == (*it).first) {
		return it;
	}

	if (k > (*it).first) {
		return correct_search(k, ++it, right);
	}
	else return correct_search(k, left, --it);
}

//Поиск подходящего индекса в случае, если искомого ключа нет
template <typename Key, typename Value>
typename flat_map<Key, Value>::iterator flat_map<Key, Value>::search_idx(const Key& k, iterator left, iterator right) {
	auto it = left + (right - left)/2;
	if (left > right) {
		return it;
	}

	if (k > (*it).first) {
		return search_idx(k, ++it, right);
	}
	else return search_idx(k, left, --it);
}

template <typename Key, typename Value>
void flat_map<Key, Value>::resize() {
	if (capacity != 0) {
		capacity *= 2;
	}
	else capacity = 10;

	std::pair<Key, Value> * new_arr = new std::pair<Key, Value>[capacity];
	std::copy(begin(), end(), iterator(new_arr));

	delete[] arr;
	arr = new_arr;
}

template <typename Key, typename Value>
flat_map<Key, Value>::flat_map() {
}

template <typename Key, typename Value>
flat_map<Key, Value>::flat_map(size_t size) :
capacity(size), arr(new std::pair<Key, Value>[size]) {
}

// example: flat_map<Key, Value> a = b;
template <typename Key, typename Value>
flat_map<Key, Value>::flat_map(const flat_map<Key, Value>& b) :
capacity(b.capacity), now_size(b.now_size), arr(new std::pair<Key, Value>[capacity]) {
	std::copy(b.begin(), b.end(), begin());
}

// Move constructor
/* Example: flat_map<Key, Value> a1;
			flat_map<Key, Value> a2 = std::move(a1);
*/
template <typename Key, typename Value>
flat_map<Key, Value>::flat_map(flat_map<Key, Value>&& a) {
	swap(a);
}

// Move assignment operator
/* Example: flat_map<Key, Value> a1;
			flat_map<Key, Value> a2;
			a1 = std::move(a2);
*/
template <typename Key, typename Value>
flat_map<Key, Value>& flat_map<Key, Value>::operator=(flat_map<Key, Value>&& a) {
	if (this != &a) {
		flat_map<Key, Value> b;
		b.swap(a);
		swap(b);
	}
	return *this;
}

template <typename Key, typename Value>
void flat_map<Key, Value>::swap(flat_map<Key, Value>& b) {
	if (this != &b) {
		std::swap(capacity, b.capacity);
		std::swap(now_size, b.now_size);
		std::swap(arr, b.arr);
	}
}

template <typename Key, typename Value>
flat_map<Key, Value>& flat_map<Key, Value>::operator=(const flat_map<Key, Value>& b) {
	if (this == &b) {
		return *this;
	}

	delete[] arr;
	capacity = b.capacity;
	now_size = b.now_size;
	arr = new std::pair<Key, Value>[capacity];
	std::copy(b.begin(), b.end(), begin());
	return *this;
}

// Очищает контейнер.
template <typename Key, typename Value>
void flat_map<Key, Value>::clear() {
	delete[] arr;
	capacity = 0;
	now_size = 0;
	arr = nullptr;
}

// Проверка наличия значения по заданному ключу.
template <typename Key, typename Value>
bool flat_map<Key, Value>::contains(const Key& k) const {
	return correct_search(k, begin(), end() - 1) != end();
}

// Возвращает значение по ключу. Небезопасный метод.
// В случае отсутствия ключа в контейнере, следует вставить в контейнер
// значение, созданное конструктором по умолчанию и вернуть ссылку на него.
template <typename Key, typename Value>
Value& flat_map<Key, Value>::operator[](const Key& k) {
	auto it = correct_search(k, begin(), end() - 1);
	if (it == end()) {
		insert(k, Value());
		it = correct_search(k, begin(), end() - 1);
	}
	return (*it).second;
}

// Возвращает значение по ключу. Бросает исключение при неудаче.
template <typename Key, typename Value>
Value& flat_map<Key, Value>::at(const Key& k) {
	auto it = correct_search(k, begin(), end() - 1);
	if (it == end()) {
		throw /*"This key not exist";*/ std::runtime_error("This key not exist");
	}
	return (*it).second;
}

template <typename Key, typename Value>
const Value& flat_map<Key, Value>::at(const Key& k) const {
	auto it = correct_search(k, begin(), end() - 1);
	if (it == end()) {
		throw /*"This key not exist";*/std::runtime_error("This key not exist");
	}
	return (*it).second;
}

// Удаляет элемент по заданному ключу.
template <typename Key, typename Value>
bool flat_map<Key, Value>::erase(const Key& k) {
	auto it = correct_search(k, begin(), end() - 1);
	if (it == end()) {
		return false;
	}
	std::copy(it + 1, end(), it);
	now_size--;
	return true;
}

template <typename Key, typename Value>
bool flat_map<Key, Value>::insert(const Key& k, const Value& val) {
	if (now_size == capacity) {
		resize();
	}

	auto it = begin();
	if (now_size != 0) {
		it = search_idx(k, begin(), end() - 1);
		if (it < end() && k == (*it).first) {
			return false;
		}
		
		std::copy_backward(it, end(), end() + 1);
	}

	(*it).first = k;
	(*it).second = val;
	now_size++;
	return true;
}

template <typename Key, typename Value>
size_t flat_map<Key, Value>::size() const {
	return now_size;
}

template <typename Key, typename Value>
bool flat_map<Key, Value>::empty() const {
	return (now_size == 0);
}

template <typename Key, typename Value>
flat_map<Key, Value>::~flat_map() {
	delete[] arr;
}

template <typename Key, typename Value>
bool operator==(const flat_map<Key, Value> & a, const flat_map<Key, Value> & b) {
	if (a.now_size == b.now_size) {
		for (size_t i = 0; i < a.now_size; i++) {
			if (a.arr[i] != b.arr[i]) {
				return false;
			}
		}
		return true;
	}
	return false;
}

template <typename Key, typename Value>
bool operator!=(const flat_map<Key, Value> & a, const flat_map<Key, Value> & b) {
	return !(a == b);
}

#endif