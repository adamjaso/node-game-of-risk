"use strict";

const GameOfRisk = require('./build/Release/GameOfRisk.node');

module.exports = GameOfRisk;

GameOfRisk.Battle.prototype.play = function(numPlays) {
    let self = this;
    return new Promise(function(resolve) {
        r.playAsync(self, numPlays, resolve);
    });
}

GameOfRisk.generateDatabase = function(numOffense, numDefense, iters) {
    for (let i = 2; i <= numOffense; i++) {
        for (let j = 1; j <= numDefense; j++) {
            let b = new GameOfRisk.Battle(i, j);
            // b.play(1000000);
            b.playAsync(iters, function() {
                // console.log(b);
            });
        }
    }
};
