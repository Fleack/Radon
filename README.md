# Game Design Document: Project “Radon”

## General Information

- **Project name:** Radon (working title)
- **Genre:** FPS / Immersive Sim / Survival Exploration
- **Target platform:** Windows
- **Engine:** rbfx (C++)
- **Language:** C++23
- **Build system:** CMake
- **IDE:** CLion
- **Network mode:** Single-player, with ability to expand to P2P co-op
- **Atmosphere:** Post-Soviet post-apocalypse in the style of S.T.A.L.K.E.R., Metro, Tarkov

## Concept

An exploration-driven, atmospheric single-player shooter with survival elements, a hand-drawn map, nonlinear progression, and a gear/inventory system—eventually expandable with co-op and mod support.

## Goals

### MVP (Minimum Viable Product)

**Primary goal:** Deliver a basic single-player build including two locations, a combat system, a trader, and inventory.

### Post-MVP Expansion

**Goal:** Gradually scale up by adding story, factions, A-Life, co-op, and richer mod support.

---

# 1. MVP Contents

## 1.1 World

- **Structure:** Zone-based, 2 maps (3–4 POIs each)
- **Transition type:** Portals/zones
- **WorldGraph:** A graph-based world map with persistent location states

## 1.2 Player

- **View:** First-person (body not rendered yet, placeholder for future)
- **Movement:** Walk, run, stamina
- **Shooting:** Hip-fire only, no ADS
- **Death:** Reload at last save point

## 1.3 Combat & Weapons

- **Weapons:** 8 total (3 pistols, 1 shotgun, 3 rifles, 1 sniper)
- **Ammo:** Typed, stored as counts
- **Combat feel:** Simple but with realistic physics and sound
- **Planned:** Weapon mod system (scopes, attachments)

## 1.4 Inventory

- **Type:** Grid without slot limit, weight cap applies
- **Functions:** Move, use, display weight

## 1.5 Enemies

- **Type:** Bandits
- **AI:** Idle or patrolling, aggressive on player detection

## 1.6 UI

- **Main menu:** Yes (load, exit)
- **HUD:** Health, stamina, ammo, minimap
- **Inventory:** Drag & drop

## 1.7 Items

- **Categories:** Weapons, armor (5 types), medkits, energy drinks, ammo
- **Planned:** Item categories, mod expansions

## 1.8 Trading

- **Trader:** Single NPC with fixed stock
- **Buy/sell:** No dynamic pricing

## 1.9 Map

- **Revealed:** Fully visible from the start
- **Format:** Data-driven (texture + POIs)
- **Future:** Player-placed markers, fog of war until explored

## 1.10 Saving

- **Format:** JSON folder per save
- **Frequency:** Manual + auto-save every N minutes
- **Versioning:** Yes

## 1.11 Modding

- **Built-in:** All game data in JSON, loadable from `Mods/`
- **Loader:** Registries for items, scenes, enemies, etc.

---

# 2. Post-MVP Expansion

## 2.1 Story & Quests

- Main storyline
- Branching side-quests (inspired by ATOM RPG)

## 2.2 A-Life System

- Random events, NPC movements, world simulation

## 2.3 Factions

- Relationships, reputation, NPC interactions

## 2.4 New Enemy Types

- Mutants, anomalies, hostile zones

## 2.5 Weather System

- Rain, fog, lighting, dynamic skies

## 2.6 Lighting & Graphics

- Post-processing support, custom loot shaders, soft shadows

## 2.7 Co-op (P2P)

- 1–3 player connections via P2P
- Shared progress, item and action sync

## 2.8 RPG-style Progression

- Skills improve through use (à la Tarkov)
- Impact on combat and non-combat stats

## 2.9 World Expansion

- New zones added via WorldGraph
- Modular content loading

## 2.10 UI Enhancements

- Sights, flashlight, status indicators, quest tracker

## 2.11 Scripting

- NPC behaviors, quests, events scriptable (Lua/YAML)
