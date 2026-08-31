#ifndef TMPFILE_HPP
#define TMPFILE_HPP

#include "File/File.hpp"
#include <exception>
#include <set>
#include <stdint.h>
#include <string>
#include <sys/types.h>
#include <unistd.h>

#define FILE_PREFIX ".webserv_tmp_"

class TmpFile : public File {
  private:
	TmpFile(const TmpFile &other);
	TmpFile &operator=(const TmpFile &other);

	static std::set<std::string> _activeFiles;

	const std::string _target;
	const std::string _tmpPath;
	bool _commited;

  protected:
	static int openTmpFileFd(const std::string &path, int flags, mode_t mode = 0);

  public:
	class ReservedNameException : public std::exception {
		const char *what() const throw() {
			return "File requested match reserved name";
		}
	};

	class BusyException : public std::exception {
		const char *what() const throw() {
			return "File requested is already busy";
		}
	};

	TmpFile(const std::string &path, int flags, mode_t mode = 0);
	~TmpFile();

	bool commit();

	const std::string &target() const { return _target; }

	static std::string format(const std::string &filePath);
	static bool matchFormat(const std::string &filePath);
	static bool isActive(const std::string &filePath);
};

#endif
