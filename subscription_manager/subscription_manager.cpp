#include "subscription_manager.hpp"

std::unordered_map<Window*, std::unordered_set<Window*>>
    SubscriptionManager::subscriptions;

void SubscriptionManager::add_subscription(Window* sender, ...) {
    va_list recipients;
    Window* cur_recipient = nullptr;

    va_start(recipients, sender);

    cur_recipient = va_arg(recipients, Window*);

    while (cur_recipient != NULL) {
        subscriptions[sender].insert(cur_recipient);
        cur_recipient = va_arg(recipients, Window*);
    }
}

void SubscriptionManager::unsubscribe(Window* sender, Window* recipient) {
    subscriptions[sender].erase(recipient);
}

void SubscriptionManager::unsubscribe_all(Window* sender) {
    subscriptions[sender].clear();
}

void SubscriptionManager::unsubscribe_from_all(Window *recipient) {
    for(auto& subscription : subscriptions) {
        if(subscription.second.contains(recipient)) {
            subscription.second.erase(recipient);
        }
    }
}
