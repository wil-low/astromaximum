#include "AstroLabelContainer.h"

AstroLabel* AstroLabelContainer::find_by_chart_id (int chart, int id)
{
	alc_by_chart_id& ref = get<chart_id_tag>();
	alc_by_chart_id::iterator it = ref.find (boost::make_tuple(chart, id));
	return (it != ref.end()) ? *it : NULL;
}
