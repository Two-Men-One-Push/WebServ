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
		ErrorInfo::operator=(other);
		_type = other._type;
		_content = other._content;
		_raw_content = other._raw_content;
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
	ErrorInfo::clear();
	_type = NONE;
	_content.clear();
	_raw_content.clear();
}

std::string	Segment::typeString() const
{
	switch (_type)
	{
		case NONE:            return "NONE";
		case DEFAULT:         return "DEFAULT";
		case DEFAULT_ESCAPED: return "DEFAULT_ESCAPED";
		case SQUOTE:          return "SQUOTE";
		case SQUOTE_ESCAPED:  return "SQUOTE_ESCAPED";
		case DQUOTE:          return "DQUOTE";
		case DQUOTE_ESCAPED:  return "DQUOTE_ESCAPED";
		default:              return "UNKNOWN";
	}
}

Segment::Type	Segment::type() const
{
	return _type;
}

Segment::Type	&Segment::type()
{
	return _type;
}

const std::string	&Segment::content() const
{
	return _content;
}

const std::string	&Segment::rawContent() const
{
	return _raw_content;
}
