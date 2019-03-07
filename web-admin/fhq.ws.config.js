if(!window.fhq) window.fhq = {};
if(!window.fhq.ws) window.fhq.ws = {};


fhq.ws.base_url = 'ws://' + window.location.hostname + ":1234/api-ws/";

if(window.location.protocol == "https:"){
	fhq.ws.base_url = "wss://" + window.location.hostname + ":4613/api-wss/";
}

if(window.location.hostname == "freehackquest.com"){
	fhq.ws.base_url = 'wss://freehackquest.com/api-wss/';
}
