# mp3

## Rotary encoder preference

Wiring: CLK to D1 (GPIO5), DT to D2 (GPIO4), SW to D3 (GPIO0).
Use `REVERSE_DIRECTION = true`: the user requested the opposite volume
direction from the initial encoder test. Preserve this mapping in future
encoder and MP3 player code. The saved test is `rotary-encoder-testing.cpp`.
