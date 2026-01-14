#ifndef CORE_TIME_H
#define CORE_TIME_H

namespace app_time {
  #ifdef SECOND
  #undef SECOND
  #undef MINUTE
  #endif

  static constexpr int SECOND = 1;
  static constexpr int MINUTE = 60 * SECOND;
}

#endif // CORE_TIME_H
