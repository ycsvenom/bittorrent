#include <iterator>
#include <sstream>
#include <stack>
#include <stdexcept>

#include "bytestream.hpp"

#include <debug_macros.hpp>

ByteStream::ByteStream(const std::string &text)
{
	m_buffer = std::vector<uint8_t>(text.begin(), text.end());
}

ByteStream::ByteStream(const std::vector<uint8_t> bytes, int count)
{
	if (count < 0)
		count = bytes.size();
	m_buffer = std::vector<uint8_t>(bytes.cbegin(), bytes.cbegin() + count);
}

size_t ByteStream::size() const
{
	return m_buffer.size();
}

uint8_t ByteStream::front() const
{
	return m_buffer.front();
}

uint8_t ByteStream::back() const
{
	return m_buffer.back();
}

bool ByteStream::empty() const
{
	return m_buffer.empty();
}

std::vector<uint8_t>::const_iterator ByteStream::begin() const
{
	return m_buffer.begin();
}

std::vector<uint8_t>::const_iterator ByteStream::end() const
{
	return m_buffer.end();
}

void ByteStream::push_back(const std::string &value)
{
	for (auto &&c : value)
	{
		m_buffer.push_back(static_cast<uint8_t>(c));
	}
}

void ByteStream::push_back(char value)
{
	m_buffer.push_back(static_cast<uint8_t>(value));
}

void ByteStream::push_back(int value)
{
	std::stack<uint8_t> bytes;
	for (size_t i = 0; i < sizeof(int); i++)
	{
		bytes.push(value);
		value >>= 8;
	}
	while (!bytes.empty())
	{
		m_buffer.push_back(bytes.top());
		bytes.pop();
	}
}

void ByteStream::push_back(unsigned value)
{
	std::stack<uint8_t> bytes;
	for (size_t i = 0; i < sizeof(unsigned); i++)
	{
		bytes.push(value);
		value >>= 8;
	}
	while (!bytes.empty())
	{
		m_buffer.push_back(bytes.top());
		bytes.pop();
	}
}

void ByteStream::push_back(uint8_t value)
{
	m_buffer.push_back(value);
}

std::string ByteStream::pop_str(size_t length)
{
	if (length < 0)
		length = size();
	std::stringstream ss;
	size_t len = length > size() ? size() : length;
	for (int i = 0; i < len; i++)
		ss << m_buffer.at(i);
	remove_prefix(len);
	return ss.str();
}

char ByteStream::pop_char()
{
	auto value = static_cast<char>(front());
	remove_prefix(1);
	return value;
}

int ByteStream::pop_int()
{
	int value = 0;
	size_t len = sizeof(int) > size() ? size() : sizeof(int);
	for (size_t i = 0; i < len; i++)
	{
		value <<= 8;
		value |= front();
		remove_prefix(1);
	}
	return value;
}

unsigned ByteStream::pop_uint()
{
	size_t value = 0;
	size_t len = sizeof(unsigned) > size() ? size() : sizeof(unsigned);
	for (size_t i = 0; i < len; i++)
	{
		value <<= 8;
		value |= front();
		remove_prefix(1);
	}
	return value;
}

uint8_t ByteStream::pop_byte()
{
	auto value = front();
	remove_prefix(1);
	return value;
}

void ByteStream::remove_prefix(size_t count)
{
	if (count < 0)
		throw std::invalid_argument("can't remove negative number of elements!");
	else if (count >= size())
	{
		m_buffer.clear();
		return;
	}
	m_buffer.erase(m_buffer.begin(), m_buffer.begin() + count);
}

void ByteStream::remove_suffix(size_t count)
{
	if (count < 0)
		throw std::invalid_argument("can't remove negative number of elements!");
	else if (count >= size())
	{
		m_buffer.clear();
		return;
	}
	for (size_t i = 0; i < count; i++)
	{
		m_buffer.pop_back();
	}
}

bool ByteStream::operator==(const ByteStream &other) const
{
	return m_buffer == other.m_buffer;
}

bool ByteStream::operator==(const std::vector<uint8_t> &other) const
{
	if (m_buffer.size() != other.size())
		return false;
	for (size_t i = 0; i < m_buffer.size(); i++)
		if (m_buffer.at(i) != other[i])
			return false;
	return true;
}

bool ByteStream::operator!=(const ByteStream &other) const
{
	return m_buffer != other.m_buffer;
}

bool ByteStream::operator!=(const std::vector<uint8_t> &other) const
{
	if (m_buffer.size() != other.size())
		return true;
	for (size_t i = 0; i < m_buffer.size(); i++)
		if (m_buffer.at(i) != other[i])
			return true;
	return false;
}

const uint8_t &ByteStream::operator[](size_t index) const
{
	return m_buffer.at(index);
}

ByteStream::~ByteStream()
{
}

std::ostream &operator<<(std::ostream &out, const ByteStream &bstream)
{
	std::copy(bstream.begin(), bstream.end(), std::ostream_iterator<uint8_t>(out));
	return out;
}

ByteStream &ByteStream::operator<<(const std::string &value)
{
	push_back(value);
	return *this;
}

ByteStream &ByteStream::operator<<(const char &value)
{
	push_back(value);
	return *this;
}

ByteStream &ByteStream::operator<<(const int &value)
{
	push_back(value);
	return *this;
}

ByteStream &ByteStream::operator<<(const unsigned &value)
{
	push_back(value);
	return *this;
}

ByteStream &ByteStream::operator<<(const size_t &value)
{
	push_back((unsigned)value);
	return *this;
}

ByteStream &ByteStream::operator<<(const uint8_t &value)
{
	push_back(value);
	return *this;
}

ByteStream &ByteStream::operator<<(const ByteStream &stream)
{
	m_buffer.insert(m_buffer.end(), stream.m_buffer.begin(), stream.m_buffer.end());
	return *this;
}
