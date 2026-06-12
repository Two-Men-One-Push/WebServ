#include "Word.hpp"

Word::Word(): ErrorInfo()
{
}

Word::~Word()
{
}

Word::Word(const Word &copy): ErrorInfo(copy), _segments(copy._segments)
{
}

Word	&Word::operator=(const Word &other)
{
	if (this != &other)
	{
		ErrorInfo::operator=(other);
		_segments = other._segments;
	}
	return (*this);
}

void	Word::clear()
{
	_segments.clear();
	ErrorInfo::clear();
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
		ErrorInfo::operator=(segment);
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