var api = require('./api/api.js');
var www = require('./www/www.js');

www.listen();
api.listen();
