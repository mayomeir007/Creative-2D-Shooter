# Creative 2D Shooter — Game Design Spec

Status: **draft for review**. This document is meant to be complete enough that an
implementer with no other context can build the game from it alone. Anywhere a
choice was not explicitly given by the requester, it is called out under
[Assumptions & Open Items](#assumptions--open-items) — please confirm or override
those before implementation starts.

Target codebase: `homework/src/*` (raylib + C++ STL only). Window is fixed at
1920x1080 @ 60 FPS (`main.cpp`).

This document intentionally covers **mechanics and rules only** — object-oriented
structure, class boundaries, and how systems like weapons should be modeled in
code are a separate design pass to be done afterward.

---

## 1. Game Flow / States

Five states, owned by `Game`:

- **MainMenu** — shown on launch. Centered title text + a clickable **Start**
  button. Clicking Start (LMB) resets all game state and transitions to
  Playing. Esc quits the application from here too.
- **Playing** — active gameplay, described in the rest of this doc.
- **Paused** — entered from Playing by pressing **P**. All gameplay updates
  freeze in place: no movement, no firing, no cooldown/reload timers advance.
  The last active frame stays on screen as the backdrop, with a centered
  "PAUSED" overlay on top. Pressing **P** again resumes Playing exactly where
  it left off — no state is reset or lost. P has no effect in any other
  state (MainMenu, GameOver, Win).
- **GameOver** — entered when player health reaches 0. Freeze the battlefield
  as the backdrop, overlay centered "GAME OVER" text (red) + a **Start**
  button that begins a brand-new game (back to Playing, not MainMenu).
- **Win** — entered when all enemies are dead. Same treatment as GameOver but
  green "YOU WON" text.

**Esc's behavior depends on which state you're in.** From **MainMenu**, Esc
quits the application immediately, no confirmation. From **any other
state** — Playing, Paused, GameOver, or Win — Esc immediately returns to
MainMenu instead, abandoning whatever game is in progress (no Win/Loss is
recorded for it, and score is reset to 0 — see "Score lifetime" below). To
actually close the application from inside a game, Esc backs out to MainMenu
first, then Esc again quits. There is no separate "return to menu" keybind — Esc is
overloaded to serve both purposes depending on state.

**Implementation note:** raylib defaults `KEY_ESCAPE` as the built-in
trigger for `WindowShouldClose()`, which would fire regardless of game state
and short-circuit the state-dependent behavior above. `SetExitKey(KEY_NULL)`
must be called during init to disable that default, and `Game` needs its own
explicit signal (e.g. a `ShouldQuit()` query or quit flag) that `main()`
checks alongside `WindowShouldClose()`, since the current `Game` interface
has no way to request an application quit.

GameOver and Win are visually/functionally the same "results screen" with a
different title/color — treat as one reusable concept, not two.

There is no wave/round progression: one Start → one battle → one outcome →
Start again. (Confirmed with requester: not endless waves.)

### What a new game resets

Every time Start is pressed (from MainMenu, GameOver, or Win), the following
are torn down and rebuilt fresh:

- **Player** — position back to spawn (§3.1), health back to full, magazine
  refilled, any in-progress reload cancelled.
- **Enemies** — the previous set is discarded and a new set of the fixed
  enemy count (§3.2) is spawned. Their *positions* land in exactly the same
  spots every game, since the spawn layout is a fixed deterministic formula,
  not randomized — but each enemy's cosmetic color is freshly re-randomized.
- **Obstacles** — the entire field is regenerated from scratch: new random
  sizes and positions via the same rejection-sampling rules (§3.4), still
  using the same fixed obstacle count.
- **Projectiles** — anything in flight is cleared immediately.

**Score is handled separately** — pressing Start does not rebuild it. Its
lifetime follows the rules below.

### Score lifetime

Score is `+1` per enemy killed (§6) and starts at 0 when the application
launches.

- **Reset to 0 on a loss.** When the player dies (GameOver), the final score
  stays visible on the GameOver screen as-is; it is zeroed the moment the
  player leaves that screen, whether by Start or by Esc. So the next game
  after a loss always begins at 0.
- **Reset to 0 on entering MainMenu**, from any state (Playing, Paused,
  GameOver, Win) via Esc. So Start pressed from MainMenu always begins at 0.
- **Not reset on a win.** Pressing Start from the Win screen keeps the
  current score, so score accumulates across consecutive wins until the
  player either loses or returns to MainMenu.

---

## 2. Arena & Coordinate System

- Screen: 1920×1080, origin top-left, +x right, +y down (raylib default).
- **Arena margin `M = 60px`**: a border this far in from the screen edges is
  drawn as a thin outline (matches the reference screenshots' border box) and
  acts as the hard movement boundary for player, enemies, and projectiles.
  - Arena rect: `x=M, y=M, width=1920-2M, height=1080-2M`.
- Nothing (player, enemy, projectile) may have its center leave the arena
  rect; circles are additionally clamped so their edge doesn't cross the
  border (clamp center to `[M+radius, 1920-M-radius]` etc.).
- Projectiles that reach the arena boundary are destroyed (not bounced).

---

## 3. Entities

### 3.1 Player

| Property | Value |
|---|---|
| Shape | Circle, radius `32px` |
| Color | Blue, RGB `(77, 171, 247)` — same blue as the sample circle in the starter `Guide.hpp` |
| Start position | Bottom-middle of arena, flush against the bottom boundary: `(width/2, height - M - radius)` |
| Move speed | `260 px/s` (see [movement rules](#movement-rules)) |
| Facing | Always toward current mouse cursor position |
| Max health | `6` (each incoming hit = 1 damage) |
| Weapon | Default `Uzi` (see [§4 Weapon System](#4-weapon-system)) |
| Collision | Circle vs. arena bounds, circle vs. obstacle rects, circle vs. enemy circles (push-apart, no damage from touching) |

Note the `- radius` term: it's there so the spawn point is already the
farthest-down legal position under the arena clamp in §2 (`height - M -
radius`), not one radius past it. Spawning at `height - M` would place the
player's center 32px outside its own clamp range, so it would get silently
snapped up by 32px on the very first frame before ever moving — spawning
pre-clamped avoids that invisible jump entirely.

**Movement rules:** WSAD moves the player in absolute screen directions
(W=up, S=down, A=left, D=right), normalized so diagonal movement isn't
faster than axial movement. **Movement direction is independent of facing**
— this is a twin-stick shooter, not a tank-steering one. See §5 for exactly
how movement is resolved against obstacles and other circles.

**Firing:** Holding LMB or Space fires automatically at the current weapon's
fire rate (see weapon table) as long as: not currently reloading, and the
magazine has ≥1 round. If the magazine is empty, holding the fire button does
nothing (silent dry-fire, no auto-reload). Pressing **R** starts a reload at
any time (even with a partially-full magazine) as long as not already
reloading; firing is disabled for the reload's duration, after which the
magazine refills to full. Ammo is **infinite reserve** — there is no total
ammo pool, only the magazine; the HUD ammo readout is always `current/50`.

**Visual:** Filled circle in the player's color, a short rectangle "gun"
protruding from the edge toward the facing direction, two small dot "eyes"
offset toward facing. A thin horizontal health-bar rectangle floats a fixed
offset above the player's head, filled proportionally to `health/maxHealth`,
always axis-aligned regardless of facing. `health` is always clamped to
`[0, maxHealth]` immediately when damage is applied (§5, step 11) — it can
never go negative, not even for a single frame before the win/loss check
runs — so the bar's fill fraction is always a valid, non-negative value.

### 3.2 Enemy

| Property | Value |
|---|---|
| Shape | Circle, radius `32px` (same as player) |
| Color | Cosmetic only, no gameplay meaning — randomly generated per enemy, independently, so it works for any enemy count (no fixed palette). Each color is a random hue in `[0°, 360°)` at fixed saturation `0.55` and brightness `1.0` (bright pastels, readable on the dark background), **excluding hues within ±30° of the player's blue** (hue ≈ 207°, so `[177°, 237°]` is rejected and re-rolled) so no enemy can be mistaken for the player |
| Move speed | `260 px/s` (same as player, per spec) |
| Max health | `6`, same damage-per-hit rules as player |
| Weapon | Its own distinct weapon definition, `Pistol`, separate from the player's Uzi — see §4 |
| Ammo | Infinite — enemies never reload |
| Facing | Rotates toward the player at a limited turn rate — no longer purely cosmetic, it now gates firing (see below) |

**Enemies carry their own weapon, not a scaled copy of the player's.** A
weapon with a different fire rate is conceptually a different weapon, not
the same weapon behaving differently depending on who's holding it. So
rather than multiplying the player's fire rate at the moment an enemy fires,
enemies are simply equipped with a separate weapon definition that happens
to have a slower fire rate authored into it directly. See §4 for its stats.

**Facing has a turn rate, and starts pointing straight down.** Unlike the
player (§3.1), whose facing snaps instantly to the mouse every frame with no
turn-rate limit, each enemy's facing rotates toward the current
direction-to-player at a limited angular speed of **180°/second** — a full
360° rotation takes **2.0 seconds**. It always turns via the shorter angular
path toward that target and never overshoots past it. Every enemy's facing
is initialized to point straight down (toward the bottom of the screen) at
spawn, regardless of where it actually sits relative to the player. With
the player spawning at bottom-middle and `N=5`, the initial error works out
to: 0° for the enemy directly above the player, about 31° for the next two
along the top edge, and about 51° for the two on the side edges (which
spawn at y=264, i.e. 900px to the side and 724px above the player). So the
worst-case initial turn is roughly 51°, which takes about 0.3s at the turn
rate below. The facing target is always the
straight-line direction to the player, regardless of whether that line is
currently obstructed — an enemy keeps turning to (eventually) face the
player even while its corner-seek movement (below) is carrying it a
different way; obstruction is what blocks firing (via the line-of-sight
gate), not what the enemy is trying to face.

This is what fixes the spawn-alpha-strike problem: since every enemy's fire
cooldown starts at zero, without a turn delay every enemy with a clear line
of sight would open fire in the very first frame. Requiring facing to be
aligned with the player before firing (see Combat, below) means every enemy
needs a moment to turn before it can land its first shot, giving the player
a brief window at the start of each game.

**Movement — reacquire-and-approach behavior:**

Every frame, the enemy tests the straight line segment from its own center
to the player's center against every obstacle:

1. **Line is clear:** the enemy moves directly toward the player's current
   position at its move speed. This is the default/most common state.
2. **Line is blocked:** the enemy does *not* stop or wait — it walks toward
   whichever corner of the blocking obstacle's rectangle is nearest to it
   (i.e., the first corner it would reach while going around that side of
   the obstacle).
3. **Every subsequent frame while still blocked**, the same test repeats: if
   the enemy has rounded far enough that it now has a clear line to the
   player, it immediately switches back to behavior 1 (direct approach). If
   it's still blocked — either by the same obstacle or a different one now
   in the way — it continues heading toward the next nearest corner. In
   effect the enemy hugs the obstacle's perimeter, corner to corner, for as
   long as it's blocked, and peels off toward the player the instant a clear
   shot opens up.

There is no separate "going around" mode or memory of a plan — it's a purely
reactive, per-frame decision based on the current line-of-sight test. This is
a cheap steering heuristic, not full pathfinding (see
[Assumptions](#assumptions--open-items)).

**Interaction with the arena boundary:** a corner-seek target is a desired
position to steer toward, not a guaranteed-reachable one. An enemy heading
toward one is still subject to the exact same arena-bounds clamp (§2) as any
other entity, applied on top of its steering movement every frame, with no
special-case override. If reaching that corner would require leaving the
arena — e.g. an obstacle sits close enough to the boundary that going around
it on that side runs off the map — the clamp simply wins: the enemy makes
only as much progress as the boundary allows and can end up stalled against
the wall, still blocked, until the geometry changes (typically because the
player moves and opens a line of sight from a different angle). This isn't
handled as a special case; it falls out naturally from applying the same
clamp every entity always gets. See [Assumptions](#assumptions--open-items).

Enemies never stop advancing toward the player, even while shooting —
movement and firing happen simultaneously. What actually stops an enemy from
walking into/through the player is the circle-vs-circle push-apart described
in §5, which naturally halts it at contact distance.

**Combat:** An enemy fires at the player whenever **all three** are true:
(a) the same line-of-sight segment used for movement is currently
unobstructed, (b) its own fire-rate cooldown has elapsed, and (c) its
current facing is aligned with the direction to the player within a **5°
tolerance** (i.e., it has finished, or nearly finished, turning to face its
target — see the facing/turn-rate paragraph above). The cooldown still
ticks down in the background regardless of facing, so once alignment is
reached the enemy fires on the very next eligible frame rather than waiting
through a second cooldown. No leading/prediction of player movement in v1 —
enemies aim directly at the player's current position.

**Spawn layout:** Enemies are placed along a fixed path that covers the top
edge and the top halves of the left/right edges (i.e., opposite and around
from the player's bottom-middle start). Concretely, picture four points: the
midpoint of the left edge, the top-left corner, the top-right corner, and
the midpoint of the right edge. Walking from the first point to the last
traces exactly the top edge plus the top halves of the two side edges; call
its total length (measured along all three segments) `L`.

With a fixed enemy count `N` (currently 5), divide that walk into `N` equal
stretches of length `L/N`, and place each enemy at the **midpoint of its own
stretch** rather than at a stretch boundary: enemy index `i` (counting from
0 at the left-edge end to `N-1` at the right-edge end) sits at arc-length
distance `(i + 0.5) × (L/N)` measured from the walk's start. Using the
midpoint of each stretch — not its edge — is what keeps every enemy exactly
half a stretch away from the nearest corner and from the two path endpoints,
so with `N=5` nobody spawns exactly on a corner, exactly at the two vertical
midpoints, or bunched together at either end.

**Visual:** Same treatment as the player (§3.1) — a filled circle in the
enemy's own (randomized) color, a short rectangle "gun" protruding from the
edge toward its current facing, and two small dot "eyes" offset toward
facing. The only visual difference from the player is that enemies do
**not** get a health bar above them (§6) — only the player's health is ever
shown.

### 3.3 Projectile

| Property | Value |
|---|---|
| Shape | Filled circle, radius `5px` |
| Speed | `900 px/s`, identical for player and enemy shots |
| Damage | `1` per hit |
| Color | One fixed color for player shots, a different fixed color for enemy shots (purely visual) |
| Lifetime end | On hitting a valid target (apply damage, destroy self), on hitting any obstacle (destroy self, no damage), or on leaving the arena bounds (destroy self) |
| Friendly fire | Player shots only ever damage enemies. **Enemy shots can damage the player or any enemy** — including one enemy accidentally shooting another that's standing between it and the player |

Spawned at the shooter's circle edge (center + radius along facing
direction), not at the exact center, so it doesn't immediately register a
self-collision.

### 3.4 Obstacle

| Property | Value |
|---|---|
| Shape | Axis-aligned rectangle (no rotation — confirmed with requester) |
| Count per game | A **fixed constant, currently 8** — the same count every playthrough, not randomized, changeable only by editing the constant in source |
| Size | Width and height each randomized in `[40, 260]px`, independently, per obstacle |
| Placement | Uniform-random position inside the arena, **rejection-sampled** against the two rules below |
| Blocks | Movement and projectiles, for both player and enemies |
| Visual | Filled rectangle + lighter outline, matching `Guide.hpp`'s existing draw style |

**Rejection rule 1 — minimum gap from spawn points.** A candidate obstacle is
rejected if it comes within `150px` of the player spawn point or any enemy
spawn point. That `150px` is measured as the shortest distance from the
spawn point to the **nearest point on the obstacle's rectangle** (its edge,
or its interior if the point would fall inside it) — not to the rectangle's
center. Measuring to the center would let a large obstacle's edge sit much
closer than 150px to the spawn point (a 260px-wide obstacle could have an
edge only 20px away while its center is still 150px off), which defeats the
point of the clearance; measuring to the nearest point guarantees a genuinely
empty 150px radius around every spawn point regardless of obstacle size.

**Rejection rule 2 — minimum gap between obstacles.** A candidate obstacle is
rejected if it comes too close to any obstacle already placed. "Too close"
is defined per pair as the sum of each obstacle's own half-thickness, where
an obstacle's half-thickness is half of its **smaller** dimension (i.e.
`min(width, height) / 2` — using the smaller side rather than the side facing
the gap keeps the rule simple and gives thin obstacles a proportionally
smaller buffer, without needing to special-case orientation). So for
obstacles A and B, the required minimum gap is
`min(A.width, A.height)/2 + min(B.width, B.height)/2`. The actual gap
between two axis-aligned rectangles is computed the standard way: find the
horizontal gap (0 if their x-ranges overlap, otherwise the distance between
their nearest x-edges) and the vertical gap (same idea on y), then combine
them as `sqrt(horizontalGap² + verticalGap²)`. This naturally collapses to
a plain horizontal or vertical distance whenever the pair is purely
side-by-side or purely stacked (verticalGap or horizontalGap is 0 in those
cases), and handles a diagonal arrangement correctly when both are nonzero.
The candidate is rejected if this actual gap is smaller than the required
minimum gap for that pair (this check alone also guarantees no two
obstacles ever overlap, since the required gap is always positive).

Rejection sampling is capped at a reasonable retry count (e.g. 200 attempts
per obstacle); if it can't find a valid spot, that obstacle is simply
skipped. There is **no** guarantee-of-solvability check (e.g., no validation
that a path always exists between player and every enemy) — see
[Assumptions](#assumptions--open-items).

---

## 4. Weapon System

Every weapon is described by the same handful of numbers: a magazine size,
a fire rate (shots per second), a projectile speed, a damage-per-hit value,
and a reload time. v1 ships exactly two weapon definitions — the player's
`Uzi`, and a separate weapon, `Pistol`, that every enemy is equipped with:

| Weapon | magazineSize | fireRateRoundsPerSecond | projectileSpeed | damagePerHit | reloadTimeSeconds |
|---|---|---|---|---|---|
| Uzi (player) | 50 | 8 | 900 | 1 | 3.0 |
| Pistol (enemy) | n/a — infinite | 4 | 900 | 1 | n/a — never reloads |

The enemy's Pistol is its **own weapon definition**, not the Uzi with a
runtime modifier applied — it shares the Uzi's projectile speed and damage,
but its fire rate (`4`) is simply the number authored into it, chosen to be
half the Uzi's `8`. If the Uzi's fire rate is ever retuned later, the
Pistol's fire rate does not automatically follow — it's an independent
value that happens to currently sit at that ratio. Magazine size and reload
time don't apply to it, since enemies have infinite ammo and never reload.

Both the player and each enemy separately track their own live ammo count
and time until their next allowed shot; the player additionally tracks time
remaining on an in-progress reload.

**Note:** the enemy's Pistol is not the same thing as the deferred "multiple
weapon types / pickups" task below — it's simply the one fixed weapon every
enemy is born with, not something the player can pick up or swap to.

**Weapon pickups and a second *player-facing* weapon type are deferred out
of v1** — confirmed acceptable by the requester (see §9 Confirmed). How
multiple player weapons and pickups should be modeled is left for the
follow-up object-oriented design discussion.

---

## 5. Collision & Movement Resolution

### Per-frame update order

Every frame while in the **Playing** state, the game updates in exactly this
order (Paused/MainMenu/GameOver/Win skip all of it and just render the
current frozen frame):

1. **Input sampling** — read WSAD, current mouse position, whether the fire
   button (LMB/Space) is held, and whether R/P/Esc were just pressed, once
   for the frame.
2. **Global state transitions** — Esc or P are handled first and can end the
   frame early: Esc returns to MainMenu (§1); P toggles Paused. If either
   fires, none of the remaining steps below run this frame.
3. **Timers tick down** — the player's reload timer (if reloading) and every
   entity's fire-cooldown timer (player and each enemy) are decremented by
   `dt`. A reload timer reaching zero refills the magazine and clears the
   reloading flag.
4. **Movement/steering intent** — compute this frame's desired displacement:
   for the player, from WSAD; for each enemy, from the line-of-sight test in
   §3.2 (direct-approach or corner-seek).
5. **Axis-separated slide vs. obstacles** — apply each entity's desired
   displacement using the per-axis check described below.
6. **Arena-bounds clamp** — clamp every entity's resulting position to the
   arena rect (§2), including an enemy mid corner-seek (see the boundary
   note in §3.2).
7. **Circle-vs-circle push-apart** — resolve overlaps between player/enemy
   circles (below), then re-apply the arena-bounds clamp once more. (Not
   re-checked against obstacles — see the note at the end of this section.)
8. **Facing update** — the player's facing snaps instantly to the current
   mouse direction (unchanged, no turn-rate limit). Each enemy's facing
   rotates toward the current direction-to-player by at most
   `180°/s × dt` this frame, via the shorter angular path, without
   overshooting past it. (Every enemy's facing starts pointing straight down
   at spawn — see §3.2.)
9. **Firing** — for the player: if the fire button is held, the magazine has
   ≥1 round, and its cooldown has reached zero, spawn a projectile at the
   player's current (already-moved) position/facing, decrement ammo, and
   reset the cooldown. For each enemy: same check, but gated on its line of
   sight being currently clear (instead of a held button) **and** its
   facing being within 5° of the direction to the player (§3.2).
10. **Projectile movement** — every projectile, **including any spawned in
    step 9 this same frame**, moves by `speed × dt`. A projectile therefore
    starts moving on the very frame it's fired — it is never held stationary
    for one frame after spawning.
11. **Projectile collisions** — check every projectile against obstacles,
    the arena boundary, and opposing circles (per the summary table below);
    apply damage — **clamping the target's resulting health to a minimum of
    0** (health can never go negative, not even for a single frame) — and
    destroy projectiles as needed.
12. **Win/loss check** — if the player's health has reached 0, transition to
    GameOver; else if the enemy count has reached 0, transition to Win.

Draw always happens after Update and never mutates game state.

### How movement interacts with obstacles: axis-separated slide

Each entity's intended movement for the frame is a 2D displacement (e.g.
"move left by 4px this frame"). Rather than testing that whole displacement
against obstacles as one diagonal step, it's split into its horizontal and
vertical components and each is tried **separately, in sequence**:

1. Try applying just the horizontal (x) part of the movement. If the
   resulting position would overlap an obstacle, that horizontal movement is
   discarded (the entity's x position doesn't change this frame); otherwise
   it's kept.
2. Then, independently, try applying just the vertical (y) part of the
   movement (starting from whatever x position step 1 left it at). Same
   rule: discard it if it would cause an obstacle overlap, otherwise keep it.

The effect: if an entity is moving diagonally and only one of the two
directions is blocked, it still slides along the obstacle's face using the
other direction — it doesn't come to a dead stop just because one axis is
blocked. If an entity is moving in a single pure direction (e.g., straight
up) and that direction is blocked, there's no other component to fall back
on, so it simply stops advancing until the input direction changes.

**Answering directly — player moves up (W only) into an obstacle directly
above it:** the vertical component of the movement is rejected by the check
above (it would overlap the obstacle), and there's no horizontal component
to fall back on (no A/D held), so the player's position doesn't change that
frame — it stays put, pressed against the obstacle's edge, for as long as W
alone is held. If the player also holds A or D, that horizontal slide still
applies normally, so the player slides sideways along the obstacle's bottom
edge instead of being fully stuck.

This axis-separated check is done **per entity, independently**, against
obstacles only — it has nothing to do with other circles (player vs. enemy,
enemy vs. enemy). That's a separate step, described next.

### How circles resolve overlap with each other: push-apart

After every entity has moved for the frame (and been slid against obstacles
as above), a second pass checks every pair of circles (player-vs-enemy,
enemy-vs-enemy) for overlap. If two circles' centers end up closer together
than the sum of their radii, they're overlapping by some amount. That
overlap is resolved by moving **both** circles apart along the line
connecting their centers, each by half the overlap distance, so they end up
exactly touching (distance = sum of radii) rather than overlapping. Neither
circle is treated as "the one that gets stopped" — the correction is always
split evenly between the two.

**Answering directly — player moves left, and an enemy that's already to the
player's left is simultaneously moving right toward the player (because the
player is to its right):** both movements are applied first (nothing here
involves an obstacle, so the axis-separated slide above doesn't reject
either one) — the player moves left, the enemy moves right, and because
they were moving toward each other, the gap between them closes by both
movements combined that frame. If that puts their circles closer together
than the sum of their radii (i.e., they'd overlap), the push-apart pass
kicks in and moves each of them back out along the line between their
centers by half the overlap — the player gets nudged slightly back to the
right, the enemy gets nudged slightly back to the left, until they're just
touching. Neither one "wins": the net effect is that they stop closing
further once they're in contact, with each side absorbing half of the
correction, regardless of the fact that one of them was under direct player
control and the other was AI-driven. Any part of either one's movement that
wasn't along the collision axis (e.g. if the approach wasn't perfectly
head-on) still takes effect normally.

Push-apart is not re-checked against obstacles afterward (the arena-bounds
clamp is still reapplied afterward, per the per-frame order above, but
obstacles are not) — in the rare case it nudges an entity slightly into an
obstacle's corner, that's an accepted minor edge case for this scope (see
[Assumptions](#assumptions--open-items)).

### Summary table

| Pair | Effect |
|---|---|
| Player/Enemy circle ↔ Obstacle rect | Blocks movement (axis-separated slide, above) |
| Player/Enemy circle ↔ Arena bounds | Clamped, cannot exit |
| Projectile ↔ Obstacle rect | Projectile destroyed, no damage |
| Projectile ↔ Arena bounds | Projectile destroyed |
| Player projectile ↔ Enemy circle | Enemy takes damage, projectile destroyed |
| Enemy projectile ↔ Player circle | Player takes damage, projectile destroyed |
| Enemy projectile ↔ another Enemy circle | That enemy takes damage (friendly fire), projectile destroyed; if it dies, the player still scores the kill (§6) |
| Player circle ↔ Enemy circle | Push-apart (above), no damage from contact |
| Enemy circle ↔ Enemy circle | Push-apart (above) |
| Enemy-to-player line of sight ↔ Obstacle rect | Segment intersection test; blocks both enemy firing and enemy's "direct approach" movement mode |

All of the above only need circle-vs-AABB, segment-vs-AABB, and
circle-vs-circle checks — no rotated-rect math, per the axis-aligned
obstacle decision.

---

## 6. UI / HUD

**Bottom-right, stacked, in this order (top to bottom):**
1. `Reloading {remaining:.1f}s` — amber/orange, only rendered while
   reloading; the line is blank (not present) otherwise. Other lines below
   it do **not** shift — the reload line always reserves its vertical slot.
2. `Score: {score}`
3. `Enemies left: {count}`
4. `Ammo: {current}/{magazineSize}`
5. `Health: {current}`

**Bottom-left:** single line control legend:
`WSAD move | Mouse aim | Space/LMB shoot | R reload | P pause`

**Top-right:** FPS counter, same style as the existing starter overlay.

**Center (Paused state only):** a "PAUSED" overlay, styled consistently with
the GameOver/Win results text.

**Above player:** floating health-bar rectangle (see §3.1). No health bars
over enemies (only the player's is visible, per spec).

**Font:** all HUD, menu, and results-screen text reuses the same font
already set up for the starter overlay in `Guide.hpp` (a Segoe UI file
loaded at startup, falling back to raylib's built-in default font if that
file isn't available on the machine). No new font asset is introduced —
one consistent typeface is used everywhere text appears in the game.

**Score rule:** `+1` for every enemy that dies, **regardless of whose shot
killed it** — including an enemy killed by another enemy's stray shot
(friendly fire, §3.3). Exposed as a named constant.

---

## 7. Constants Reference

| Name | Value |
|---|---|
| Screen size | 1920 × 1080 |
| Arena margin `M` | 60px |
| Player/Enemy radius | 32px |
| Player/Enemy move speed | 260 px/s |
| Projectile radius | 5px |
| Projectile speed | 900 px/s |
| Damage per hit | 1 |
| Player/Enemy max health | 6 |
| Health floor | 0 — clamped immediately on damage, never negative |
| Enemy turn rate | 180°/s (a full 360° turn takes 2.0s) |
| Enemy fire-alignment tolerance | 5° |
| Magazine size (Uzi) | 50 |
| Player fire rate (Uzi) | 8 shots/s |
| Enemy fire rate (Pistol) | 4 shots/s |
| Reload time (Uzi) | 3.0s |
| Enemy count | Fixed constant, 5 — not randomized, developer-set only |
| Obstacle count | Fixed constant, 8 — not randomized, developer-set only |
| Obstacle width/height | Randomized in [40, 260]px each, per obstacle |
| Obstacle clearance from spawn points | 150px, measured to nearest point on rectangle (§3.4 rule 1) |
| Obstacle-to-obstacle minimum gap | `min(w,h)/2` of each obstacle, summed (§3.4 rule 2) |
| Score per kill | 1 |

---

## 8. Assumptions & Open Items

These were not explicitly specified and were filled in with a reasonable
default. Flag anything you want changed:

1. **Movement is twin-stick** (WSAD direction independent of mouse-facing
   direction), not tank-steering. Matches "player always faces the mouse"
   read as a purely visual/aiming statement.
2. **R reloads on demand**, not just when the mag is empty; firing on an
   empty mag is a silent no-op rather than an auto-reload.
3. **Score = 1 point per kill.** No scoring rule was given.
4. **Enemy line-of-sight/approach uses a per-frame steering heuristic**
   (aim at the near corner of a blocking obstacle), not real pathfinding
   (A*/navmesh). Deliberately not attempting "seek cover" or predictive
   aiming in v1 — left as clearly-marked future extensions, consistent with
   "not expected to finish everything."
5. **Obstacle field has no solvability guarantee** — it's possible (if
   unlikely, given the clearance radius and count) for random placement to
   make an enemy's path awkward, but there's no maze-connectivity check.
   Considered acceptable for this scope.
6. **Enemy colors are cosmetic/random** with no gameplay meaning, generated
   by the hue rule in §3.2 (the saturation/brightness values and the ±30°
   exclusion band are my choices, not from the requester). The exact colors of projectiles, guns, eyes, obstacles, and the health bar
   are not pinned down in this spec.
7. **Push-apart is not re-validated against obstacles afterward** — see the
   note at the end of §5. A rare, accepted edge case.
8. **Enemies can stall against the arena boundary while rounding an obstacle
   placed near it** — the arena clamp always overrides a corner-seek target
   (§3.2), so if the only route around an obstacle would exit the arena, the
   enemy simply gets stuck at the wall until the player repositions enough
   to reopen a line of sight. No unsticking logic is specified; this is
   accepted as part of the same no-guaranteed-solvability limitation as
   item 5 above.
9. **Projectile collision is a discrete post-move point check, not a
   swept/continuous test.** Per the per-frame order in §5 (step 10 moves
   projectiles, step 11 then checks collisions), a projectile is tested
   against obstacles/circles/bounds only at its new end-of-frame position,
   not along the path it traveled that frame. At 900px/s (~15px/frame at
   60 FPS) against a 40px-minimum obstacle dimension and a 5px projectile
   radius, tunneling through a corner is unlikely but not impossible.
   Accepted as the same class of edge case as items 5, 7, and 8 above — no
   continuous collision detection in v1.

---

## 9. Explicitly Confirmed (not open for silent change)

Recorded here so a future re-read of this doc doesn't accidentally relitigate
decisions already made with the requester:

- Ammo is **infinite reserve** (magazine always refills to 50 on reload).
- Obstacles are **axis-aligned only**, no rotation.
- Winning **ends the round** (Start again for a new one) — no auto-respawn
  waves.
- Enemy count and obstacle count are **fixed, non-random constants**,
  changeable only by editing source — not randomized per game, not
  runtime-configurable.
- Enemy shots **can** damage other enemies (intentional friendly fire among
  enemies); player shots cannot damage the player.
- **Weapon pickups and additional weapon types are deferred out of v1**
  (assumption confirmed acceptable, even though listed as a task in the
  broader assignment brief).
- **Hit/death effects will be a minimal placeholder** (brief flash/scale-down
  on hit or death) rather than a full particle system — confirmed acceptable
  scope.
- **Score lifetime**: reset to 0 on a loss and whenever the game returns to
  MainMenu; carried over on a win (accumulating across consecutive wins).
  Everything else about the game state resets fresh on Start — see "What a
  new game resets" and "Score lifetime" in §1.
