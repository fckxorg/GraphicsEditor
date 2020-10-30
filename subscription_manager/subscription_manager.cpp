#include "subscription_manager.hpp"

#include <cstdio>

std::unordered_map<Window*, std::unordered_set<Window*>>
    SubscriptionManager::subscriptions;

void SubscriptionManager::add_subscription(Window* sender, Window* recipient) {
    subscriptions[sender].insert(recipient);
    fprintf(stderr, "Subscribed %p to %p\n", recipient, sender);
}

void SubscriptionManager::unsubscribe(Window* sender, Window* recipient) {
    subscriptions[sender].erase(recipient);
}

void SubscriptionManager::unsubscribe_all(Window* sender) {
    subscriptions[sender].clear();
}

void SubscriptionManager::unsubscribe_from_all(Window* recipient) {
    for (auto& subscription : subscriptions) {
        if (subscription.second.contains(recipient)) {
            subscription.second.erase(recipient);
        }
    }
}

void SubscriptionManager::send_event(Window* sender, Event* event) {
    printf("Sending events to recipients from %p\n", sender);
    for (auto& recipient : subscriptions[sender]) {
        recipient->handle_event(event);
    }

    delete event;
}