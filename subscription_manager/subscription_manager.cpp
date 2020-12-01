#include "subscription_manager.hpp"

std::stack<std::unordered_map<Window*, std::unordered_set<Window*>>>
    SubscriptionManager::subscriptions;

Window* SubscriptionManager::system_event_sender = nullptr;

void SubscriptionManager::add_subscription(Window* sender, Window* recipient) {
  subscriptions.top()[sender].insert(recipient);
}

void SubscriptionManager::unsubscribe(Window* sender, Window* recipient) {
  subscriptions.top()[sender].erase(recipient);
}

void SubscriptionManager::unsubscribe_all(Window* sender) {
  subscriptions.top()[sender].clear();
}

void SubscriptionManager::unsubscribe_from_all(Window* recipient) {
  for (auto& subscription : subscriptions.top()) {
    if (subscription.second.contains(recipient)) {
      subscription.second.erase(recipient);
    }
  }
}

void SubscriptionManager::send_event(Window* sender, Event* event) {
  for (auto& recipient : subscriptions.top()[sender]) {
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

void SubscriptionManager::init_new_layer() {
    subscriptions.push(std::unordered_map<Window*, std::unordered_set<Window*>>());
}

void SubscriptionManager::deinit_layer() {
    subscriptions.pop();
}
