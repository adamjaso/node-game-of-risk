"use strict";

console.log(process.env.UV_THREADPOOL_SIZE);
process.env.UV_THREADPOOL_SIZE = 40;

let GameOfRisk = require("./");
let iters = 100000;

module.exports = demo;

function demo() {

    // for (let i = 0; i < 20 ; i++) {
    //     (function(i) {
    //         console.log('running', i)
    //         let b = new GameOfRisk.Battle(i, 10);
    //         b.playAsync(100000, function() {
    //             console.log('finished', i, this);
    //         });
    //     })(i);
    // }

    GameOfRisk.generateDatabase(15, 15, 10000)
        .then(function(results) {
            console.log(results.map(r => r.summary()), 'done');
        });
};

demo();
