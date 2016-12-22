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
    this.max_cat_timer = 150;
    this.cat_timer = 0;
    this.step_time = 0;
    this.last_time = 0;

    this.set_running = function(s) { 
	this.running=s 
	if (s) this.update();
    };

    this.reset_cat_timer = function() {
	this.cat_timer=0;
	$("#catthank").fadeOut("slow");
	$("#catbox").fadeOut("slow");
	$("#catalt").fadeIn("slow");
    }

    this.init = function() {
	range_init(); 
	recalc_cost_functions();
	var xout = TwoDArray(2*N,NEVOL);
	/* Call adaptive dynamics routine (main solver) */
	this.model = new range(xout,_u,_v,_E,_a);
	this.sound.init();
	this.update();
	clear_sim("xcanvas","host");
	clear_sim("ycanvas","parasite");
	clear_sim("icanvas","infection");
	clear_extinct();
	this.reset_cat_timer();
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

	    this.cat_timer++;
	    
	    if (this.cat_timer>this.max_cat_timer) {
		$("#catbox").fadeIn("slow");
		$("#catthank").fadeOut("slow");
		$("#catalt").fadeOut("slow");
	    }

	    if (this.cat_timer%5==0) {
		var time_now = Date.now();
		if (this.last_time!=0) {
		    if (this.step_time==0) {
			this.step_time = (time_now-this.last_time)/1000;
		    } else {
			var st = (time_now-this.last_time)/1000;
			this.step_time = 0.01*st+0.99*this.step_time;

			var steps_left=this.max_cat_timer-this.cat_timer;
			if (steps_left>0) { 
			    $("#cattimer").html(parseInt(this.step_time*steps_left/5));
			}
		    }
		}

		this.last_time = time_now;
	    }

	}
    }
}

function clear_sim(canvas_id,type) {
    var canvas = document.getElementById(canvas_id);
    var ctx = canvas.getContext("2d");
    ctx.fillStyle = "#ffffff";
  
    if (type=="host") {
	ctx.fillStyle = "#9ed9ce";
    }
    if (type=="parasite") {
	ctx.fillStyle = "#dcdc00";
    }

    ctx.fillRect(0,0,500,100);
}

var plot_clock_p=0;
var plot_clock_h=0;

function plot_sim(arr,canvas_id,type) {
    var canvas = document.getElementById(canvas_id);
    var ctx = canvas.getContext("2d");

/*    var clock=plot_clock_p;
    if (type=="host") {
	clock=plot_clock_h;
	plot_clock_h++;
    } else {
	plot_clock_p++;
    }
*/
//    ctx.fillStyle = "red";
//    ctx.fillRect(400,0,100,100);

/*
    ctx.drawImage(canvas, 400, 0, 100, 100, 399, 0, 100, 100);
    if ((clock%2)==0) {
	ctx.drawImage(canvas, 300, 0, 100, 100, 299, 0, 100, 100);
    }
    if ((clock%4)==0) {
	ctx.drawImage(canvas, 200, 0, 100, 100, 199, 0, 100, 100);
    }
    if ((clock%8)==0) {
	ctx.drawImage(canvas, 100, 0, 100, 100, 99, 0, 100, 100);
    }
    if ((clock%8)==16) {
	ctx.drawImage(canvas, -1, 0, 100, 100, 0, 0, 100, 100);
    }
*/
    ctx.drawImage(canvas, -1, 0);
    bgcol = [0xdc,0xdc,0x00];
  
    // select the right colour
    var col = [0xda,0x1b,0xa8];
    if (type=="host") {
	col = [0x4f,0x93,0xa8];
	bgcol = [0x9e,0xd9,0xce];
    }

    col = [0,0,0];

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
    
    v = 100-((sum_x/sum_y)*100);
    ctx.fillStyle = "rgba(0,0,0,1.0)";
    ctx.fillRect(499,v,2,100-v);
}


function plot_tradeoff(arr,canvas_id,type,col,x_label,y_label) {
    var canvas = document.getElementById(canvas_id);
    var ctx = canvas.getContext("2d");

    ctx.fillStyle = "white";
    ctx.fillRect(0,0,100,100);

/*    var min = 9999;
    var max = -9999;
    for (var i=0; i<arr.length; i++) {
    	if (arr[i]<min) min=arr[i];
    	if (arr[i]>max) max=arr[i];
    }
*/
    min=0;
    max=1;
    
    var sc = max-min;

    ctx.fillStyle = "black";
    ctx.fillText(trunc_for_output(min),5,95);
    ctx.fillText(trunc_for_output(max),5,12);
    ctx.fillText(x_label,30,95);

    ctx.save();
    ctx.rotate(-Math.PI/2);
    ctx.fillText(y_label,-75,10);
    ctx.restore();
    
    //ctx.globalCompositeOperation = "xor";

    ctx.strokeStyle = "black";
    ctx.lineWidth=2;
    ctx.beginPath();
    // tradeoffs are inverted
    ctx.moveTo(15, ((arr[0]-min)/sc)*80+5);
    for (var i=1; i<arr.length; i++) {
    	// normalise to fill graph
     	var v = ((arr[i]-min)/sc)
	ctx.lineTo(15+i*0.8, v*80+5);
     	//ctx.fillRect( , 2, 2 );
    }
    ctx.stroke();
    ctx.lineWidth=1;

    ctx.globalCompositeOperation = "source-over";
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

    ctx.lineWidth=2;
    for (var j=0; j<arr.length; j+=10) {
	ctx.beginPath();
	ctx.moveTo(0, 100-((arr[0][j]-min)/sc)*100);
	for (var i=1; i<arr.length; i++) {
    	    // normalise to fill graph
     	    var v = ((arr[i][j]-min)/sc)
     	    //ctx.fillRect( i, 100-v*100, 2, 2 );
	    ctx.lineTo(i, 100-v*100);
	}
	ctx.stroke();
    }
    ctx.lineWidth=1;
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
	sim.reset_cat_timer();
    }
    // connect up stuff
    $(id).on('input', _);
    $(id).on('change', _); // IE10
}

function connect_checkbox(id,fn) { 
    var _=function() {
	fn($(id).prop("checked"));
	recalc_cost_functions();
	sim.reset_cat_timer();
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

function randomise_int_value(id) {
    if (Math.random()<0.5) {
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

function randomise_float_value(id) {
    $(id).val(Math.random()*100);
    $(id).change();
}

function trunc_for_output(v) {
    v=parseInt(v*100);
    v/=100;
    return v;
}

function update_slider_num(id,v) {
    $(id+"-num").html(trunc_for_output(v));
}

////////////////////////////////////////////////////////
// event callbacks here...

var text_col = "#777244";

function button_on(id,text) {
    $(id).html(text);
    //$(id).css('background','black');
    //$(id).css('color','white');
}

function button_off(id,text) {
    $(id).html(text);
    //$(id).css('background','white');
    //$(id).css('color',text_col);
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
    sim.set_running(true);
}

function show_host_extinct() {
    $("#host-extinct").show("slow");
    sim.set_running(false);
}

function show_parasite_extinct() {
    $("#parasite-extinct").show("slow");
    sim.set_running(false);
}

function clear_extinct() {
    $("#parasite-extinct").hide("slow");
    $("#host-extinct").hide("slow");
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

    var save_transmission_type = 0;
    if (transmission_type=="virulence") save_transmission_type=1;

    var save_transmission_function = 0;
    if (transmission_function=="range") save_transmission_function=1;
    if (transmission_function=="matching") save_transmission_function=2;

    $.post("/save_livesim/", {
        created_date: datetime,
	base_name: "test",
	host_img_data: xdata,
	parasite_img_data: ydata,
	status: 0,

	param_host_cost: get_float_value("#host-cost"),
	param_host_recovery: get_float_value("#host-recovery"),
	param_host_curve: get_float_value("#host-curve"),
	param_transmission_type: save_transmission_function,
	param_transmission_shape1: get_float_value("#transmission-shape1"),
	param_transmission_shape2: get_float_value("#transmission-shape2"),
	param_parasite_cost: get_float_value("#parasite-cost"),
	param_parasite_curve: get_float_value("#parasite-curve"),
	param_parasite_mortality: get_float_value("#parasite-mortality"),
	param_parasite_transmission: get_float_value("#parasite-transmission"),
	param_parasite_sterility: get_float_value("#parasite-sterility"),
	param_parasite_type: save_transmission_type,

	cat_host_cycling: get_int_value("#cat-h-cycling"),
	cat_host_single: get_int_value("#cat-h-single"),
	cat_host_many: get_int_value("#cat-h-many"),
	cat_host_strange: get_int_value("#cat-h-strange"),
	cat_parasite_cycling: get_int_value("#cat-p-cycling"),
	cat_parasite_single: get_int_value("#cat-p-single"),
	cat_parasite_many: get_int_value("#cat-p-many"),
	cat_parasite_strange: get_int_value("#cat-p-strange"),
    });

    sim.cat_timer=0;
    $("#catbox").fadeOut("slow");
    $("#catthank").fadeIn("slow");


}

// and back in...
function load_sim(data) {
    transmission_type = "transmission";
    if (data.param_parasite_type==1) transmission_type="virulence";
    set_int_value("#function-type-"+transmission_type,1);
    if (transmission_type=="transmission") {
	set_int_value("#parasite-type-trans",1);
    } else {
	set_int_value("#parasite-type-vir",1);
    }

    transmission_function = "universal";
    if (data.param_transmission_type==1) transmission_function="range";
    if (data.param_transmission_type==2) transmission_function="matching";
    set_int_value("#function-type-"+transmission_function,1);
    
    set_float_value("#host-curve",data.param_host_cost);    
    set_float_value("#host-cost",data.param_host_cost);
    set_float_value("#host-recovery",data.param_host_recovery);
    set_float_value("#host-curve",data.param_host_curve);
    set_float_value("#transmission-shape1",data.param_transmission_shape1);
    set_float_value("#transmission-shape2",data.param_transmission_shape2);
    set_float_value("#parasite-cost",data.param_parasite_cost);
    set_float_value("#parasite-curve",data.param_parasite_curve);
    set_float_value("#parasite-mortality",data.param_parasite_mortality);
    set_float_value("#parasite-transmission",data.param_parasite_transmission);
    set_float_value("#parasite-sterility",data.param_parasite_sterility);
	    
    set_int_value("#cat-h-cycling",data.cat_host_cycling);
    set_int_value("#cat-h-single",data.cat_host_single);
    set_int_value("#cat-h-many",data.cat_host_many);
    set_int_value("#cat-h-strange",data.cat_host_strange);
    set_int_value("#cat-p-cycling",data.cat_parasite_cycling);
    set_int_value("#cat-p-single",data.cat_parasite_single);
    set_int_value("#cat-p-many",data.cat_parasite_many);
    set_int_value("#cat-p-strange",data.cat_parasite_strange);

    sim.init();
}

function randomise_sim() {
    transmission_type = "transmission";
    if (Math.random()<0.5) transmission_type="virulence";
    if (transmission_type=="transmission") {
	set_int_value("#parasite-type-trans",1);
    } else {
	set_int_value("#parasite-type-vir",1);
    }
    
    transmission_function = "universal";
    var t=Math.random();
    if (t<0.33) transmission_function="range";
    if (t>0.66) transmission_function="matching";    
    set_int_value("#function-type-"+transmission_function,1);

    randomise_float_value("#host-curve");
    randomise_float_value("#host-cost");
    randomise_float_value("#host-recovery");
    randomise_float_value("#host-curve");
    randomise_float_value("#transmission-shape1");
    randomise_float_value("#transmission-shape2");
    randomise_float_value("#parasite-cost");
    randomise_float_value("#parasite-curve");
    randomise_float_value("#parasite-mortality");
    randomise_float_value("#parasite-transmission");
    randomise_float_value("#parasite-sterility");
    
    set_int_value("#cat-h-cycling",0);
    set_int_value("#cat-h-single",0);
    set_int_value("#cat-h-many",0);
    set_int_value("#cat-h-strange",0);
    set_int_value("#cat-p-cycling",0);
    set_int_value("#cat-p-single",0);
    set_int_value("#cat-p-many",0);
    set_int_value("#cat-p-strange",0);

    sim.init();
    sim.set_running(true);
}


sim = new sim_handler();
sim.init();

sound = new sound_handler();
sound.init();

