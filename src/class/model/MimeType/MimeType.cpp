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

const std::map<std::string, std::string>	&MimeType::getMimeTypes() const
{
	return (this->_mimeTypes);
}

void	MimeType::addMimeType(const std::string &extension, const std::string &mimeType)
{
	this->_mimeTypes[extension] = mimeType;
}