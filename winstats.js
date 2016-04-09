"use strict";

const GameOfRisk = require('./');
const sqlite3 = require('sqlite3').verbose();
const knex = require('knex')({ client: 'sqlite3', useNullAsDefault: true });

const DATABASE_FILE = __dirname + '/winstats.sqlite';
const ITERATIONS = 1000000;
const CREATE_SQL = `
CREATE TABLE IF NOT EXISTS winstats (
    num_offense INTEGER NOT NULL,
    num_defense INTEGER NOT NULL,
    win_offense INTEGER NOT NULL,
    win_defense INTEGER NOT NULL,
    total_armies INTEGER NOT NULL,
    loss_count_offense INTEGER NOT NULL,
    loss_average_offense DOUBLE NOT NULL,
    loss_stdev_offense DOUBLE NOT NULL,
    loss_count_defense INTEGER NOT NULL,
    loss_average_defense DOUBLE NOT NULL,
    loss_stdev_defense DOUBLE NOT NULL,
    PRIMARY KEY (num_offense, num_defense)
)`;

// const INSERT_IGNORE = `
// INSERT INTO winstats (num_offense, num_defense, win_offense, win_defense, total_armies, loss_count_offense, loss_average_offense, loss_stdev_offense) VALUES ()`;

module.exports = generate;

function generate() {
    return new Promise((resolve, reject) => {
        let db = new sqlite3.Database(DATABASE_FILE);

        db.run(CREATE_SQL);
        process.on('exit', () => db.close());

        db.serialize(function(err) {
            if (err) {
                return reject(err);
            }

            GameOfRisk.generateDatabase(15, 15, ITERATIONS)
                .then(function(results) {
                    try {

                        let insert;
                        results = results.map(function(battle) {
                            let summary = battle.summary();
                            let q = knex('winstats').insert({
                                num_offense: summary.numOffense,
                                num_defense: summary.numDefense,
                                win_offense: summary.winOffense,
                                win_defense: summary.winDefense,
                                total_armies: summary.totalArmies,
                                loss_count_offense: summary.lossOffense.count,
                                loss_average_offense: summary.lossOffense.average,
                                loss_stdev_offense: summary.lossOffense.stdev,
                                loss_count_defense: summary.lossDefense.count,
                                loss_average_defense: summary.lossDefense.average,
                                loss_stdev_defense: summary.lossDefense.stdev,
                            }).toSQL();

                            if (!insert) {
                                insert = db.prepare(q.sql.replace(/^insert/i, 'replace'));
                            }

                            insert.run(q.bindings);

                            return q;
                        });
                        insert.finalize();

                    } catch (e) {
                        return reject(e);
                    }

                    resolve();
                });
        });
    });
};


generate();
