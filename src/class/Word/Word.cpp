#include "Word.hpp"

Word::Word()
{
}

Word::~Word()
{
}

Word::Word(const Word &copy): _segments(copy._segments)
{
}

Word	&Word::operator=(const Word &other)
{
	if (this != &other)
	{
		_segments = other._segments;
	}
	return (*this);
}

void	Word::clear()
{
	_segments.clear();
}

bool	Word::empty() const
{
	return _segments.empty();
}

void	Word::addSegment(Segment &segment)
{
	if (segment.getType() == Segment::NONE)
		return ;
	if (_segments.size() == 0)
	{
		_filename = segment.getFilename();
		_line_number = segment.getLineNumber();
		_column_number = segment.getColumnNumber();
	}
	_segments.push_back(segment);
	segment.clear();
}

Word	&Word::operator+=(Segment &segment)
{
	addSegment(segment);
	return (*this);
}

std::string	Word::getContent() const
{
	std::string	str;

	for (size_t i = 0; i < _segments.size(); ++i)
	{
		str += _segments[i].getContent();
	}
	return (str);
}

std::string	Word::getRawContent() const
{
	std::string	str;

	for (size_t i = 0; i < _segments.size(); ++i)
	{
		str += _segments[i].getRawContent();
	}
	return (str);
}

const std::vector<Segment>	&Word::getSegments() const
{
	return (_segments);
}

const std::string	&Word::getFilename() const
{
	if (_segments.size() > 0)
		return (_segments[0].getFilename());
	else
		return (_filename);
}

size_t	Word::getLineNumber() const
{
	if (_segments.size() > 0)
		return (_segments[0].getLineNumber());
	else
		return (_line_number);
}

size_t	Word::getColumnNumber() const
{
	if (_segments.size() > 0)
		return (_segments[0].getColumnNumber());
	else
		return (_column_number);
}