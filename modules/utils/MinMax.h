#ifndef MINMAX_H_
#define MINMAX_H_

struct MinMax
{
	double min;
	double max;

	bool operator<(const MinMax &other)
	{
		return min < other.max;
	}
	bool operator>(const MinMax &other)
	{
		return max > other.min;
	}
	bool inside(const MinMax &other)
	{
		return ((min < other.min && other.min < max) || (min < other.max && other.max < max));
	}
	bool bothInside(const MinMax &other)
	{
		return ((min < other.min && other.min < max) && (min < other.max && other.max < max));
	}
	bool minInside(const MinMax &other)
	{
		return (min < other.min && other.min < max);
	}
	bool maxInside(const MinMax &other)
	{
		return (min < other.max && other.max < max);
	}
	double minToMax()
	{
		return max-min;
	}
	bool intersection(MinMax &other)
	{
		return inside(other) || other.inside(*this);
	}
};

#endif
