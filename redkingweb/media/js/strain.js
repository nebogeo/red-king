// code to find strains in a simulation time slice
function find_strain_centres(level) {
    var av = 0
    var mx = 0
    // find average and max for this slice
    for (var i=0; i<level.length; i++) {
        av+=level[i]
        if (level[i]>mx) mx=level[i]
    }
    av/=level.length
    av/=10 // threshold the detection
    if (mx==0) mx=1
    // record the positions of any strains above the threshold
    // until they drop below the threshold
    var in_strain = false
    var strain_start = 0
    var strains = []
    for (var i=0; i<level.length; i++) {
        if (!in_strain && level[i]>av) {
            in_strain = true
            strain_start = i
	}
        if (in_strain && level[i]<av) {
            in_strain = false
            strains.push([strain_start,i])
	}
    }
    // find the centres of the strains and record the maximum there
    var strain_centres = []
    for (var s=0; s<strains.length; s++) {
	strain=strains[s]; 
        var strain_max = 0
        var strain_pos = 0
        for (var i=strain[0]; i<strain[1]; i++) {
            if (level[i]>strain_max) {
                strain_max=level[i]
                strain_pos=i
	    }
	}
        strain_centres.push([strain_pos,strain_max])
    }

    return strain_centres
}
