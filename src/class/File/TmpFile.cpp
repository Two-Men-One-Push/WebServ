#include "./TmpFile.hpp"
#include "File/File.hpp"
#include "Logger/Logger.hpp"
#include "errors/WebservErrors.hpp"
#include <cstdio>
#include <string>
#include <sys/types.h>

TmpFile::TmpFile(const std::string &path, int flags, mode_t mode)
	: File(TmpFile::openTmpFileFd(path, flags, mode)), _target(path), _tmpPath(TmpFile::format(path)), _commited(false) {}

TmpFile::~TmpFile() {
	if (_commited) return;
	std::remove(this->_tmpPath.c_str());
	TmpFile::_activeFiles.erase(this->_target);
}

int TmpFile::openTmpFileFd(const std::string &path, int flags, mode_t mode) {
	if (TmpFile::matchFormat(path)) throw TmpFile::ReservedNameException();
	if (TmpFile::isActive(path)) throw TmpFile::BusyException();
	int fd = File::openFd(TmpFile::format(path), flags, mode);
	TmpFile::_activeFiles.insert(path);
	return fd;
}

void TmpFile::commit() {
	if (this->_commited) {
		Logger::warn() << "Multiple commission on same TmpFile instance" << std::endl;
		return;
	}
	if (std::rename(_tmpPath.c_str(), _target.c_str()) < 0) {
		throw WebservErrors::SysError("rename", errno, this->_target);
	}
	TmpFile::_activeFiles.erase(this->_target);
	this->_commited = true;
}

std::string TmpFile::format(const std::string &path) {
	size_t pos = path.find_last_of('/');
	std::string dirName;
	std::string fileName;

	if (pos == std::string::npos) {
		return TMP_FILE_PREFIX + path;
	} else {
		return path.substr(0, pos + 1) + TMP_FILE_PREFIX + path.substr(pos + 1);
	}
}

bool TmpFile::matchFormat(const std::string &path) {
	size_t pos = path.find_last_of('/');

	if (pos == std::string::npos) {
		return path.compare(0, sizeof(TMP_FILE_PREFIX) - 1, TMP_FILE_PREFIX) == 0;
	} else {
		return path.compare(pos + 1, sizeof(TMP_FILE_PREFIX) - 1, TMP_FILE_PREFIX) == 0;
	}
}

bool TmpFile::isActive(const std::string &filePath) {
	return TmpFile::_activeFiles.find(filePath) != TmpFile::_activeFiles.end();
}
