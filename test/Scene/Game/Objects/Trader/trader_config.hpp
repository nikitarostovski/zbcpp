#pragma once
#include <vector>
#include "trading_item.hpp"

struct TraderConfig {
    int materialAmount;
    std::vector<TradingItem> itemsToSell;
    
};
