# Roadmap MVP — Radon

## Priority Legend

- 🔴 **Blocker** — Project is unplayable without this
- 🟠 **High** — Critical system, affects core loop
- 🟡 **Medium** — Improves playability, can be temporarily stubbed
- ⚪ **Low** — Non-critical, can be postponed

---

## Stage 1 — Core Setup & Data Loading

| Task                                             | Priority | Notes                                        |
| ------------------------------------------------ | -------- | -------------------------------------------- |
| Initialize rbfx project with CMake               | 🔴        | Set up basic structure and `main.cpp`        |
| Logger + developer console                       | 🔴        | For debugging, modding, and development      |
| JSON loading system                              | 🔴        | Plug in data-driven registries               |
| Load item & weapon data                          | 🔴        | `ItemRegistry` and `ItemData` struct         |
| Directory structure (Data, Assets, Scenes, Mods) | 🔴        | Avoid later reorganization                   |

---

## Stage 2 — Player & Basic Mechanics

| Task                             | Priority | Notes                                      |
| -------------------------------- | -------- | ------------------------------------------ |
| Player controller (FPS)          | 🔴        | Walk, run, jump, camera look               |
| Stamina system                   | 🟠        | Limits sprint and other actions            |
| Damage & death                   | 🔴        | Foundation of combat                       |
| Inventory: UI & logic            | 🔴        | Grid, drag & drop, weight                  |
| World-item support               | 🟠        | Spawn & pickup objects                     |
| Minimap HUD                      | 🟡        | Can simplify or defer                      |

---

## Stage 3 — Combat System & AI

| Task                             | Priority | Notes                                           |
| -------------------------------- | -------- | ----------------------------------------------- |
| Hip-fire shooting                | 🔴        | For all weapon types                            |
| Weapon & ammo implementation     | 🔴        | From JSON: damage, rate of fire, etc.           |
| Basic enemy system (bandits)     | 🔴        | Detect player, attack, die                      |
| Enemy spawning in scenes         | 🟠        | Spawn points or script nodes                    |
| Armor: damage mitigation         | 🟠        | Damage modifiers, weight, durability            |

---

## Stage 4 — World & Transitions

| Task                                    | Priority | Notes                                       |
| --------------------------------------- | -------- | ------------------------------------------- |
| Load `.scene` by name                   | 🔴        | Zone transitions                            |
| Implement WorldGraph                    | 🔴        | Connect locations, track state              |
| Transition system (zones or triggers)   | 🟠        | Auto-transitions or “Enter” button          |
| Save location state                     | 🔴        | Loot, enemies, environmental changes        |
| Trader: UI & logic                      | 🟠        | Buy/sell window, currency calculation       |

---

## Stage 5 — UI & UX

| Task                                         | Priority | Notes                                          |
| -------------------------------------------- | -------- | ---------------------------------------------- |
| Main menu: Continue / New Game / Exit        | 🟡        | Important for UX, not critical to core logic   |
| Pause & settings menu                        | ⚪         | Can defer                                     |
| HUD: health, ammo, stamina                   | 🔴        | Always on screen                              |
| Inventory: visual interface                  | 🔴        | Integrate with backend                        |
| Trading: UI                                  | 🟠        | Needed for trader interaction                  |

---

## Stage 6 — Saving & Modding

| Task                               | Priority | Notes                                          |
| ---------------------------------- | -------- | ---------------------------------------------- |
| Save world & player state          | 🔴        | JSON in save folder                            |
| Auto-save on timer                 | 🟠        | Can defer but recommended                      |
| Manual save/load                   | 🔴        | Via menu or hotkey                             |
| Save versioning                    | 🟡        | For future compatibility                       |
| Mods folder support                | 🟠        | Load custom JSON overrides                     |

---

## Stage 7 — Content & Scenes

| Task                                            | Priority | Notes                      |
| ----------------------------------------------- | -------- | -------------------------- |
| Build two full locations (POIs, art, layout)    | 🔴        | Minimal viable world       |
| Place enemies & loot                            | 🔴        | For combat & exploration   |
| Add trader & safe zone                          | 🔴        | Starting hub               |
| Lighting & ambience tuning                      | 🟠        | Visual atmosphere          |
| Footstep, gunshot, ambient sounds               | 🟡        | Atmosphere & feedback       |

---

## Stage 8 — Debugging & Testing

| Task                           | Priority | Notes                                            |
| ------------------------------ | -------- | ------------------------------------------------ |
| Developer console              | 🟠        | spawn, tp, give, toggle logs, etc.               |
| Scene & transition debugging   | 🔴        | Ensure stable loads & zone swaps                 |
| State inspector                | ⚪         | For future variable debugging                    |
| File-based logging system      | 🔴        | Vital for release-stage error tracking           |
