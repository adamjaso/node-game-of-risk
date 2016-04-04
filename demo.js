"use strict";

let r = require("./");
let iters = 100000;

console.log(r.random(), r, new r.Battle(8,8));
let b = new r.Battle(8,8);

r.Battle.prototype.play = function(numPlays) {
    let self = this;
    return new Promise(function(resolve) {
        r.playAsync(self, numPlays, resolve);
    });
}
// b.setDebug(false);

console.log('first');
b.playSync(iters);
console.log('first2', b);

// b.playAsync(iters, function() {
//     console.log('asdf', b);
// });
//
// r.playAsync(b, iters, function() {
//     console.log('asdf2', b);
// });
//
// b.play(iters).then(function() {
//     console.log('asdf3', b)
// });
