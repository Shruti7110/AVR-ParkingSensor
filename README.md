# AVR-ParkingSensor
A bare-metal AVR project that measures distance using an HC-SR04 ultrasonic sensor and provides real-time feedback via LEDs, a passive buzzer, and UART. Built entirely in C without any Arduino framework, demonstrating Timer CTC, Timer PWM, external interrupts, and ISR edge switching on the ATmega328P.

---

## Features
- Distance measurement using HC-SR04 ultrasonic sensor
- External interrupt (INT0) for precise echo pulse timing
- Timer1 CTC mode — triggers measurement every 60ms automatically
- Timer0 Fast PWM — controls passive buzzer pitch based on distance
- Three LED indicators (green/yellow/red) for visual distance feedback
- UART output showing live distance in cm at 9600 baud

---

## Hardware

| Component | Pin |
|-----------|-----|
| HC-SR04 Trigger | PB0 (Pin 8) |
| HC-SR04 Echo | PD2 (Pin 2) — INT0 |
| Passive Buzzer | PD5 (Pin 5) — OC0B PWM |
| Green LED (far) | PB1 (Pin 9) |
| Yellow LED (medium) | PB2 (Pin 10) |
| Red LED (close) | PB3 (Pin 11) |

---

## Project Structure

```
AVR-ParkingSensor/
├── main.c
├── timer.c / timer.h
├── hcsr04.c / hcsr04.h
├── uart.c / uart.h
└── README.md
```

---

## How It Works

### Distance Measurement
```
Timer1 fires every 60ms → sets trigger_flag
    ↓
main() sends 10µs pulse on PB0
    ↓
HC-SR04 emits ultrasonic burst
    ↓
Echo returns → INT0 interrupt fires on RISING edge → save TCNT1
    ↓
Echo ends → INT0 switches to FALLING edge → save TCNT1
    ↓
distance = (echo_end - echo_start) / 58
```

### Distance Formula
```
Timer1 prescaler = 8
Each tick = 0.5µs
Sound travels 1cm in 58µs (round trip)
distance_cm = (echo_end - echo_start) / 58
```

---

## Feedback Logic

| Distance | LED | Buzzer |
|----------|-----|--------|
| > 50cm | Green (PB1) | Silent (OCR0B = 0) |
| 20–50cm | Yellow (PB2) | Medium (OCR0B = 64) |
| < 20cm | Red (PB3) | Loud (OCR0B = 128) |

---

## Interrupts Used

| Interrupt | Purpose |
|-----------|---------|
| `TIMER1_COMPA_vect` | Fires every 60ms, sets trigger_flag |
| `INT0_vect` | Detects echo rising/falling edge, records TCNT1 |

### ISR Edge Switching
INT0 starts configured for rising edge. When echo rises, ISR saves start time and switches INT0 to falling edge. When echo falls, ISR saves end time. This allows a single interrupt pin to capture both edges of the echo pulse.

---

## Timers

### Timer1 — CTC Mode (Measurement Interval)
```
Prescaler: 8
OCR1A: calculated for 60ms interval
Interrupt: TIMER1_COMPA — sets trigger_flag every 60ms
```

### Timer0 — Fast PWM (Buzzer)
```
Prescaler: 8
Mode: Fast PWM, Phase Correct
Output: OC0B (PD5)
OCR0B: set by main() based on distance
```

---

## UART Output (9600 baud)
```
Remaining distance = 72 cm
Remaining distance = 45 cm
Remaining distance = 18 cm
Remaining distance = 12 cm
```

---

## Build & Upload

### Tools
- Atmel Studio 7
- AVR-GCC
- avrdude 6.3
- USBasp programmer

### Upload via USBasp
```
-C"...avrdude.conf" -v -patmega328p -cusbasp -Pusb "-Uflash:w:$(ProjectDir)Debug\$(TargetName).hex:i"
```

---

## Concepts Covered
| Concept | Implementation |
|---------|---------------|
| GPIO output | LED pins, trigger pin |
| GPIO input | Echo pin |
| Timer CTC mode | Timer1 — 60ms trigger interval |
| Timer PWM mode | Timer0 — buzzer pitch control |
| External interrupt | INT0 — echo pulse detection |
| ISR edge switching | Rising → falling inside INT0_vect |
| Volatile variables | Shared safely between ISR and main |
| Global interrupt enable | `sei()` |

---

## License
MIT
