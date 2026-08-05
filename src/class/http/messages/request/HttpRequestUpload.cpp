#include "./HttpRequest.hpp"
#include "File/TmpFile.hpp"
#include "Logger/Logger.hpp"
#include "errors/WebservErrors.hpp"
#include "http/errors/HttpStandardErrors.hpp"
#include "http/messages/Body/BodyTmpFile.hpp"
#include <cerrno>
#include <fcntl.h>
#include <string>

void HttpRequest::upload(const std::string &path) {
	try {
		BodyTmpFile *tmpFile = new BodyTmpFile(path, O_WRONLY | O_CREAT | O_TRUNC, 0600);
		this->replaceBody(tmpFile);
	} catch (const TmpFile::BusyException &) {
		throw HttpErrors::ConflictException();
	} catch (const TmpFile::ReservedNameException &) {
		throw HttpErrors::ForbiddenException();
	} catch (const WebservErrors::SysError &e) {
		if (e.err() == ENAMETOOLONG) throw HttpErrors::URITooLongException();
		throw HttpErrors::InternalServerErrorException();
	}
}

bool HttpRequest::commitBody() {
	BodyTmpFile *tmpFile = dynamic_cast<BodyTmpFile*>(this->_body);
	if (!tmpFile) {
		Logger::error() << "Invalid cast concrete class" << std::endl;
		throw HttpErrors::InternalServerErrorException();
	}
	try {
		return tmpFile->commit();
	} catch (const WebservErrors::SysError &e) {
		throw HttpErrors::InternalServerErrorException();
	}
}
