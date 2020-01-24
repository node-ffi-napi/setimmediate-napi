'use strict';
const uvloop = require('get-uv-event-loop-napi-h');
const symbol = require('get-symbol-from-current-process-h');
const path = require('path');
exports.includeRaw = [path.resolve(__dirname, 'include')].concat(uvloop.includeRaw).concat(symbol.includeRaw);
exports.include = exports.includeRaw.map(x => `"${x}"`.replace(/\\/g, '\\\\')).join(' ');
exports.gyp = uvloop.gyp;
