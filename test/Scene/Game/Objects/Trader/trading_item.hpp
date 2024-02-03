#pragma once

class PlayerConfig;
class ShipConfig;

class TradingItem {
public:
    int materialPrice;
    std::string itemTitle;
    std::string itemDescription;
    
    std::function<void(PlayerConfig *playerConfig, ShipConfig *shipConfig)> applyItem;
    
    TradingItem(int price, std::string itemTitle, std::string itemDescription, std::function<void(PlayerConfig *playerConfig, ShipConfig *shipConfig)> applyItem)
        : materialPrice(price)
        , itemTitle(itemTitle)
        , itemDescription(itemDescription)
        , applyItem(applyItem)
    { }
};
