#pragma once

#include <cstddef>
#include <vector>
#include <stdexcept>

template<typename T>
class RevolvingCollection
{
public:
	typedef typename std::vector<T>::iterator IteratorType;
	typedef typename std::vector<T>::const_iterator ConstIteratorType;

public:
	RevolvingCollection() :
		m_index(0),
		m_first(true)
	{
	}

	RevolvingCollection(const RevolvingCollection& source) = default;
	RevolvingCollection(RevolvingCollection&& source) = default;

public:
	template<typename Self>
	static [[nodiscard]] decltype(auto) get_next(Self& self)
	{
		if (self.m_items.size() == 0)
		{
			throw std::logic_error("self.m_items.size() == 0");
		}

		if (self.m_first)
		{
			self.m_first = false;
		}
		else
		{
			self.m_index += 1;
		}

		if (self.m_index > self.m_items.size() - 1)
		{
			self.m_index = 0;
		}

		return self.m_items[self.m_index];
	}

	void reset() const
	{
		if (m_index > 2)
		{
			int bp = 0;
			bp = bp;
		}
		m_first = true;
		m_index = 0;
	}

	[[nodiscard]] decltype(auto) next()
	{
		return get_next(*this);
	}

	[[nodiscard]] decltype(auto) next() const
	{
		return get_next(*this);
	}

	[[nodiscard]] ConstIteratorType cbegin() const noexcept
	{
		return m_items.cbegin();
	}

	[[nodiscard]] ConstIteratorType cend() const noexcept
	{
		return m_items.cend();
	}

	[[nodiscard]] IteratorType begin() noexcept
	{
		return m_items.begin();
	}

	[[nodiscard]] ConstIteratorType begin() const noexcept
	{
		return m_items.begin();
	}

	[[nodiscard]] typename std::vector<T>::iterator end() noexcept
	{
		return m_items.end();
	}

	[[nodiscard]] typename std::vector<T>::iterator end() const noexcept
	{
		return m_items.end();
	}

	[[nodiscard]] T& back() noexcept
	{
		return m_items.back();
	}

	[[nodiscard]] const T& back() const noexcept
	{
		return m_items.back();
	}

	void push_back(T&& value)
	{
		m_items.push_back(value);
	}

	void push_back(const T& value)
	{
		m_items.push_back(value);
	}

	[[nodiscard]] size_t size() const
	{
		return m_items.size();
	}

private:
	std::vector<T> m_items;
	mutable size_t m_index;
	mutable bool m_first;
};
