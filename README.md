# Game of Risk Node.js Addon

For the Game Risk, this module runs a battle for N iterations in C++. It's also my first Node.js Addon.

# API

```
GameOfRisk = require('game-of-risk')

class GameOfRisk.Battle(numOffense: Integer, numDefense: Integer)
    numDefense: Integer
    numOffense: Integer
    numDefenseRemaining: Integer
    numOffenseRemaining: Integer
    numOffenseWins: Integer
    numDefenseWins: Integer
    offense: GameOfRisk.Player
    defense: GameOfRisk.Player
    play(nTimes: Integer): Promise
    playAsync(nTimes: Integer, callback: Function)
    playSync(nTimes: Integer)
    reset()
    summary(): Object

class GameOfRisk.Player(name: String, numDice: Integer, numArmies: Integer)
    numArmiesOriginal: Integer
    numArmies: Integer
    numWins: Integer,
    dice: Array<Integer>
    getLosses(): Stats
    reset()
    roll()

class GameOfRisk.Stats(name: String, numDecimals: Integer)
    average: Double
    stdev: Double
    sum: Double
    count: Integer
    add(num: Double)

GameOfRisk.generateDatabase(numOffense: Integer, numDefense: Integer): Function
```

# Usage

`npm start` will run `./gypwatcher.js` to start watching changes to all C++ files.

`node demo` will run `require('./').generateDatabase()`.

`node winstats` will generate an SQLite3 Database in `./winstats.sqlite` containing
the winning results for the # offense vs # defense used in `./winstats.js`.
