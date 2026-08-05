#include "./HttpResponse.hpp"
#include "errors/WebservErrors.hpp"
#include "http/HttpStatus.hpp"
#include "http/errors/HttpStandardErrors.hpp"
#include "http/messages/Body/BodyStringStream.hpp"
#include "generateAutoIndex.hpp"
#include <cerrno>
#include <iostream>

void HttpResponse::autoIndex(const std::string &root, const std::string &path, HttpStatus::Code status) {
	BodyStringStream *ssBody = new BodyStringStream();
	this->replaceBody(ssBody);

	try {
		printAutoIndex(*ssBody, root, path);
	} catch (const WebservErrors::SysError &e) {
		int err = e.err();
		if (err == ENOENT || err == ENOTDIR) throw HttpErrors::NotFoundException();
		if (err == EACCES || err == EPERM) throw HttpErrors::ForbiddenException();
		if (err == ENAMETOOLONG) throw HttpErrors::URITooLongException();
		throw HttpErrors::InternalServerErrorException();
	}

	this->_bodyType = BT_CONTENT_LENGTH;
	this->_status = status;
	this->_message = HttpStatus::reasonPhrase(status);
	this->_mimeType = "text/html";
	this->_contentLength = ssBody->str().size();
	this->_inState = RECV_COMPLETED;
}
