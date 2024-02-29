#include "collector_config.hpp"

std::string collectorTypeString(CollectorCategory collector)
{
    switch (collector) {
        case CollectorNone:
            return "None";
        case CollectorBasic:
            return "Basic";
        case CollectorAdvanced:
            return "Advanced";
    }
}

CollectorConfig::CollectorConfig()
    : CollectorConfig(CollectorNone, 0.0f)
{ }

CollectorConfig::CollectorConfig(CollectorCategory category, float orbCollectionRadius)
    : category(category)
    , orbCollectionRadius(orbCollectionRadius)
{ }

bool CollectorConfig::operator==(const CollectorConfig& rhs) const
{
    return category == rhs.category && orbCollectionRadius == rhs.orbCollectionRadius;
}
