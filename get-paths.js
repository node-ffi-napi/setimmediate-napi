'use strict';
const uvloop = require('get-uv-event-loop-napi-h');
const path = require('path');
exports.includeRaw = path.resolve(__dirname, 'include') + ' ' + uvloop.includeRaw;
exports.include = exports.includeRaw.replace(/\\/g, '\\\\');
exports.gyp = uvloop.gyp;
