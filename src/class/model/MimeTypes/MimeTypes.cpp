#include "MimeTypes.hpp"

MimeTypes::MimeTypes(): _types()
{
}

MimeTypes::~MimeTypes()
{
}

MimeTypes::MimeTypes(const MimeTypes &copy): _types(copy._types)
{
}

MimeTypes	&MimeTypes::operator=(const MimeTypes &other)
{
	if (this != &other)
	{
		this->_types = other._types;
	}
	return (*this);
}

const std::map<std::string, std::string>	&MimeTypes::types() const
{
	return (this->_types);
}

std::map<std::string, std::string>	&MimeTypes::types()
{
	return (this->_types);
}