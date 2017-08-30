#pragma once

#include <string>

class string_wrap
{
	std::string m_String;
	std::size_t m_Hash;

	string_wrap() = delete;
	string_wrap(const string_wrap &other) = default;
	string_wrap(string_wrap &&other) = default;
	string_wrap(const std::string &str) :
		m_String(str),
		m_Hash(std::hash<std::string>{}(str)) {}
	string_wrap(std::string &&str) :
		m_String(std::move(str)),
		m_Hash(std::hash<std::string>{}(str)) {}
	string_wrap(const std::string &str) :
		m_String(str),
		m_Hash(std::hash<std::string>{}(str)) {}
	string_wrap(std::string &&str) :
		m_String(std::move(str)),
		m_Hash(std::hash<std::string>{}(str)) {}
};

