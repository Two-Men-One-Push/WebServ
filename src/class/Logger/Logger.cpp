#include "./Logger.hpp"

Logger::LogLevel Logger::_level = Logger::LOG_INFO;

std::string Logger::levelStr(LogLevel lvl) {
	switch (lvl) {
		case Logger::LOG_ERROR: return "ERROR";
		case Logger::LOG_WARN: return "WARN";
		case Logger::LOG_INFO: return "INFO";
		case Logger::LOG_DEBUG: return "DEBUG";
	}
}
