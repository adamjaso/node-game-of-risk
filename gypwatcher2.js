"use strict";

const Path = require('path');
const ChildProcess = require('child_process');
const ch = require('chokidar');

let isRunning = false;
let watcher = ch.watch(['./**/*.cc', './**/*.h', './**/*.cpp'], {ignoreInitial: false});

watcher.on('change', function(file) {
    file = Path.join(process.cwd(), file);
    if (isRunning) {
        return;
    }

    isRunning = true;

    let proc = ChildProcess.spawn('/usr/local/lib/node_modules/node-gyp/bin/node-gyp.js', ['rebuild'], {cwd: process.cwd()});
    proc.stdout.pipe(process.stdout);
    proc.stderr.pipe(process.stderr);
    proc.on('exit', function() {
        console.log('node-gyp complete');
        try {
             require('./demo')();
        } catch (e) {
            console.error(e);
        }

        isRunning = false;
    });
});
