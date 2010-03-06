#include "OcularCluster.h"
#include "AstroLabel.h"
#include <algorithm>

double OcularCluster::label_w_ = 0;

void OcularCluster::setLabelWidth (double w)
{
	label_w_ = w;
}

OcularCluster::OcularCluster()
: label(NULL)
, min_ang(0)
, max_ang(0)
{
}

OcularCluster::OcularCluster(AstroLabel* label)
: label(label)
{
	min_ang = label->getVisibleAngle();// - label_w_;
	max_ang = label->getVisibleAngle();// + label_w_;
}

void OcularCluster::insert (const OcularCluster& cluster)
{
	vec.push_back(cluster);
	min_ang = min(min_ang, cluster.min_ang);
	max_ang = max(max_ang, cluster.max_ang);
}

double OcularCluster::min (double a1, double a2)
{
	return FXMIN(a1, a2);
}

double OcularCluster::max (double a1, double a2)
{
	return FXMAX(a1, a2);
}

bool OcularCluster::disperse()
{
	bool changed = false;
	ClusterList new_vec;
	ClusterList::iterator it = vec.begin();
	ClusterList::iterator it1 = it;
	OcularCluster tmp;
	bool merged = false;
	Iter cur_it = it1;
	cur_it = prev(cur_it);
	do {
		if (merged = canMerge (cur_it, it1)) {
			changed = true;
			(*it1).push_front(*cur_it);
			Iter tmp = cur_it;
			cur_it = prev(cur_it);
			vec.erase(tmp);
		}
		else {
			cur_it = prev(cur_it);
		}
	} while(merged);
	print();
	merged = false;
	cur_it = it1;
	cur_it = next(cur_it);
	do {
		if (merged = canMerge (it1, cur_it)) {
			changed = true;
			(*it1).push_back(*cur_it);
			Iter tmp = cur_it;
			cur_it = next(cur_it);
			vec.erase(tmp);
		}
		else {
			cur_it = next(cur_it);
		}
	} while(merged);
	return true;
}

bool OcularCluster::canMerge (OcularCluster::Iter it0, OcularCluster::Iter it1) const
{
	if (it0 == it1)
		return false;
	double delta = fabs ((*it1).min_ang - (*it0).max_ang);
	if (delta > 180)
		delta = 360 - delta;
	return  delta < label_w_ * 2;
}

OcularCluster::Iter OcularCluster::prev(Iter it)
{
	Iter prev_it = it;
	if (it == vec.begin())
		prev_it = vec.end();
	return --prev_it;
}

OcularCluster::Iter OcularCluster::next(Iter it)
{
	Iter next_it = it;
	++it;
	if (next_it == vec.end())
		next_it = vec.begin();
	return next_it;
}

void OcularCluster::push_back(const OcularCluster& oc)
{
	if (label != NULL) {
		label = NULL;
		push_back(OcularCluster(label));
	}
	if (oc.label != NULL) {
		vec.push_back(oc);
		max_ang = oc.max_ang;
/*
		double center = (min_ang + max_ang) / 2;
		min_ang = center - delta * vec.size() / 2;
		for (int i = 0; i < vec.end(); ++i) {
			vec[i]->label->setVisibleAngle*/
	}
	else {
		for (ClusterList::const_iterator it = oc.vec.begin(); it != oc.vec.end(); ++it) {
			push_back(*it);
		}
	}
}

void OcularCluster::push_front(const OcularCluster& oc)
{
	if (label != NULL) {
		label = NULL;
		push_front(OcularCluster(label));
	}
	if (oc.label != NULL) {
		push_front(OcularCluster(oc.label));
		min_ang = oc.min_ang;
	}
	else {
		for (ClusterList::const_reverse_iterator it = oc.vec.rbegin(); it != oc.vec.rend(); ++it) {
			push_front(*it);
		}
	}
}

double OcularCluster::getMin() const
{
	return min_ang;
}

double OcularCluster::getMax() const
{
	return max_ang;
}

void OcularCluster::print() const
{
	print_indented(0);
}

void OcularCluster::print_indented(int indent) const
{
	if (label)
		FXTRACE((10, "%*slabel %X; %f\n", indent * 2, "", label, getMin(), getMax()));
	else {
		FXTRACE((10, "%*sComposite; %f .. %f\n", indent * 2, "", getMin(), getMax()));
		for (OcularCluster::ConstIter it = vec.begin(); it != vec.end(); ++it) {
			(*it).print_indented (indent + 1);
		}
	}
}

bool less_deg (const OcularCluster& oc1, const OcularCluster& oc2)
{
    return oc1.getMin() < oc2.getMin();
}

void OcularCluster::sort()
{
    vec.sort(less_deg);
	int vec_size = vec.size();
	double min_dist = 10000;
	Iter zero;
	for (Iter it1 = vec.begin(); it1 != vec.end(); ++it1) {
		Iter it2 = it1;
		++it2;
		if (it2 == vec.end())
			it2 = vec.begin();
		double dist = (*it2).getMin() - (*it1).getMax();
		if (dist < 0)
			dist += 360;
		if (dist < min_dist){
			zero=it1;
			min_dist = dist;
		}
	}
	ClusterList new_vec;
	new_vec.assign(zero, vec.end());
	new_vec.insert(new_vec.end(), vec.begin(), zero);
	vec.swap(new_vec);
}
