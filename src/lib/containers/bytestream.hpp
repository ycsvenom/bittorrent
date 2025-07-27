#pragma once
#ifndef BYTESTREAM_H
#define BYTESTREAM_H

#include <deque>
#include <stdint.h>
#include <string>
#include <vector>

class ByteStream
{
private:
	std::vector<uint8_t> m_buffer;

public:
	ByteStream() = default;
	ByteStream(const std::string &str);
	ByteStream(const std::vector<uint8_t> bytes, int count = -1);
	size_t size() const;
	uint8_t front() const;
	uint8_t back() const;
	bool empty() const;
	std::vector<uint8_t>::const_iterator begin() const;
	std::vector<uint8_t>::const_iterator end() const;
	void push_back(const std::string &value);
	void push_back(char value);
	void push_back(int value);
	void push_back(unsigned value);
	void push_back(uint8_t value);
	std::string pop_str(size_t length = -1);
	char pop_char();
	int pop_int();
	unsigned pop_uint();
	uint8_t pop_byte();
	void remove_prefix(size_t count);
	void remove_suffix(size_t count);
	bool operator==(const ByteStream &other) const;
	bool operator==(const std::vector<uint8_t> &other) const;
	bool operator!=(const ByteStream &other) const;
	bool operator!=(const std::vector<uint8_t> &other) const;
	ByteStream &operator<<(const std::string &value);
	ByteStream &operator<<(const char &value);
	ByteStream &operator<<(const int &value);
	ByteStream &operator<<(const unsigned &value);
	ByteStream &operator<<(const size_t &value);
	ByteStream &operator<<(const uint8_t &value);
	ByteStream &operator<<(const ByteStream &value);
	const uint8_t &operator[](size_t index) const;
	~ByteStream();
};

std::ostream &operator<<(std::ostream &out, const ByteStream &bstream);

#endif