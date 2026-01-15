# M5Stack Pomodoro

A Pomodoro timer for the M5Stack Core2 using M5Unified. The app provides
focus/break cycles, a simple configuration screen, and a lightweight UI layer
with reusable components.

## Features
- Pomodoro focus/break timer with start/pause and reset.
- Orientation-based mode switch (work vs. break).
- On-device configuration for work duration.
- Battery indicator with 5 bars and charging bolt.

## Build and Upload
This project uses PlatformIO.

- Build: `pio run`
- Upload: `pio run -t upload`
- Monitor: `pio device monitor -b 115200`

## Project Structure
- `src/main.cpp`: Hardware init and app loop.
- `src/pomodoro/PomodoroApp.h` / `src/pomodoro/PomodoroApp.cpp`:
  Main app orchestration, state machine, input handling, and layout.
- `src/pomodoro/TimerFace.h` / `src/pomodoro/TimerFace.cpp`:
  Timer screen renderer (title, mode label, time).
- `src/pomodoro/ConfigFace.h` / `src/pomodoro/ConfigFace.cpp`:
  Configuration screen renderer (work/break durations + controls).
- `src/ui/Component.h`:
  Base UI interface with `update` and `render`.
- `src/ui/Button.h`:
  Simple touch button with label and callback.
- `src/ui/BatteryIndicator.h` / `src/ui/BatteryIndicator.cpp`:
  Battery icon renderer (5 bars + charging bolt).
- `src/core/Time.h`:
  Time constants used across the app.

## Module Notes
### PomodoroApp
- Owns the timer state (work/break), screen state (timer/config), and layout.
- Handles input, updates timer ticks, and requests redraws.
- Coordinates child components and buttons.

### TimerFace
- Pure renderer: draws title, mode label, and remaining time.
- Uses the shared layout scale to stay resolution-independent.

### ConfigFace
- Screen component: renders the configuration view and handles +/- controls.
- Updates work and break durations via callbacks.

### BatteryIndicator
- Draws a compact battery icon with five bars in the top-left corner.
- Shows a charging bolt when the device reports charging.

### UI Components
- `Component` defines the minimal interface for app UI pieces.
- `Button` is a clickable component with simple hit detection.

## Extending
- Add new UI components under `src/ui/` and keep them stateless when possible.
- Keep rendering logic inside components and state logic inside the app.
- Use `scaleFrom240` patterns to keep elements proportional.
