#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

class Logger {
  public:
	enum LogLevel {
		LOG_SILENT,
		LOG_ERROR,
		LOG_WARN,
		LOG_INFO,
		LOG_DEBUG
	};

	class LogStream {
	  private:
		LogLevel _lvl;
		bool _active;
		std::ostringstream _oss;

		static std::string colorLevelStr(LogLevel lvl);
		static std::string timestamp();

	  public:
		LogStream(Logger::LogLevel lvl, LogLevel currentLevel);
		LogStream(const LogStream &other);

		~LogStream();

		template <typename T>
		LogStream &operator<<(const T &value) {
			if (this->_lvl == LOG_DEBUG) {
#ifdef NDEBUG
				if (_active)
					_oss << value;
#endif
				return *this;
			} else {
				if (_active)
					_oss << value;
				return *this;
			}
		}

		LogStream &operator<<(std::ostream &(*manip)(std::ostream &)) {
			if (this->_lvl == LOG_DEBUG) {
#ifdef NDEBUG
				if (_active)
					manip(_oss);
#endif
				return *this;
			} else {
				if (_active)
					manip(_oss);
				return *this;
			}
		}

		LogStream &write(const char *s, std::streamsize n) {
			if (this->_lvl == LOG_DEBUG) {
#ifdef NDEBUG
				if (_active)
					_oss.write(s, n);
#endif
				return *this;
			} else {
				if (_active)
					_oss.write(s, n);
				return *this;
			}
		}
	};

  private:
	static LogLevel _level;

  public:
	static void level(LogLevel lvl) { _level = lvl; }

	static LogLevel level() { return _level; };

	static std::string levelStr(LogLevel lvl);
	static std::string levelStr();

	static LogStream error() { return LogStream(LOG_ERROR, _level); }

	static LogStream warn() { return LogStream(LOG_WARN, _level); }

	static LogStream info() { return LogStream(LOG_INFO, _level); }

	static LogStream debug() { return LogStream(LOG_DEBUG, _level); }
};

#endif
