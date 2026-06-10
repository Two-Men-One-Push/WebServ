#include "Segment.hpp"

Segment::Segment(): _type(NONE), _content(), _raw_content(), _filename(), _line_number(0), _column_number(0)
{
}

Segment::~Segment()
{
}

Segment::Segment(const Segment &copy): _type(copy._type), _content(copy._content), _raw_content(copy._raw_content), _filename(copy._filename), _line_number(copy._line_number), _column_number(copy._column_number)
{
}

Segment	&Segment::operator=(const Segment &other)
{
	if (this != &other)
	{
		_content = other._content;
		_raw_content = other._raw_content;
		_type = other._type;
		_filename = other._filename;
		_line_number = other._line_number;
		_column_number = other._column_number;
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
	_filename.clear();
	_line_number = 0;
	_column_number = 0;
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

const std::string	&Segment::getFilename() const
{
	return _filename;
}

size_t	Segment::getLineNumber() const
{
	return _line_number;
}

size_t	Segment::getColumnNumber() const
{
	return _column_number;
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

void	Segment::setLineNumber(size_t line_number)
{
	_line_number = line_number;
}

void	Segment::setColumnNumber(size_t column_number)
{
	_column_number = column_number;
}

void	Segment::setFilename(const std::string &filename)
{
	_filename = filename;
}