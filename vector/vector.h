#ifndef VECTOR_H
#define VECTOR_H

#include<iostream>

template<typename T>

class Vector {
public:
	using value_type = T;
	using const_reference = const value_type&;
	class ConstIterator;
	class Iterator;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using reference = value_type&;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using iterator = Vector<T>::Iterator;
	using const_iterator = Vector<T>::ConstIterator;

private:
	size_t sz;
	size_t max_sz;
	static constexpr size_t min_sz{ 2 };
	value_type* values;
public:
	Vector<T>() : Vector<T>(min_sz) {

	}

	Vector<T>(const Vector<T>& src) {
		max_sz = src.max_sz;
		values = new value_type[max_sz];
		for (size_t i{ 0 }; i < src.sz; ++i)
			values[i] = src.values[i];
		sz = src.sz;
	}

	Vector<T>(size_t n) {
		if (n < min_sz)
			n = min_sz;
		sz = 0;
		max_sz = n;
		values = new value_type[max_sz];
	}


	Vector<T>(std::initializer_list<value_type> d) : Vector<T>(d.size()) {
		for (const auto& k : d) {
			values[sz++] = k;
		}
	}

	~Vector<T>() {
		delete[] values;
	}

	Vector<T> operator= (Vector src) {
		std::swap(sz, src.sz);
		std::swap(max_sz, src.max_sz);
		std::swap(values, src.values);
		return *this;
	}

	size_t size() const { return sz; }

	bool empty() const {
		return sz == 0;
	}

	void clear() {
		sz = 0;
	}

	void reserve(size_t n) {
		if (n > max_sz) {
			size_t new_sz = n;
			value_type* new_values = new value_type[new_sz];
			for (size_t i{ 0 }; i < sz; i++)
				new_values[i] = values[i];
			max_sz = new_sz;
			delete[] values;
			values = new_values;
		}
	}

	void shrink_to_fit() {
		if (sz < max_sz) {
			size_t new_sz = sz < min_sz ? min_sz : sz;
			value_type* new_values = new value_type[new_sz];
			for (size_t i{ 0 }; i < sz; i++)
				new_values[i] = values[i];
			max_sz = new_sz;
			delete[] values;
			values = new_values;
			max_sz = new_sz;
		}
	}


	void push_back(const_reference val) {
		if (sz >= max_sz) {
			reserve(max_sz * 2);
		}
		values[sz++] = val;
	}

	void pop_back() {
		if (sz == 0)
			throw std::runtime_error(" ");

		sz = sz - 1;

	}

	value_type& operator[](size_t index) {
		if (index >= sz)
			throw std::runtime_error(" ");

		return values[index];
	}

	value_type& operator[](size_t index) const {
		if (index >= sz)
			throw std::runtime_error(" ");

		return values[index];
	}

	size_t capacity() const { return max_sz; }

	std::ostream& print(std::ostream& o) const {
		o << "[";
		bool first{ true };
		for (size_t i{ 0 }; i < sz; i++) {
			if (first)
				first = false;
			else
				o << ", ";
			o << values[i];
		}
		o << "]";
		return o;
	}

	iterator begin() {
		 return Iterator(&values[0], &values[sz]);
	}

	iterator end() { return Iterator(&values[sz], &values[sz]); }

	const_iterator begin() const { return ConstIterator(&values[0], &values[sz]); }

	const_iterator end() const { return ConstIterator(&values[sz], &values[sz]); }

	iterator insert(const_iterator pos, const_reference val) {
		auto diff = pos - begin();
		if (diff<0 || static_cast<size_type>(diff)>sz)
			throw std::runtime_error("Iterator out of bounds");
		size_type current{ static_cast<size_type>(diff) };
		if (sz >= max_sz)
			reserve(max_sz * 2);
		for (auto i{ sz }; i-- > current;)
			values[i + 1] = values[i];
		values[current] = val;
		++sz;
		return iterator{ values + current, &values[sz]};
	}

	iterator erase(const_iterator pos) {
		auto diff = pos - begin();
		if (diff < 0 || static_cast<size_type>(diff) >= sz)
			throw std::runtime_error("Iterator out of bounds");
		size_type current{ static_cast<size_type>(diff) };
		for (auto i{ current }; i < sz - 1; ++i)
			values[i] = values[i + 1];
		--sz;
		return iterator{ values + current,&values[sz]};
	}

	class Iterator {
	public:
		using value_type = Vector<T>::value_type;
		using reference = Vector<T>::reference;
		using pointer = Vector<T>::pointer;
		using difference_type = Vector<T>::difference_type;
		using iterator_category = std::forward_iterator_tag;

	private:
		pointer ptr;
		pointer end;

	public:
		Iterator() {
			this->ptr = nullptr;
			this->end = nullptr;
		}

		Iterator(pointer ptr, pointer end) {
			this->ptr = ptr;
			this->end = end;
		}

		reference operator*() const {
			if (ptr == end) {
				throw std::runtime_error("end of array");
			}
			return *ptr;
		}

		pointer operator->() {
			if (ptr == end) {
				throw std::runtime_error("end of array");
			}
			return ptr;
		}

		bool operator==(const const_iterator& it) const {
			return ConstIterator(*this) == it;
		}

		bool operator!=(const const_iterator& it) const {
			return ConstIterator(*this) != it;
		}

		iterator& operator++() {
			if (ptr != end) {
				ptr++;
			}
			return *this;
		}

		iterator operator++(int) {
			iterator old = *this;
			if (ptr !=end) {
				++(*this);
			}
			return old;
		}

		operator const_iterator() const {
			return ConstIterator(ptr, end);
		}


	};

	class ConstIterator {
	public:
		using value_type = Vector<T>::value_type;
		using reference = Vector<T>::const_reference;
		using pointer = Vector<T>::const_pointer;
		using difference_type = Vector<T>::difference_type;
		using iterator_category = std::forward_iterator_tag;
	private:
		pointer ptr;
		pointer end;
	public:
		ConstIterator() {
			this->ptr = nullptr;
			this->end = nullptr;
		}
		ConstIterator(pointer ptr, pointer end) {
			this->ptr = ptr;
			this->end = end;
		}

		reference operator*() const {
			if (ptr == end) {
				throw std::runtime_error("end of array");
			}
			return *ptr;
		}

		pointer operator->() {
			if (ptr == end) {
				throw std::runtime_error("end of array");
			}
			return ptr;
		}

		bool operator==(const const_iterator& const_it) const {
			return this->ptr == const_it.ptr;
		}

		bool operator!=(const const_iterator& const_it) const {
			return this->ptr != const_it.ptr;
		}

		const_iterator& operator++() {
			if (ptr != end) {
				ptr++;
			}
			return *this;
		}

		const_iterator operator++(int) {
			const_iterator old = *this;
			if (ptr != end) {
				++(*this);
			}
			return old;
		}


		friend Vector<T>::difference_type operator-(const Vector<T>::ConstIterator& lop, const Vector<T>::ConstIterator& rop) {
			return lop.ptr - rop.ptr;
		}


	};

};


template<typename T>
std::ostream& operator<<(std::ostream& o, const Vector<T>& v) {
	return v.print(o);
}

#endif
