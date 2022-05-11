# QUIC

A multiplexed transport over UDP

## Definition

QUIC is a new multiplexed transport built on top of UDP. HTTP/3 is designed to take advantage of QUIC's features, including lack of Head-Of-Line blocking between streams.

## Key Features

- Reduced connection establishment time - 0 round trips in the common case
- Improved congestion control feedback
- Multiplexing without head of line blocking
- Connection migration
- Transport extensibility
- Optional unreliable delivery
