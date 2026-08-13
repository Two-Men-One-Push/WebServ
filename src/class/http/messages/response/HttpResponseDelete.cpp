#include "./HttpResponse.hpp"
#include "http/HttpStatus.hpp"
#include "http/errors/HttpStandardErrors.hpp"
#include "http/messages/HttpMessage.hpp"
#include <cerrno>
#include <cstdio>

void HttpResponse::deleteFile(const std::string &path) {
	if (std::remove(path.c_str())) {
		if (errno == ENOENT || errno == ENOTDIR) throw HttpErrors::NotFoundException();
		if (errno == EACCES || errno == EPERM || errno == EISDIR) throw HttpErrors::ForbiddenException();
		if (errno == ENAMETOOLONG) throw HttpErrors::URITooLongException();
		if (errno == EBUSY) throw HttpErrors::ConflictException();
		throw HttpErrors::InternalServerErrorException();
	}

	this->replaceBody(NULL);
	this->_bodyType = BT_NONE;
	this->_status = HttpStatus::NoContent;
	this->_message = HttpStatus::reasonPhrase(HttpStatus::NoContent);
	this->_contentLength = 0;
	this->_location.clear();
	this->_inState = RECV_COMPLETED;
}
