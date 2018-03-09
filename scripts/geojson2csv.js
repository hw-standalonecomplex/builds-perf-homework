var fs = require('fs');
var es = require('event-stream');
var centroid = require('@turf/centroid').default;

if (!process.argv[2] && !fs.existsSync(process.argv[2])) {
    console.error('please provide a path to a line delimited geojson file');
    process.exit(-1);
}
var file_input = process.argv[2];
var s = fs.createReadStream(file_input);

s.pipe(es.split())
    .pipe(es.mapSync(function(line){
        // strip off the RS (0x1e, record separator) per RFC8142 
        var data = line.slice(1);
        if (data) {
            var geojson_feature = JSON.parse(data);
            var geom = geojson_feature.geometry;
            // generate centroids for all features
            var new_feature = centroid(geojson_feature.geometry);
            // dump CSV line of lon,lat,name
            console.log(new_feature.geometry.coordinates[0]+","+new_feature.geometry.coordinates[1]+','+geojson_feature.properties['name'])
        }
    })
    .on('error', function(err){
        console.log('Error while reading file.', err);
        process.exit(-1)
    })
);