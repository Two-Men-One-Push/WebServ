#include "MimeType.hpp"

MimeType::MimeType(): _mimeTypes()
{
}

MimeType::~MimeType()
{
}

MimeType::MimeType(const MimeType &copy): _mimeTypes(copy._mimeTypes)
{
}

MimeType	&MimeType::operator=(const MimeType &other)
{
	if (this != &other)
	{
		this->_mimeTypes = other._mimeTypes;
	}
	return (*this);
}

const std::map<std::string, std::string>	&MimeType::mimetypes() const
{
	return (this->_mimeTypes);
}

std::map<std::string, std::string>	&MimeType::mimetypes()
{
	return (this->_mimeTypes);
}