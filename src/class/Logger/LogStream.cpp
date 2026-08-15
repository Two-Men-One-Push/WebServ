#include "./Logger.hpp"
#include <ctime>
#include <iostream>
#include <string>

Logger::LogStream::LogStream(Logger::LogLevel lvl, LogLevel currentLevel) : _lvl(lvl), _active(lvl <= currentLevel), _oss() {}

Logger::LogStream::LogStream(const LogStream &other)
	: _lvl(other._lvl), _active(other._active), _oss() {}

Logger::LogStream::~LogStream() {
	if (_lvl == LOG_DEBUG) {
#ifdef NDEBUG
		if (!_active) return;
		std::string timestamp = this->timestamp();
		std::string level = this->colorLevelStr(this->_lvl);
		std::cerr << timestamp << ": " + level + ": ";
		std::string buff = this->_oss.str();
		std::string prefix = std::string(timestamp.size() + 2, ' ') + level + ": ";
		for (std::string::const_iterator it = buff.begin(); it != buff.end(); ++it) {
			std::cerr << *it;
			if (*it == '\n' && (it + 1) != buff.end()) std::cerr << prefix;
		}
#endif
	} else {
		if (!_active) return;
		std::string timestamp = this->timestamp();
		std::string level = this->colorLevelStr(this->_lvl);
		std::cerr << timestamp << ": " + level + ": ";
		std::string buff = this->_oss.str();
		std::string prefix = std::string(timestamp.size() + 2, ' ') + level + ": ";
		for (std::string::const_iterator it = buff.begin(); it != buff.end(); ++it) {
			std::cerr << *it;
			if (*it == '\n' && (it + 1) != buff.end()) std::cerr << prefix;
		}
	}
}

std::string Logger::LogStream::colorLevelStr(Logger::LogLevel lvl) {
	switch (lvl) {
	case Logger::LOG_ERROR:
		return "\e[1;31merror\e[0m";
	case Logger::LOG_WARN:
		return "\e[1;33mwarn\e[0m";
	case Logger::LOG_INFO:
		return "\e[1;34minfo\e[0m";
	case Logger::LOG_DEBUG:
		return "\e[1;35mdebug\e[0m";
	default:
		return "";
	}
}

std::string Logger::LogStream::timestamp() {
	std::time_t now = std::time(NULL);
	std::tm *local = std::localtime(&now);

	char buf[32];
	std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", local);
	return std::string(buf);
}
