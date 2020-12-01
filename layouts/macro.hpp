#define CREATE(WINDOW_NAME, WINDOW_TYPE, ...) \
  std::unique_ptr<Window> WINDOW_NAME(new WINDOW_TYPE(__VA_ARGS__))

#define ADOPT(PARENT, CHILD) (PARENT)->add_child_window((CHILD))
#define SUBS(SENDER, RECIPIENT) SUBSCRIBE((SENDER).get(), (RECIPIENT).get())


