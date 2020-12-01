#include "subscription_manager.hpp"

std::unordered_map<Window*, std::unordered_set<Window*>>
    SubscriptionManager::subscriptions;

Window* SubscriptionManager::system_event_sender = nullptr;

void SubscriptionManager::add_subscription(Window* sender, Window* recipient) {
  subscriptions[sender].insert(recipient);
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
  for (auto& recipient : subscriptions[sender]) {
    recipient->handle_event(event);
  }

  delete event;
}

Window* SubscriptionManager::get_system_event_sender() {
  return system_event_sender;
}

void SubscriptionManager::set_system_event_sender(Window* system_event_sender) {
  SubscriptionManager::system_event_sender = system_event_sender;
}
