#ifndef CIRCLESPREAD_H
#define CIRCLESPREAD_H

#include <vector>
#include <list>

struct SpreadValue
{
	double val_;
	void* ptr_;
	SpreadValue (double val, void* ptr) : val_(val), ptr_(ptr) {}
};

class ClusterItem
{
public:
	ClusterItem (const SpreadValue& input);
	void merge(ClusterItem* rhs);
	void sparce(double delta, double circle_limit);
	void print() const;
    double center_;
    double width_;
    double start_;
    double finish_;
	double start_orig_;
	double finish_orig_;
    ClusterItem* link_[2];
    ClusterItem* next() {return link_[1];}
    ClusterItem* prev() {return link_[0];}
    void next(ClusterItem* item) {link_[1] = item;}
    void prev(ClusterItem* item) {link_[0] = item;}
	std::list<void*> delta_list_;
	typedef std::list<void*>::iterator Iter;
	typedef std::list<void*>::const_iterator ConstIter;
};

class CircleSpread
{
public:
    CircleSpread(const std::vector<SpreadValue>& input);
    virtual ~CircleSpread();
	void append (const SpreadValue& input);
	void print() const;
	bool spread(std::vector<SpreadValue>& output, double delta, double circle_limit);
	double circular_delta (const ClusterItem* lhs, const ClusterItem* rhs) const;
protected:
private:
    ClusterItem* head_;
    ClusterItem* tail_;
    size_t size_;
	double circle_limit_;
	double delta_;
};

#endif // CIRCLESPREAD_H
