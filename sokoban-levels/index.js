
var sokobanGenerator = require("sokoban-generator");
var fs = require('fs');

for(let i = 4001; i <= 6000; i++)
{
    const opts = {
        width: 8, // the width of the sokoban grid 
        height: 8, // the height of the sokoban grid
        boxes: 4, // the boxes on the grid
        minWalls: 20, // the minimum number of walls on the grid
        attempts: 15000, // when generating the map, the maximum attempts
        seed: Date.now(), // map seed. See note below
        initialPosition: { // The initial position of player
          x: 5,
          y: 5
        },
        type: "class" // the return type, either "string" or "class" 
    }
    
    console.log("Generating: ", i);
    let level = sokobanGenerator.generateSokobanLevel(opts);

    if(level == null || !level._data || !level._data._data)
    {
        let name = "levels3/level" + i.toString().padStart(4, "0") + "-empty.txt";
        fs.writeFileSync(name, ""); /*, (err) => {
            if(err)
                console.error("\tSaving error on ", name, " : ", err);
            console.log("File saved failure");
        });*/
    }
    else {
        let l1 = "##########\n";
        for(let y of level._data._data)
        {
            l1 += "#"
            for(let x of y)
                l1 += x;
            l1 += "#\n";
        }
        l1 += "##########\n";
        let name = "levels3/level" + i.toString().padStart(4, "0") + ".txt";
        fs.writeFileSync(name, l1); /*, (err) => {
            if(err)
                console.error("\tSaving error on ", name, " : ", err);
            console.log("File saved success");
        });*/
    }
}