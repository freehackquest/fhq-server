if(!window.fhq) window.fhq = {};
if(!fhq.pages) fhq.pages = [];

fhq.formatUptime = function(t){
	var t_sec = t % 60;
	t = (t - t_sec) / 60;
	var t_min = t % 60;
	t = (t - t_min) / 60;
	var t_hours = t % 24;
	t = (t - t_hours) / 24;
	var t_days = t;
	return t_days + " day(s) " + t_hours + " h " + t_min + " m " + t_sec + " s";
}

fhq.startUptime = function(server_uptime_sec) {
	window["server_uptime_sec"] = server_uptime_sec;
	clearInterval(window["server_uptime_sec_intrl"]);
	window["server_uptime_sec_intrl"] = setInterval(function() {
		if ($('#server_uptime').length > 0) {
			window["server_uptime_sec"]++;
			$('#server_uptime').html('Server uptime: ' + fhq.formatUptime(window["server_uptime_sec"]) + ' sec');
		} else {
			clearInterval(window["server_uptime_sec_intrl"]);
		}
	},1000);
}

fhq.pages['server_info'] = function(){
	$('.nav-link.main-menu').removeClass("active");
	$('#menu_server_info').addClass("active");

	fhq.changeLocationState({'server_info':''});
	console.warn("server_info");
	$('#page_name').html('Server Info');
	var el = $('#page_content');
	fhq.showLoader();
	el.html('');
	fhq.ws.server_info({}).done(function(r) {
		fhq.hideLoader();
		console.log(r);
		var st = r.data.server_started;

		el.append(''
			+ '<div class="card">'
			+ '	<div class="card-header">Server uptime</div>'
			+ '	<div class="card-body">'
			+ ' 	<p>Server started: ' + r.data.server_started + '</p>'
			+ ' 	<p id="server_uptime">Server uptime: ' + fhq.formatUptime(r.data.server_uptime_sec) + ' sec</p>'
			+ '	</div>'
			+ '</div><br>'
		);
		fhq.startUptime(r.data.server_uptime_sec);

		el.append(''
			+ '<div class="card">'
			+ '	<div class="card-header">Requests statistics</div>'
			+ '	<div class="card-body">'
			+ '		<table class="table table-striped">'
			+ '			<thead>'
			+ '				<tr>'
			+ '					<th>Handler</th>'
			+ '					<th>Counter</th>'
			+ '				</tr>'
			+ '			</thead>'
			+ '			<tbody id="request_statistics"></tbody>'
			+ '		</table>'
			+ '	</div>'
			+ '</div><br>'
		);
		
		var rs = r.data.request_statistics;

		for(var i in rs){
			$('#request_statistics').append(''
				+ '	<tr>'
				+ '		<td>' + i + '</td>'
				+ '		<td>' + rs[i] + '</td>'
				+ '	</tr>'
			);
		}
		
		el.append(''
			+ '<div class="card">'
			+ '	<div class="card-header">Last log messages</div>'
			+ '	<div class="card-body">'
			+ '		<table class="table table-striped">'
			+ '			<thead>'
			+ '				<tr>'
			+ '					<th>Message</th>'
			+ '				</tr>'
			+ '			</thead>'
			+ '			<tbody id="last_log_messages"></tbody>'
			+ '		</table>'
			+ '	</div>'
			+ '</div><br>'
		);
		
		msgs = r.data.last_log_messages;
		for(var i in msgs){
			$('#last_log_messages').append(''
				+ '	<tr>'
				+ '		<td>' + msgs[i] + '</td>'
				+ '	</tr>'
			);
		}
	}).fail(function(err){
		fhq.hideLoader();
		console.error(err);
	})
}
