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


 var requestAnimFrame = (function() {
    return window.requestAnimationFrame ||
    window.webkitRequestAnimationFrame ||
    window.mozRequestAnimationFrame ||
    window.oRequestAnimationFrame ||
    window.msRequestAnimationFrame ||
    function(/* function FrameRequestCallback */ callback, /* DOMElement Element */ element) {
    window.setTimeout(callback, 1000/60);    };
    })();

function load_canvas(id,dataURL) {
    var canvas = document.getElementById(id);
    var context = canvas.getContext('2d');
    var image = new Image();
    image.onload = function() {
        context.drawImage(this, 0, 0);
    };    
    image.src = dataURL;
}
