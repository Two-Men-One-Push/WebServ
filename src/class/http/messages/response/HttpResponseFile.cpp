#include "./HttpResponse.hpp"
#include "errors/WebservErrors.hpp"
#include "http/HttpStatus.hpp"
#include "http/errors/HttpStandardErrors.hpp"
#include "http/messages/Body/BodyFile.hpp"
#include "http/messages/HttpMessage.hpp"
#include <cerrno>
#include <sys/stat.h>

void HttpResponse::file(const std::string &path, HttpStatus::Code status, const std::string &mimeType) {
	struct stat stat_buf;

	int result = ::stat(path.c_str(), &stat_buf);
	if (result < 0) {
		if (errno == ENOENT || errno == ENOTDIR) throw HttpErrors::NotFoundException();
		if (errno == EACCES) throw HttpErrors::ForbiddenException();
		if (errno == ENAMETOOLONG) throw HttpErrors::URITooLongException();
		throw HttpErrors::InternalServerErrorException();
	}

	BodyFile *file;

	try {
		file = new BodyFile(path, O_RDONLY);
	} catch (const WebservErrors::SysError &e) {
		if (e.err() == ENOENT || e.err() == ENOTDIR) throw HttpErrors::NotFoundException();
		if (e.err() == EACCES) throw HttpErrors::ForbiddenException();
		if (e.err() == ENAMETOOLONG) throw HttpErrors::URITooLongException();
		throw HttpErrors::InternalServerErrorException();
	}
	this->replaceBody(file);

	this->_contentLength = stat_buf.st_size;
	this->_status = status;
	this->_message = HttpStatus::reasonPhrase(status);
	this->_mimeType = mimeType;
	this->_inState = RECV_COMPLETED;
}
