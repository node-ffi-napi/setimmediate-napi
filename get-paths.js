'use strict';
const uvloop = require('get-uv-event-loop-napi-h');
const path = require('path');
exports.include = path.resolve(__dirname, 'include') + ' ' + uvloop.include;
exports.gyp = uvloop.gyp;
