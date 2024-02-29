#pragma once
#include <string>

enum CollectorCategory
{
    CollectorNone,
    CollectorBasic,
    CollectorAdvanced
};

struct CollectorConfig
{
    CollectorCategory category;
    float orbCollectionRadius;
    
    CollectorConfig();
    CollectorConfig(CollectorCategory category, float orbCollectionRadius);
    
    bool operator==(const CollectorConfig& rhs) const;
};

std::string collectorTypeString(CollectorCategory collector);
