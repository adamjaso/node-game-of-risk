"use strict";

console.log(process.env.UV_THREADPOOL_SIZE);
process.env.UV_THREADPOOL_SIZE = 40;

let r = require("./");
let iters = 100000;

module.exports = demo;

function demo() {

    // console.log(r.random(), r, new r.Battle(8,8));
    // let b = new r.Battle(8,8);
    //
    // let p = new r.Player('Adam', 3, 8);
    // console.log(p);

    // r.Battle.prototype.play = function(numPlays) {
    //     let self = this;
    //     return new Promise(function(resolve) {
    //         r.playAsync(self, numPlays, resolve);
    //     });
    // }

    for (let i = 0; i < 20 ; i++) {
        (function(i) {
            console.log('running', i)
            let b = new r.Battle(i, 10);
            b.playAsync(100000, function() {
                console.log('finished', i, this);
            });
        })(i);
    }

};

demo();
