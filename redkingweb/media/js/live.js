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
    this.running = false;

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

    frame = 0;

    this.update = function() {
	if (this.running) {
	    this.model.run();

	    if (frame%3==0) {
		plot_sim(this.model.get_host(),"xcanvas","host");
		plot_sim(this.model.get_parasite(),"ycanvas","parasite");
		plot_infected(this.model.get_host(),this.model.get_parasite(),"icanvas");
	    }
	    frame++;
	    
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

    var col = [0,0,0];
    bgcol = [0xff,0xff,0xff];

    for (i=0; i<arr.length; i++) {
	v = safelog10(arr[i])
	//c = Math.floor((1-v)*255);
	ctx.fillStyle = "rgba("+(bgcol[0]*(1-v)+v*col[0])+","+
	                        (bgcol[1]*(1-v)+v*col[1])+","+
	                        (bgcol[2]*(1-v)+v*col[2])+","+1.0+")";
	ctx.fillRect(499,100-i,1,1);
    }
}

function plot_infected(arrx,arry,canvas_id) {
    var canvas = document.getElementById(canvas_id);
    var ctx = canvas.getContext("2d");

    ctx.drawImage(canvas, -1, 0);
    
    var sum_x=0
    for (var i=0; i<arrx.length; i++) {
	sum_x+=arrx[i];
    }
    var sum_y=0
    for (var i=0; i<arrx.length; i++) {
	sum_y+=arry[i];
    }

    ctx.fillStyle = "white";
    ctx.fillRect(499,0,1,500);

    v = (sum_x/sum_y)*100;
    ctx.fillStyle = "rgba(0,0,0,1.0)";
    ctx.fillRect(499,v,2,100-v);
}


function plot_tradeoff(arr,canvas_id,type,col) {
    var canvas = document.getElementById(canvas_id);
    var ctx = canvas.getContext("2d");

    var bgcol = "rgba(255,228,96,1.0)";
    if (type=="host") {
	bgcol = "rgba(207,255,255,1.0)";
    }
    //ctx.fillStyle = bgcol;
    ctx.fillStyle = "white";
    ctx.fillRect(0,0,100,100);
    
    //ctx.fillStyle = col;
    ctx.fillStyle = "black";

    var min = 9999;
    var max = -9999;
    for (var i=0; i<arr.length; i++) {
    	if (arr[i]<min) min=arr[i];
    	if (arr[i]>max) max=arr[i];
    }

    console.log(min+" "+max);
    
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
    
    var bgcol = "rgba(255,228,96,1.0)";
    //ctx.fillStyle = bgcol;
    ctx.fillStyle = "white";
    ctx.fillRect(0,0,100,100);
    
    ctx.fillStyle = "black";
    
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

function plot_heatmap(arr,canvas_id) {
    var canvas = document.getElementById(canvas_id);
    var ctx = canvas.getContext("2d");
    
    var bgcol = "rgba(255,228,96,1.0)";
    //ctx.fillStyle = bgcol;
    ctx.fillStyle = "white";
    ctx.fillRect(0,0,100,100);
    
    ctx.fillStyle = "black";
    
    var min = 9999;
    var max = -9999;
    for (var i=0; i<arr.length; i++) {
	for (var j=0; j<arr.length; j++) {
    	    if (arr[i][j]<min) min=arr[i][j];
    	    if (arr[i][j]>max) max=arr[i][j];
	}
    }
    
    var sc = max-min;
    
    for (var j=0; j<arr.length; j++) {
	for (var i=0; i<arr.length; i++) {
    	    // normalise to fill graph
     	    var v = ((arr[i][j]-min)/sc)
	    v*=255;
	    v=parseInt(v);
	    ctx.fillStyle = "rgba("+v+","+v+","+v+",1.0)";
     	    ctx.fillRect( i, j, 2, 2 );
	}
    }
}


function connect_slider(id,fn) { 
    var _=function() {
	v=fn($(id).val()/100.0);
	update_slider_num(id,v);
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

function get_int_value(id) {
    if ($(id).prop("checked")) return 1;
    return 0;
}

function set_int_value(id,v) {
    if (v==0) {
	$(id).prop('checked', false);
    } else {
	$(id).prop('checked', true);
    }
    $(id).change();
}

function get_float_value(id) {
    return $(id).val()/100.0;
}

function set_float_value(id,v) {
    $(id).val(v*100);
    $(id).change();
}

function update_slider_num(id,v) {
    v=parseInt(v*100);
    v/=100;
    $(id+"-num").html(v);
}

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

// out
function button_save() {
    var currentdate = new Date(); 
    var datetime = currentdate.getFullYear()+"-"+
	(currentdate.getMonth()+1)+"-"+
	currentdate.getDate()+" "+
        currentdate.getHours()+":"+
	currentdate.getMinutes()+":"+
	currentdate.getSeconds();

    // 2016-11-10 11:43:49+00:00

    var xcanvas = document.getElementById("xcanvas");
    var xdata = xcanvas.toDataURL('image/png');
    var ycanvas = document.getElementById("ycanvas");
    var ydata = ycanvas.toDataURL('image/png');

    $.post("/save_livesim/", {
        created_date: datetime,
	base_name: "test",
	host_img_data: xdata,
	parasite_img_data: ydata,
	param_host_cost: get_float_value("#host-curve"),
	param_parasite_cost: get_float_value("#parasite-curve"),
	cat_host_cycling: get_int_value("#cat-h-cycling"),
	cat_host_single: get_int_value("#cat-h-single"),
	cat_host_many: get_int_value("#cat-h-many"),
	cat_host_strange: get_int_value("#cat-h-strange"),
	cat_parasite_cycling: get_int_value("#cat-p-cycling"),
	cat_parasite_single: get_int_value("#cat-p-single"),
	cat_parasite_many: get_int_value("#cat-p-many"),
	cat_parasite_strange: get_int_value("#cat-p-strange"),
    });
}

// and back in...
function load_sim(data) {
    set_float_value("#host-curve",data.param_host_cost);    
    set_float_value("#parasite-curve",data.param_parasite_cost);    
    set_int_value("#cat-h-cycling",data.cat_host_cycling);
    set_int_value("#cat-h-single",data.cat_host_single);
    set_int_value("#cat-h-many",data.cat_host_many);
    set_int_value("#cat-h-strange",data.cat_host_strange);
    set_int_value("#cat-p-cycling",data.cat_parasite_cycling);
    set_int_value("#cat-p-single",data.cat_parasite_single);
    set_int_value("#cat-p-many",data.cat_parasite_many);
    set_int_value("#cat-p-strange",data.cat_parasite_strange);
}

sim = new sim_handler();
sim.init();

sound = new sound_handler();
sound.init();

