cash-of-clans
=============

- a free re-implementation of a working game economy system.
- detailed system info [here](http://www.deconstructoroffun.com/2012/09/clash-of-clans-winning-formula.html).

### core loop
- collect & farm. wait.
- build & train. wait.
- (battle)
- repeat

### mechanics
- farm.
- build.
- level up.
- relocate facilities.
- deploy attack units.

### secondary mechanics:
- decorate facilities.
- recharge defenses.

### economy basics
- there are two virtual currencies:
  - $ gold: you can buy defense items with it, and upgrade elixir facilities with it.
  - E elixir: you can buy offense items with it, and upgrade gold facilities with it.
  - factories will farm virtual cash during lapse intervals.
  - factories will stop farming once limits are reached (say, 3h) forcing player to rejoin the game.
- there is always one shortcut:
  - gems: can speed up time. trades for real money.

### axes equilibrium (resources/units axis)
- build: - resources + units
- attack: + resources - units

### currency equilibrium (gold/elixir axis)
- build attack: -elixir
- build defense: -gold
- attack player: +elixir
- attack player: +gold

### solo campaign
- super hard
- low earnings
- split screen with multiplayer

### multiplayer matchmaking
- search costs virtual money ($N).
- result is random, but won't ever find same result twice.
- battle benefits are always higher than search costs by a factor of M.
- exit ($0), search again ($N)

### multiplayer battles (simplified)
- before battle: both positive and negative trophy numbers are calculated for each player.
- before battle: a percentage of both elixir and gold is taken from each player into a deposit.
- during battle: attacking units are never returned back. once deployed they're lost forever.
- during battle: some defending units are lost unless recharged.
- battle over: winner gets applied a positive trophy, original deposit backed, all coin+elixir earnings from battle are added, plus coin+elixir league/clan bonus.
- battle over: loser gets applied a negative trophy, original deposit is never returned back, all coin+elixir earnings from battle are substracted.
- battle over: defender can initiate attack next time and won't get revenged back.
- if village is destroyed a time-lapsed protection shield is activated on loser.

#### sample
```
search is $170
percentage is 50%
attacker bonus per victory, 60 according to his league
defender bonus per victory, 110 according to his league
attacker stats: A) lvl1 $1000 E1000
defender stats: B) lvl3 $10000 E10000
deposit for attacker: $610=$1000(50%)+110 E610=E1000(50%)+110
deposit for defender: $5060=$10000(50%)+60 E5060=E10000(50%)+60
attacker stats: A) lvl1 $390 E390
defender stats: B) lvl3 $4940 E4940
battle as seen by attacker: victory +29 defeat -13 wins upto +$5000 +E5000
battle as seen by defender: victory +13 defeat -29 wins +$610 +E610 loses upto -$5060 -E5060

battle scenario #1: A) loses or shutdowns B) receives B.deposit + B.bonus + A.b
battle scenario #1: A) lvl1(-13) $390(+$0) E390(+E0)
battle scenario #1: B) lvl3(+13) $4940(+$5060+$610) E4940(+E5060+E610)
battle scenario #1: A) lvl1 $390 E390 B) lvl3 $10610 E10610 we) +$170

battle scenario #2: A) wins units1(x2) units3(x4) $800 E2000 bonus $60 bonus E60
battle scenario #2: B) loses traps(x2)
battle scenario #2: A) lvl1(+29) $390(+$610+$800+$60) E390(+E610+E2000+E60)
battle scenario #2: B) lvl3(-29) $4940(+$5060-$800-$60) E4940(+E5060-E2000-E60) "A stole you 860 and 2060"
battle scenario #2: A) lvl1 $1860 E3060 B) lvl3 $9140 E7940 we) +$170

battle scenario #3: A) shutdowns
battle scenario #3: B) server fault, cannot move
battle scenario #3: drawn match A) += A.deposit B) += B.deposit we) +$170
```

**to be continued....**

### timing
- seconds (20,30,40,45)
- tens of minutes (2,3,4,6,8,10,15,20,30,45)
- several hours (1,2,3,4,6,8,10,12)
- several days (1,2,3,4,6 ...)

### facilities
- production is parallelized
- production is limited by experience, cash and/or number of free builders.
- canceled orders take 50% money refund.

### units
- production is serialized
- production is limited by experience and/or cash.
- canceled units take 100% money refund.

### ranking
- sorted by: trophies: number of victories at attacking + deffending. defeats do not count.
- grouped by: global and country: both solo players and clans.

### leagues are based by ranking level:
- subgroups: III/II/I subgroups, none/wood/copper/bronze/silver/gold/zaphyr

### earnings
- earn: ranking (maybe), gold (maybe), elixir (maybe).
- lose: ranking (maybe), gold (maybe), elixir (maybe), units (maybe).

### achievements
- earn: gems.

### highlights
- tutorial is 3 minutes.
- typical grown session may be as quick as 25 seconds.
- loading is fast, farming is one tap, building a few touches, exiting is fast.
- shield is on during first three days, then player joins the real game.
- weekly tournaments (for clans).
- revenge attacks.
- join clan by restoring the castle next to their village. contextualized, great way to set up a long-term goal and avoid overwhelming new players, avoid newcomers in clans.

### possibly missing features
- gift items/gems
- interact with (facebook) friends: attack, chat, gift
- invite other players
- lack of world map

### interesting comments
```
"I hate paying for speed ups. I much prefer purchasing non-consumable content
that enhances the game or unlocks new abilities." - Ryan Hoover
```

```
"I agree with you on the too steep progression of the production timers. In
Kabam's games you go from seconds to minutes and from minutes to tens of
minutes, then hour, a couple of hours etc. In Clash of Clans they jump from tens
of minutes to several hours and then quickly to several days. And it really
kills the fun and I bet some retention too.

Nevertheless these are a matter of tweaking and content. When you launch a game
you're forced to have long timers and a lot of waiting/grinding simply because
you don't have enough of content to last. But once you get few updates in and
add that content you can simply reduce the timers and hopefully see increase in
retentions." - Michail Katkoff
```
