"use strict";

const PARALLEL_LIMIT = 20;
process.env.UV_THREADPOOL_SIZE = PARALLEL_LIMIT;

const GameOfRisk = require('./build/Release/GameOfRisk.node');
const async = require('async');

module.exports = GameOfRisk;

GameOfRisk.Battle.prototype.play = function(numPlays) {
    let self = this;
    return new Promise(function(resolve) {
        r.playAsync(self, numPlays, resolve);
    });
}

GameOfRisk.Battle.prototype.summary = function() {
    return {
        numOffense: this.offense.numArmiesOriginal,
        numDefense: this.defense.numArmiesOriginal,
        winOffense: this.offense.numWins,
        winDefense: this.defense.numWins,
        totalArmies: this.offense.numWins + this.defense.numWins,
        lossOffense: this.offense.getLosses(),
        lossDefense: this.defense.getLosses(),
    };
};

GameOfRisk.generateDatabase = function(numOffense, numDefense, iters) {
    return new Promise(function(resolve) {
        let jobs = [];
        for (let i = 2; i <= numOffense; i++) {
            for (let j = 1; j <= numDefense; j++) {
                let battle = new GameOfRisk.Battle(i, j);
                jobs.push(battle);
            }
        }

        async.mapLimit(jobs, PARALLEL_LIMIT, (battle, done) =>
            battle.playAsync(iters, () =>
                done(null, battle)), (err, results) =>
                    resolve(results));
    });
};
