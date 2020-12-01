#include "subscription_manager.hpp"

#include <stdio.h>

std::stack<std::unordered_map<Window*, std::unordered_set<Window*>>>
    SubscriptionManager::subscriptions;

Window* SubscriptionManager::system_event_sender = nullptr;
bool SubscriptionManager::cleanup_needed = false;

void SubscriptionManager::add_subscription(Window* sender, Window* recipient) {
  assert(sender != nullptr);
  assert(recipient != nullptr);

  subscriptions.top()[sender].insert(recipient);
}

void SubscriptionManager::unsubscribe(Window* sender, Window* recipient) {
  assert(sender != nullptr);
  assert(recipient != nullptr);

  subscriptions.top()[sender].erase(recipient);
}

void SubscriptionManager::unsubscribe_all(Window* sender) {
  assert(sender != nullptr);

  subscriptions.top()[sender].clear();
}

void SubscriptionManager::unsubscribe_from_all(Window* recipient) {
  assert(recipient != nullptr);

  for (auto& subscription : subscriptions.top()) {
    if (subscription.second.contains(recipient)) {
      subscription.second.erase(recipient);
    }
  }
}

void SubscriptionManager::send_event(Window* sender, Event* event) {
  assert(sender != nullptr);
  assert(event != nullptr);

  for (auto& recipient : subscriptions.top()[sender]) {
    recipient->handle_event(event);
  }

  delete event;

  if (cleanup_needed) {
    deinit_layer();
    cleanup_needed = false;
  }
  printf("Stack size is %ld\n", subscriptions.size());
}

Window* SubscriptionManager::get_system_event_sender() {
  return system_event_sender;
}

void SubscriptionManager::set_system_event_sender(Window* system_event_sender) {
  assert(system_event_sender != nullptr);

  SubscriptionManager::system_event_sender = system_event_sender;
}

void SubscriptionManager::init_new_layer() {
  subscriptions.push(
      std::unordered_map<Window*, std::unordered_set<Window*>>());
}

void SubscriptionManager::deinit_layer() { subscriptions.pop(); }

void SubscriptionManager::cleanup() { cleanup_needed = true; }
