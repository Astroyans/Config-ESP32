# Roadmap

## Vision

Build a mesh of ESP32 nodes that share route data with each other and can also connect to a Raspberry Pi. Today, route strings typed over Serial and stored on a MicroSD card (`/ruta.txt`) stand in for updates that will later arrive over the network. The roadmap grows this single-node test into a networked system in small, verifiable steps.

Layout reference: PlatformIO project (`platformio.ini` at repo root, code in `src/main.cpp`, env `esp32doit-devkit-v1`).

## Phase 0 - Foundation

- [x] SD read/write test sketch (Serial input overwrites and reads back `/ruta.txt`). Done when: typing a route in the Serial Monitor updates and prints the file contents without reboot.
- [x] Migrate project to PlatformIO. Done when: `platformio.ini` and `src/main.cpp` exist and the code matches the original sketch's behavior.
- [x] Verify build from CLI. Done when: `pio run` succeeds on a clean checkout.
- [ ] Verify upload and monitor from CLI. Done when: `pio run -t upload` and `pio device monitor` reproduce the SD test.
- [x] Update README for PlatformIO usage. Done when: README setup steps no longer depend on Arduino IDE.

## Phase 1 - Single-node hardening

- [ ] Handle SD init failure. Done when: with no card inserted, the node prints a clear error and keeps running.
- [ ] Recover from SD removal or reinsertion. Done when: reinserting the card restores read/write without a reboot.
- [ ] Derive a node ID from the MAC address. Done when: the node prints the same stable ID on every boot.
- [ ] Define the route data format (see Open Decisions). Done when: the format is documented in the repo with at least two example records.
- [ ] Parse and validate incoming route input. Done when: malformed input is rejected with a message and does not touch `/ruta.txt`.
- [ ] Write safely (write temp file, then rename). Done when: cutting power mid-write leaves either the old or the new route intact, never a partial file.

## Phase 2 - Mesh networking between ESP32 nodes

- [ ] Decide mesh technology (compare ESP-NOW, ESP-WIFI-MESH, painlessMesh on range, node count, complexity, Pi compatibility). Done when: the choice and reasons are recorded under Open Decisions.
- [ ] Bring up the mesh stack on one node. Done when: the node initializes it and logs its ID without errors.
- [ ] Exchange a message between 2 nodes. Done when: a message sent from node A is printed on node B.
- [ ] Send a route update from one node to another. Done when: node B receives a valid route record from node A.
- [ ] Persist a received route to SD. Done when: a route received over the network is in `/ruta.txt` after reboot.
- [ ] Propagate a route to all nodes. Done when: a route entered on one node ends up on every other node in a 3-node setup.
- [ ] Multi-node test with 3 or more nodes. Done when: all nodes hold the same route after a series of updates from different nodes.

## Phase 3 - Raspberry Pi integration

- [ ] Decide the Pi link (for example Wi-Fi root/bridge node with MQTT or HTTP, versus USB serial gateway). Done when: the choice and protocol are recorded under Open Decisions.
- [ ] Designate a gateway node. Done when: one node is configured to bridge between the mesh and the Pi link.
- [ ] Pi receives node data. Done when: a route stored on any node appears on the Pi.
- [ ] Pi pushes a route update into the mesh. Done when: a route sent from the Pi is stored on all nodes.
- [ ] Document Pi setup. Done when: the README lists the steps to bring up the Pi side from scratch.

## Phase 4 - Reliability and demo

- [ ] Reconnect after a node drops out. Done when: a node that is power-cycled rejoins the mesh and catches up to the current route.
- [ ] Reconnect the Pi link. Done when: the gateway resumes Pi communication after the Pi restarts.
- [ ] Handle duplicate messages. Done when: sending the same update twice results in a single applied change.
- [ ] Handle out-of-order updates (for example via version or timestamp). Done when: an older update arriving late does not overwrite a newer route.
- [ ] Write an end-to-end demo script. Done when: a written step-by-step script runs from power-on to a route update visible on all nodes and the Pi.
- [ ] Rehearse the demo. Done when: the script completes successfully twice in a row from a cold start.

## Open Decisions

- [ ] Mesh technology (ESP-NOW, ESP-WIFI-MESH, painlessMesh).
- [ ] Raspberry Pi link and protocol (Wi-Fi bridge with MQTT or HTTP, USB serial gateway, other).
- [ ] Route data format (fields, encoding, versioning).
- [ ] Number of nodes for the demo.
- [ ] Power source for the nodes (USB, battery, other).
