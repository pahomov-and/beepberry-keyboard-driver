# Beepberry Keyboard — Layers Map

## Physical Layout

```
BASE

  [ CALL ON ] [   WIN   ] [  MENU   ] [ CALL OFF ]

  [ Q ][ W ][ E ][ R ][ T ][ Y ][ U ][ I ][ O ][ P ]
  [ A ][ S ][ D ][ F ][ G ][ H ][ J ][ K ][ L ][ ⌫  ]
  [ALT][ Z ][ X ][ C ][ V ][ B ][ N ][ M ][ $ ][ ↵  ]
  [ ⇧ ][MIC][        SPACE        ][SYM]         [ ⇧ ]


SYM  (tap or hold SYM)

  [ CALL ON ] [   WIN   ] [  MENU   ] [ CALL OFF ]

  [ # ][ 1 ][ 2 ][ 3 ][ ( ][ ) ][ _ ][ - ][ + ][ @ ]
  [ * ][ 4 ][ 5 ][ 6 ][ / ][ : ][ ; ][ ' ][ " ][ ⌫  ]
  [ALT][ 7 ][ 8 ][ 9 ][ ? ][ ! ][ , ][ . ][MUT][ ↵  ]
  [ ⇧ ][ 0 ][        SPACE        ][SYM]         [ ⇧ ]


SYM + SHIFT  (third layer, top row only)

  [ CALL ON ] [   WIN   ] [  MENU   ] [ CALL OFF ]

  [ { ][ } ][ [ ][ ] ][ < ][ > ][ ^ ][ ~ ][ = ][ | ]
  [ * ][ ` ][ % ][ \ ][ / ][ : ][ ; ][ ' ][ " ][ ⌫  ]
  [ALT][ & ][ * ][   ][ ? ][ ! ][ , ][ . ][MUT][ ↵  ]
  [ ⇧ ][ 0 ][        SPACE        ][SYM]         [ ⇧ ]


MOD4  (hold WIN)

  [ CALL ON ] [   WIN   ] [  MENU   ] [ CALL OFF ]

  [   ][ F1 ][ F2 ][ F3 ][ F11][ F12][PgUp][   ][PgDn][   ]
  [   ][ F4 ][ F5 ][ F6 ][   ][← * ][↓ * ][↑ * ][→ * ][ ⌫  ]
  [ALT][ F7 ][ F8 ][ F9 ][ F10][   ][Home][End ][   ][ ↵  ]
  [ ⇧ ][   ][        SPACE        ][SYM]         [ ⇧ ]

  * conflict with SYM — works only when SYM is not active
```

---

## Special Keys

```
  WIN  (Berry)   Mod4 modifier — hold for MOD4 layer, lone tap → META
  MENU           KEY_MENU — back / context menu
  CALL ON        KEY_PHONE — answer call
  CALL OFF       KEY_HANGUP — end call
  MIC            KEY_MICMUTE — mute microphone
  $              dedicated key, produces "$"
  MUT (SYM+$)    KEY_MUTE — audio mute / toggle brightness in meta mode
```

---

## Screen Indicators

Appear in the top bar when a sticky modifier is active:

```
  slot 0  ⇧   Shift
  slot 1  —   (reserved, unused)
  slot 2  C   Ctrl
  slot 3  A   Alt
  slot 4  S   SYM
  slot 5  M   Meta mode  (currently disabled)
  slot 6  T   Touch / trackpad mode
```

---

## Layers Table

```
         Q     W     E     R     T     Y     U     I     O     P
─────────────────────────────────────────────────────────────────
BASE     q     w     e     r     t     y     u     i     o     p
SYM      #     1     2     3     (     )     _     -     +     @
SYM+⇧   {     }     [     ]     <     >     ^     ~     =     |
MOD4           F1    F2    F3    F11   F12

         A     S     D     F     G     H     J     K     L
─────────────────────────────────────────────────────────
BASE     a     s     d     f     g     h     j     k     l
SYM      *     4     5     6     /     :     ;     '     "
SYM+⇧         `     %     \
MOD4           F4    F5    F6          ←     ↓     ↑     →

         Z     X     C     V     B     N     M     $
──────────────────────────────────────────────────────
BASE     z     x     c     v     b     n     m     $
SYM      7     8     9     ?     !     ,     .    mute
SYM+⇧   &     *
MOD4     F7    F8    F9    F10         Home  End
```

---

## Priority Chain

```
  keypress → FW → Touch → Modifiers / SYM → MOD4 → Sway / app
```

Each layer can consume a key — if consumed, the next layers never see it.
SYM runs before MOD4, so **if SYM is active, MOD4 navigation is blocked**
for keys that have a SYM mapping: `H J K L U O N M`

---

## Sticky Modifiers

```
  tap         →  one-shot  (next key only)
  hold        →  continuous
  double-tap  →  locked  (until tapped again)
```

---

## Examples

```
  Sway
    WIN tap          open launcher (META key)
    WIN + 1..9       switch workspace  (MOD4 passed through, unmapped)
    WIN + F/unmapped window management

  Vim / terminal
    MOD4 + H/J/K/L   arrow keys
    MOD4 + U/O        PgUp / PgDn
    MOD4 + W          F1  (help)
    MOD4 + N/M        Home / End

  Text editing
    SYM + H           :   (colon)
    SYM + Q           #
    SYM + G           /
    CTRL tap, then C  Ctrl+C  (one-shot)
    CTRL double-tap   Ctrl locked — all keys get Ctrl until next CTRL tap
```
