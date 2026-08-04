#include "./Logger.hpp"
#include <ctime>

Logger::LogStream::LogStream(Logger::LogLevel lvl, LogLevel currentLevel) : _lvl(lvl), _active(lvl <= currentLevel), _oss() {}

Logger::LogStream::LogStream(const LogStream &other)
	: _lvl(other._lvl), _active(other._active), _oss() {}

Logger::LogStream::~LogStream() {
	if (_active) std::cerr << this->timestamp() << ": "<< this->colorLevelStr(this->_lvl) << ": " << _oss.str() << std::endl;
}

std::string Logger::LogStream::colorLevelStr(Logger::LogLevel lvl) {
	switch (lvl) {
	case Logger::LOG_ERROR: return "\e[1;31merror\e[0m";
	case Logger::LOG_WARN: return "\e[1;33mwarn\e[0m";
	case Logger::LOG_INFO: return "\e[1;34minfo\e[0m";
	case Logger::LOG_DEBUG: return "\e[1;35mdebug\e[0m";
	}
}

std::string Logger::LogStream::timestamp() {
	std::time_t now = std::time(NULL);
	std::tm* local = std::localtime(&now);

	char buf[32];
	std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", local);
	return std::string(buf);
}
