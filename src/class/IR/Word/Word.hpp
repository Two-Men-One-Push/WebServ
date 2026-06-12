#pragma once

#include "Segment.hpp"
#include <vector>

class	Word: public ErrorInfo
{
	private:
		std::vector<Segment>	_segments;
	public:
		Word();
		~Word();
		Word(const Word &copy);
		Word	&operator=(const Word &other);
		Word	&operator+=(Segment &segment);

		void	clear();
		bool	empty() const;
		void	addSegment(Segment &segment);

		std::string					content() const;
		std::string					rawContent() const;
		const std::vector<Segment>	&segments() const;
};
