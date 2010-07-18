#pragma once
#include "AstroLabel.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/composite_key.hpp>

using namespace boost::multi_index;

struct chart_id_tag {};
struct chart_tag {};
struct type_tag {};
struct chart_type_tag {};

typedef boost::multi_index_container<
	AstroLabel*,
	indexed_by<
		hashed_unique<
			tag<chart_id_tag>,
			composite_key<
				AstroLabel*,
				const_mem_fun<AstroLabel, int, &AstroLabel::getChartId>,
				const_mem_fun<AstroLabel, int, &AstroLabel::getId>
			>
		>,
		hashed_non_unique<
			tag<chart_tag>,
			const_mem_fun<AstroLabel, int, &AstroLabel::getChartId>
		>,
		hashed_non_unique<
			tag<chart_type_tag>,
			composite_key<
				AstroLabel*,
				const_mem_fun<AstroLabel, int, &AstroLabel::getChartId>,
				const_mem_fun<AstroLabel, body_type_t, &AstroLabel::getType>
			>
		>
	>
> AstroLabelSet;

class AstroLabelContainer : public AstroLabelSet
{
public:
	AstroLabel* find_by_chart_id (int chart, int id);
};

typedef AstroLabelContainer::iterator AlcIter;
typedef AstroLabelContainer::index<chart_type_tag>::type alc_by_chart_type;
typedef AstroLabelContainer::index<chart_id_tag>::type alc_by_chart_id;
