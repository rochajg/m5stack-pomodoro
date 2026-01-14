#ifndef UI_APP_H
#define UI_APP_H

#include "Component.h"

namespace ui {

class App : public Component {
 public:
  void requestRender() { needs_render_ = true; }

  bool consumeRender() {
    if (!needs_render_) {
      return false;
    }
    needs_render_ = false;
    return true;
  }

 protected:
  bool needs_render_ = true;
};

}  // namespace ui

#endif  // UI_APP_H
