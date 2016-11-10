// Red King Sonification Copyright (C) 2016-2017 FoAM Kernow
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///////////////////////////////////////////////////////////////////////////

function sim_handler() {
    this.running = true;

    this.sound = new sound_handler();

    this.set_running = function(s) { 
	this.running=s 
	if (s) this.update();
    };

    this.init = function() {
	range_init(); 
	recalc_cost_functions();
	var xout = TwoDArray(2*N,NEVOL);
	/* Call adaptive dynamics routine (main solver) */
	this.model = new range(xout,_u,_v,_E,_a);
	this.sound.init();
	this.update();
	console.log("hello");
	clear_sim("xcanvas","host");
	clear_sim("ycanvas","parasite");
    }

    this.update = function() {
	if (this.running) {
	    this.model.run();

	    plot_sim(this.model.get_host(),"xcanvas","host");
	    plot_sim(this.model.get_parasite(),"ycanvas","parasite");

	    this.sound.update(this.model.get_host(),
			      this.model.get_parasite());
	    
	    var canvas = document.getElementById("xcanvas");
	    var ctx = canvas.getContext("2d");
	    var that = this;
	    requestAnimFrame(function() { that.update() },ctx);
	}
    }
}

function clear_sim(canvas_id,type) {
    var canvas = document.getElementById(canvas_id);
    var ctx = canvas.getContext("2d");
    var bgcol = [0xff,0xe4,0x60];
    if (type=="host") {
	bgcol = [0xcf,0xff,0xff];
    }
    ctx.fillStyle = bgcol;
    ctx.fillRect(0,0,500,100);
}

function plot_sim(arr,canvas_id,type) {
    var canvas = document.getElementById(canvas_id);
    var ctx = canvas.getContext("2d");

    ctx.drawImage(canvas, -1, 0);
    
    // select the right colour
    var col = [0xff,0x84,0];
    var bgcol = [0xff,0xe4,0x60];
    if (type=="host") {
	col = [0x6f,0xb3,0xc8];
	bgcol = [0xcf,0xff,0xff];
    }

    for (i=0; i<arr.length; i++) {
	v = safelog10(arr[i])
	//c = Math.floor((1-v)*255);
	ctx.fillStyle = "rgba("+(bgcol[0]*(1-v)+v*col[0])+","+
	                        (bgcol[1]*(1-v)+v*col[1])+","+
	                        (bgcol[2]*(1-v)+v*col[2])+","+1.0+")";
	ctx.fillRect(499,100-i,1,1);
    }
}

function plot_tradoff(arr,canvas_id,col) {
    var canvas = document.getElementById(canvas_id);
    var ctx = canvas.getContext("2d");

    ctx.fillStyle = "rgba(255,255,255,1.0)";
    ctx.fillRect(0,0,100,100);
    
    ctx.fillStyle = col;

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
     	ctx.fillRect( i, 100-v*100, 2, 2 );
    }
}

function plot_matrix(arr,canvas_id,col) {
    var canvas = document.getElementById(canvas_id);
    var ctx = canvas.getContext("2d");
    
    ctx.fillStyle = "rgba(255,255,255,1.0)";
    ctx.fillRect(0,0,100,100);
    
    ctx.fillStyle = col;
    
    var min = 9999;
    var max = -9999;
    for (var i=0; i<arr.length; i++) {
	for (var j=0; j<arr.length; j++) {
    	    if (arr[i][j]<min) min=arr[i][j];
    	    if (arr[i][j]>max) max=arr[i][j];
	}
    }
    
    var sc = max-min;
    
    for (var j=0; j<arr.length; j+=10) {
	for (var i=0; i<arr.length; i++) {
    	    // normalise to fill graph
     	    var v = ((arr[i][j]-min)/sc)
     	    ctx.fillRect( i, 100-v*100, 2, 2 );
	}
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

function connect_checkbox(id,fn) { 
    var _=function() {
	fn($(id).prop("checked"));
	recalc_cost_functions();
    }
    // connect up stuff
    $(id).on('change', _); // IE10
}

///////////////////////////////////////////////////////////
// sim callbacks - tweak the globals!

connect_slider("#host-curve", function(v) { CH2 = v*15-7.5; });
connect_slider("#parasite-curve", function(v) { CP2 = v*15-7.5; });

// todo: change CP2 range
connect_checkbox("#parasite-transmission", function(v) { parasite_transmission=v; });

//////////////////////////////////////////////////////////

sim = new sim_handler();
sim.init();

sound = new sound_handler();
sound.init();

////////////////////////////////////////////////////////
// event callbacks here...

var text_col = "#777244";

function button_on(id,text) {
    $(id).html(text);
    $(id).css('background','black');
    $(id).css('color','white');
}

function button_off(id,text) {
    $(id).html(text);
    $(id).css('background','white');
    $(id).css('color',text_col);
}

function button_sim_toggle() {
    if (sim.running) {
	button_off('#sim-button','Sim: off');
	sim.set_running(false);
    } else {
	button_on('#sim-button','Sim: on');
	sim.set_running(true);
    }
}

function button_sound_toggle() {
    if (sim.sound.running) {
	button_off('#sound-button','Sound: off');
	sim.sound.set_running(false);
    } else {
	button_on('#sound-button','Sound: on');
	sim.sound.set_running(true);
    }
}

function button_sim_reset() {
    sim.init();
}

function button_save() {
    var currentdate = new Date(); 
    var datetime = currentdate.getFullYear()+"-"+
	(currentdate.getMonth()+1)+"-"+
	currentdate.getDate()+" "+
        currentdate.getHours()+":"+
	currentdate.getMinutes()+":"+
	currentdate.getSeconds();

    // 2016-11-10 11:43:49+00:00

    $.post("/save_livesim/", {
        created_date: datetime,
	base_name: "test",
	param_host_cost: 999,
	param_parasite_cost: 888
    });
}

button_on('#sim-button','Sim: on');
button_on('#sound-button','Sound: on');

