#include "OcularCluster.h"
#include "AstroLabel.h"

OcularCluster::OcularCluster()
: label(NULL)
{
}

OcularCluster::OcularCluster(AstroLabel* label)
: label(label)
{
	min_ang = max_ang = label->getVisibleAngle();
}

void OcularCluster::insert (AstroLabel* label)
{
	OcularCluster oc(label);
	insert(oc);
}

void OcularCluster::insert (const OcularCluster& cluster)
{
	vec.insert(cluster);
	min_ang = FXMIN(min_ang, cluster.min_ang);
	max_ang = FXMAX(max_ang, cluster.max_ang);
}

bool OcularCluster::disperse(double dist)
{
	bool changed = false;
	ClusterSet::iterator it = vec.begin();
	while (it != vec.end()) {
		ClusterSet::iterator it0 = it;
		(*it).print();
		ClusterSet::iterator it1 = it0;
		++it1;
		if (it1 == vec.end())
			it1 = vec.begin();
		if (merge_if (it0, it1, dist)) {
			changed = true;
			vec.erase(it1);
			break;
		}
		++it;
	}
	return changed;
}

bool OcularCluster::merge_if(OcularCluster::Iter& it0, OcularCluster::Iter& it1, double dist)
{
	if ((*it1).min_ang - (*it0).max_ang < dist) {
		(*it0).append(*it1);
		return true;
	}
	return false;
}

void OcularCluster::append(const OcularCluster& oc)
{
	if (label != NULL) {
		insert(label);
		label = NULL;
	}
	if (oc.label != NULL) {
		insert(oc.label);
	}
	else {
		for (ClusterSet::const_iterator it = oc.vec.begin(); it != oc.vec.end(); ++it) {
			insert(*it);
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

void OcularCluster::print()
{
	for (OcularCluster::Iter it = vec.begin(); it != vec.end(); ++it) {
		FXTRACE((10, "%s: %f - %f\n", __FUNCTION__, (*it).getMin(), (*it).getMax()));
	}
}
