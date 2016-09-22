function OneDArray(x) {
    var ret = []
    for (var i=0; i<x; i++) {
	ret.push(0)
    }
    return ret
}

function TwoDArray(x,y) {
    var ret = []
    for (var i=0; i<x; i++) {
	ret.push(OneDArray(y))
    }
    return ret
}

function plot_sim(arr,canvas_id,pos) {
    var canvas = document.getElementById(canvas_id);
    var ctx = canvas.getContext("2d");

    ctx.drawImage(canvas, -1, 0);
    
    for (i=0; i<arr.length; i++) {
	v = safelog10(arr[i])
	c = Math.floor((1-v)*255);
	ctx.fillStyle = "rgba("+c+","+c+","+c+","+1.0+")";
	ctx.fillRect(499,100-i,1,1);
    }
}

function plot_tradoff(arr,canvas_id) {
    var canvas = document.getElementById(canvas_id);
    var ctx = canvas.getContext("2d");

    ctx.fillStyle = "rgba(255,255,255,1.0)";
    ctx.fillRect(0,0,100,100);
    
    ctx.fillStyle = "rgba(0,0,0,1.0)";

    var min = 9999;
    var max = -9999;
    for (var i=0; i<arr.length; i++) {
    	if (arr[i]<min) min=arr[i];
    	if (arr[i]>max) max=arr[i];
    }
    
    var sc = max-min;

    

    for (var i=0; i<arr.length; i++) {
    	// normalise to fill graph
     	var v = ((arr[i]-min)/sc)
     	ctx.fillRect( i, 100-v*100, 1, 1 );
    }
}

function connect_slider(id,fn) { 
    var _=function() {
	fn($(id).val()/100.0);
	recalc_cost_functions();
    }
    // connect up stuff
    $(id).on('input', _);
    $(id).on('change', _); // IE10
}

connect_slider("#host-curve", function(v) { CH2 = v*15-7.5; });
connect_slider("#parasite-curve", function(v) { CP2 = v*15-7.5; });
