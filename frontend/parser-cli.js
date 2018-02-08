
var parser = require('./parser')

args = process.argv.slice(2);

if (args.length == 0) {
    console.log("Fatal eggog; No input file");
} else {
    var fs = require('fs');
    fs.readFile(args[0], {encoding: 'utf-8'}, function(err, data) {
        if (!err) {
            var result = parser.parse(data);
            console.log(JSON.stringify(result, null, 2));
        } else {
            console.log(err);
        }
    });
}