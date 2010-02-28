#pragma once
#include <set>
class AstroLabel;

class OcularCluster
{
	struct less_deg {
		bool operator() (const OcularCluster& oc1, const OcularCluster& oc2) const
		{
			return oc1.min_ang < oc2.min_ang;
		}
	};
public:
	OcularCluster();
	OcularCluster(AstroLabel* label);
	void insert (AstroLabel* label);
	void insert (const OcularCluster& cluster);
	void append(const OcularCluster& oc);
	bool disperse(double dist);
	double getMin() const;
	double getMax() const;
	void print();
	typedef std::set<OcularCluster, less_deg> ClusterSet;
	typedef ClusterSet::iterator Iter;
private:
	ClusterSet vec;
	bool merge_if(OcularCluster::Iter& it0, OcularCluster::Iter& it1, double dist);
	AstroLabel* label;
	double min_ang, max_ang;
};

