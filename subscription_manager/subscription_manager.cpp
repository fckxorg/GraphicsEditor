#include "subscription_manager.hpp"

std::unordered_map<Window*, std::unordered_set<Window*>>
    SubscriptionManager::subscriptions;

void SubscriptionManager::add_subscription(Window* sender, Window* recipient) {
  assert(sender != nullptr);
  assert(recipient != nullptr);

  subscriptions[sender].insert(recipient);
}

void SubscriptionManager::unsubscribe(Window* sender, Window* recipient) {
  assert(sender != nullptr);
  assert(recipient != nullptr);

  subscriptions[sender].erase(recipient);
}

void SubscriptionManager::unsubscribe_all(Window* sender) {
  assert(sender != nullptr);

  subscriptions[sender].clear();
}

void SubscriptionManager::unsubscribe_from_all(Window* recipient) {
  assert(recipient != nullptr);

  for (auto& subscription : subscriptions) {
    if (subscription.second.contains(recipient)) {
      subscription.second.erase(recipient);
    }
  }
}

void SubscriptionManager::send_event(Window* sender, Event* event) {
  assert(sender != nullptr);
  assert(event != nullptr);

  for (auto& recipient : subscriptions[sender]) {
    recipient->handle_event(event);
  }

  delete event;
}
