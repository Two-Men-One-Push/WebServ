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
	return *this;
}

Word	&Word::operator+=(Segment &segment)
{
	addSegment(segment);
	return *this;
}

void	Word::clear()
{
	ErrorInfo::clear();
	_segments.clear();
}

bool	Word::empty() const
{
	return _segments.empty();
}

void	Word::addSegment(Segment &segment)
{
	if (segment.type() == Segment::NONE)
		return;
	if (_segments.empty())
		ErrorInfo::operator=(segment);
	_segments.push_back(segment);
	segment.clear();
}

std::string	Word::content() const
{
	std::string	str;

	for (size_t i = 0; i < _segments.size(); ++i)
		str += _segments[i].content();
	return str;
}

std::string	Word::rawContent() const
{
	std::string	str;

	for (size_t i = 0; i < _segments.size(); ++i)
		str += _segments[i].rawContent();
	return str;
}

const std::vector<Segment>	&Word::segments() const
{
	return _segments;
}
