#ifndef UI_BATTERY_INDICATOR_H
#define UI_BATTERY_INDICATOR_H

#include <type_traits>
#include <utility>

#include "Component.h"

namespace ui {

class BatteryIndicator : public Component {
 public:
  BatteryIndicator() = default;

  void setLayout(int16_t origin_x, int16_t origin_y, int16_t size);
  void update(UiContext&) override {}
  void render(UiContext& ctx) override;

  static int16_t WidthFor(int16_t size);
  static int16_t HeightFor(int16_t size);
  static int16_t MarginFor(int16_t size);

 private:
  static int16_t scaleFrom240(int16_t size, int16_t value);
  static int16_t batteryWidth(int16_t size);
  static int16_t batteryHeight(int16_t size);
  static int16_t batteryMargin(int16_t size);

  template <typename T>
  class HasIsCharging {
   private:
    template <typename U>
    static auto test(int) -> decltype(std::declval<U>().isCharging(),
                                      std::true_type{});
    template <typename>
    static std::false_type test(...);

   public:
    static constexpr bool value = decltype(test<T>(0))::value;
  };

  template <typename T>
  class HasGetChargeState {
   private:
    template <typename U>
    static auto test(int) -> decltype(std::declval<U>().getChargeState(),
                                      std::true_type{});
    template <typename>
    static std::false_type test(...);

   public:
    static constexpr bool value = decltype(test<T>(0))::value;
  };

  template <typename T>
  static typename std::enable_if<HasIsCharging<T>::value, bool>::type
  chargingFromPower(T& power) {
    return power.isCharging();
  }

  template <typename T>
  static typename std::enable_if<!HasIsCharging<T>::value &&
                                      HasGetChargeState<T>::value,
                                  bool>::type
  chargingFromPower(T& power) {
    return power.getChargeState() != 0;
  }

  template <typename T>
  static typename std::enable_if<!HasIsCharging<T>::value &&
                                      !HasGetChargeState<T>::value,
                                  bool>::type
  chargingFromPower(T&) {
    return false;
  }

  static bool isCharging();

  int16_t origin_x_ = 0;
  int16_t origin_y_ = 0;
  int16_t size_ = 0;
};

}  // namespace ui

#endif  // UI_BATTERY_INDICATOR_H
