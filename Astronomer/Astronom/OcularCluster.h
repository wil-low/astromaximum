#pragma once
#include <list>
class AstroLabel;

class OcularCluster
{
public:
	OcularCluster();
	OcularCluster(AstroLabel* label);
	void insert (const OcularCluster& cluster);
	void append(const OcularCluster& oc);
	bool disperse();
	double getMin() const;
	double getMax() const;
	void sort();
	void print() const;
	static void setLabelWidth (double w);
	typedef std::list<OcularCluster> ClusterList;
	typedef ClusterList::iterator Iter;
	typedef ClusterList::const_iterator ConstIter;
	void push_front(const OcularCluster& oc);
	void push_back(const OcularCluster& oc);
private:
	ClusterList vec;
	bool canMerge (Iter it0, Iter it1) const;
	void print_indented(int indent) const;
	AstroLabel* label;
	double min_ang, max_ang;
	static double label_w_;
	static double min (double a1, double a2);
	static double max (double a1, double a2);
	Iter prev(Iter it);
	Iter next(Iter it);
};

