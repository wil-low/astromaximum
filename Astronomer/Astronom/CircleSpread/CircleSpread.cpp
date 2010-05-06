#include "CircleSpread.h"
//#include <iostream>
#include <algorithm>
#include <fxdefs.h>

bool less_input (const SpreadValue& v1, const SpreadValue& v2);
double normalize(double val, double circle_limit);

bool less_input (const SpreadValue& v1, const SpreadValue& v2)
{
	return v1.val_ < v2.val_;
}

double normalize(double val, double circle_limit)
{
	while (val < 0)
		val += circle_limit;
	while (val >= circle_limit)
		val -= circle_limit;
	return val;
}

ClusterItem::ClusterItem (const SpreadValue& input)
{
	start_ = finish_ = start_orig_ = finish_orig_ = center_ = input.val_;
	width_ = 0;
	link_[0] = link_[1] = NULL;
	delta_list_.push_back(input.ptr_);
}

void ClusterItem::merge(ClusterItem* rhs)
{
	finish_orig_ = rhs->finish_orig_;
	for (Iter it = rhs->delta_list_.begin(); it != rhs->delta_list_.end(); ++it) {
		delta_list_.push_back(*it);
	}
	ClusterItem* over_next = rhs->next();
	next(over_next);
	over_next->prev(this);
}

void ClusterItem::sparce(double delta, double circle_limit)
{
	int start_quad = (int)(start_orig_ / (circle_limit / 4));
	int finish_quad = (int)(finish_orig_ / (circle_limit / 4));
	double center = (finish_orig_ + start_orig_);
	if (((start_quad == 0) && (finish_quad == 3)) || ((start_quad == 3) && (finish_quad == 0)))
		center -= circle_limit;
	size_t delta_size = delta_list_.size() - 1;
	center_ = center / 2;
	width_ = delta * delta_size / 2;
	start_ = center_ - width_;
	finish_ = start_ + delta * delta_size;
	start_ = normalize (start_, circle_limit);
	finish_ = normalize (finish_, circle_limit);
}

void ClusterItem::print() const
{
	FXTRACE((99, "start %.2f, finish %.2f, orig start %.2f, finish %.2f; ", start_, finish_, start_orig_, finish_orig_));
	for (ConstIter it = delta_list_.begin(); it != delta_list_.end(); ++it) {
		FXTRACE((99, "%d ", (int)(*it)));
	}
}


/////////////////////////
CircleSpread::CircleSpread(const std::vector<SpreadValue>& input)
: head_(NULL)
, tail_(NULL)
, size_(0)
{
	std::vector<SpreadValue> sorted(input);
	std::sort(sorted.begin(), sorted.end(), less_input);
	FXTRACE((99, "Sorted input:\n"));
	for (size_t i = 0; i < sorted.size(); ++i) {
		append(sorted[i]);
		FXTRACE((99, "  %d -> %.2f\n", (int)sorted[i].ptr_, sorted[i].val_));
	}
	FXTRACE((99, "\n"));
}

CircleSpread::~CircleSpread()
{
	ClusterItem* ptr = head_;
	for (size_t i = 0; i < size_; ++i) {
		ClusterItem* tmp = ptr;
		ptr = ptr->next();
		delete tmp;
	}
}

void CircleSpread::append (const SpreadValue& input)
{
	if (!head_) {
		head_ = tail_ = new ClusterItem(input);
		head_->prev(head_);
		head_->next(head_);
	}
	else {
		ClusterItem* item = new ClusterItem(input);
		item->prev(tail_);
		tail_->next(item);
		item->next(head_);
		head_->prev(item);
		tail_ = item;
	}
	++size_;
}

bool CircleSpread::spread(std::vector<SpreadValue>& output, double delta, double circle_limit)
{
	circle_limit_ = circle_limit;
	delta_ = delta;
	ClusterItem* cur = head_;
	int merge_count;
	do {
		merge_count = 0;
		bool is_merged = false;
		do {
			is_merged = false;
			ClusterItem* tmp = cur->next();
			if (tmp == cur)
				break;
			double cdelta = circular_delta(cur, tmp);
			if (cdelta < delta_) {
				cur->merge(cur->next());
				cur->sparce(delta_, circle_limit_);
				if (tmp == head_) {
					head_ = tmp->next();
					tail_ = cur;
				}
				delete tmp;
				is_merged = true;
				--size_;
				print();
				++merge_count;
			}
		} while (is_merged);
		FXTRACE((99, "----------\n"));
		cur = cur->next();
	} while (merge_count || cur != head_);

	ClusterItem* ptr = head_;
	for (size_t i = 0; i < size_; ++i) {
		ptr = ptr->next();
		double start = ptr->start_;
		for (ClusterItem::ConstIter it = ptr->delta_list_.begin(); it != ptr->delta_list_.end(); ++it) {
			output.push_back(SpreadValue(start, *it));
			start = normalize(start + delta_, circle_limit_);
		}
	}
	return true;
}

double CircleSpread::circular_delta (const ClusterItem* lhs, const ClusterItem* rhs) const
{
	double delta = normalize(rhs->center_ - lhs->center_, circle_limit_);
	delta -= (lhs->width_ + rhs->width_);
	return delta;// < 0 ? -delta : delta;
}

void CircleSpread::print() const
{
	FXTRACE((99, "CircleSpread size = %d\n", size_));
	ClusterItem* ptr = head_;
	for (size_t i = 0; i < size_; ++i) {
		ClusterItem* tmp = ptr;
		ptr = ptr->next();
		FXTRACE((99, "Item %d: ", i));
		tmp->print();
		FXTRACE((99, "\n"));
	}
	FXTRACE((99, "\n"));
}
