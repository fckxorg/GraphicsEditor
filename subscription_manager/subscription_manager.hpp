#ifndef SUBSCRIPTION_MANAGER_HPP
#define SUBSCRIPTION_MANAGER_HPP

#include <cassert>
#include <cstdarg>
#include <unordered_map>
#include <unordered_set>
#include <stack>

#include "../window_base/window_base.hpp"

#define SUBSCRIBE(SENDER, RECIPIENT) \
  SubscriptionManager::add_subscription((SENDER), (RECIPIENT))

#define SEND(SENDER, EVENT) SubscriptionManager::send_event((SENDER), (EVENT))

class SubscriptionManager {
 private:
  static Window* system_event_sender;
  static std::stack<std::unordered_map<Window*, std::unordered_set<Window*>>>
      subscriptions;  // TODO custom hash for pointers

  // XXX std::function-like functors with arguments union

 public:
  SubscriptionManager() = delete;
  static void add_subscription(Window* sender, Window* recipient);
  static void unsubscribe(Window* sender, Window* recipient);
  static void unsubscribe_all(Window* sender);
  static void unsubscribe_from_all(Window* recipient);
  static void send_event(Window* sender, Event* event);
  static Window* get_system_event_sender();
  static void set_system_event_sender(Window* system_event_sender);
  static void init_new_layer();
  static void deinit_layer();
};

#endif
