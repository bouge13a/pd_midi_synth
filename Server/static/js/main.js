$(document).ready( function() {

	ws.onopen = function() {

	}

	ws.onclose = function() {

	}

	// Handle what happens when a message is received from the server.
	ws.onmessage = function(evt) {
	    let msg_dict = JSON.parse(evt.data);

	    console.log(msg_dict)


	}
}