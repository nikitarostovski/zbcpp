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
    float capacity;
    
    CollectorConfig();
    CollectorConfig(CollectorCategory category, float capacity);
    
    bool operator==(const CollectorConfig& rhs) const;
};

std::string collectorTypeString(CollectorCategory collector);
