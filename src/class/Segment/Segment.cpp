#include "Segment.hpp"

Segment::Segment(): ErrorInfo(), _type(NONE), _content(), _raw_content()
{
}

Segment::~Segment()
{
}

Segment::Segment(const Segment &copy): ErrorInfo(copy), _type(copy._type), _content(copy._content), _raw_content(copy._raw_content)
{
}

Segment	&Segment::operator=(const Segment &other)
{
	if (this != &other)
	{
		_content = other._content;
		_raw_content = other._raw_content;
		_type = other._type;
		ErrorInfo::operator=(other);
	}
	return *this;
}

Segment	&Segment::operator+=(const char c)
{
	_content += c;
	return *this;
}

Segment	&Segment::operator<<(const char c)
{
	_raw_content += c;
	return *this;
}

void	Segment::clear()
{
	_type = NONE;
	_content.clear();
	_raw_content.clear();
	ErrorInfo::clear();
}

const std::string	Segment::getTypeString() const
{
	switch (_type)
	{
		case NONE:
			return "NONE";
		case DEFAULT:
			return "DEFAULT";
		case DEFAULT_ESCAPED:
			return "DEFAULT_ESCAPED";
		case SQUOTE:
			return "SQUOTE";
		case SQUOTE_ESCAPED:
			return "SQUOTE_ESCAPED";
		case DQUOTE:
			return "DQUOTE";
		case DQUOTE_ESCAPED:
			return "DQUOTE_ESCAPED";
		default:
			return "UNKNOWN";
	}
}

Segment::Type	Segment::getType() const
{
	return _type;
}

const std::string	&Segment::getContent() const
{
	return _content;
}

const std::string	&Segment::getRawContent() const
{
	return _raw_content;
}

void	Segment::setType(Type type)
{
	_type = type;
}

void	Segment::setContent(const std::string &content)
{
	_content = content;
}

void	Segment::setRawContent(const std::string &raw_content)
{
	_raw_content = raw_content;
}