#pragma once

#include "Segment.hpp"
#include <vector>

class	Word
{
	private:
		std::vector<Segment>	_segments;
		std::string				_filename;
		size_t					_line_number;
		size_t					_column_number;
	public:
		Word();
		~Word();
		Word(const Word &copy);
		Word	&operator=(const Word &other);
		Word	&operator+=(Segment &segment);

		void	clear();
		bool	empty() const;
		void	addSegment(Segment &segment);

		std::string					getContent() const;
		std::string					getRawContent() const;
		const std::vector<Segment>	&getSegments() const;
		const std::string			&getFilename() const;
		size_t						getLineNumber() const;
		size_t						getColumnNumber() const;
};