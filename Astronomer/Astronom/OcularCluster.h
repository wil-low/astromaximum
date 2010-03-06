#pragma once
#include <list>
class AstroLabel;

class OcularCluster
{
public:
	OcularCluster();
	OcularCluster(AstroLabel* label);
	void insert (AstroLabel* label);
	void insert (const OcularCluster& cluster);
	void append(const OcularCluster& oc);
	bool disperse(double dist);
	double getMin() const;
	double getMax() const;
	void sort();
	void print();
	typedef std::list<OcularCluster> ClusterList;
	typedef ClusterList::iterator Iter;
private:
	ClusterList vec;
	bool merge_if(OcularCluster::Iter& it0, OcularCluster::Iter& it1, double dist);
	AstroLabel* label;
	double min_ang, max_ang;
};

