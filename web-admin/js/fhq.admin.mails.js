if(!window.fhq) window.fhq = {};
if(!fhq.pages) fhq.pages = [];


fhq.mailSend = function()  {
	fhq.showLoader();
	$('#error_info').hide();
	var data = {};
	data["to"] = $("#mail_to").val();
	data["subject"] = $("#mail_subject").val();
	data["body"] = $("#mail_body").val();
	
	fhq.ws.mail_send(data).done(function(r){
		fhq.hideLoader();
		fhq.pages['mails']();
	}).fail(function(err){
		fhq.hideLoader();
		console.error(err);
		$('#error_info').show();
		$('#error_info .alert').html('ERROR: ' + err.error);
		
	})
};


fhq.pages['mail_send'] = function(){
	fhq.changeLocationState({'mail_send':''});
	$('#page_name').html('Mail Send');
	var el = $('#page_content');
	fhq.hideLoader();
	el.html(''
		+ '<div class="card">'
		+ '		<div class="card-header">New mail</div>'
		+ '		<div class="card-body">'
		+ '			<div class="form-group row">'
		+ '				<label for="mail_to" class="col-sm-2 col-form-label">To</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<input type="email" class="form-control" value="" id="mail_to">'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="mail_subject" class="col-sm-2 col-form-label">Subject</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<input type="text" class="form-control" value="" id="mail_subject">'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="mail_body" class="col-sm-2 col-form-label">Body</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<textarea class="form-control" style="height: 220px;" id="mail_body"></textarea>'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label class="col-sm-2 col-form-label"></label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<div class="btn btn-secondary" onclick="fhq.mailSend();">Send</div>'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row" id="error_info" style="display: none">'
		+ '				<label class="col-sm-2 col-form-label"></label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<div class="alert alert-danger"></div>'
		+ '				</div>'
		+ '			</div>'
		+ '		</div>'
		+ '</div>'
	);
}

fhq.pages['mails'] = function(){
	$('#page_name').html('Mails');
	$('#page_content').html('');
	fhq.showLoader();
	
	var onpage = 5;
	if(fhq.containsPageParam("onpage")){
		onpage = parseInt(fhq.pageParams['onpage'], 10);
	}

	var page = 0;
	if(fhq.containsPageParam("page")){
		page = parseInt(fhq.pageParams['page'], 10);
	}
	
	var el = $("#page_content");
	el.html('Loading...')
	
	window.fhq.changeLocationState({'mails': '', 'onpage': onpage, 'page': page});

	fhq.ws.mails_list({'onpage': onpage, 'page': page}).done(function(r){
		fhq.hideLoader();
		console.log(r);
		el.html('');
		
		el.append('<button id="mail_send" class="btn btn-secondary">Mail Send</button><hr>');
		$('#mail_send').unbind().bind('click', fhq.pages['mail_send']);
		
		
		
		el.append(fhq.paginator(0, r.count, r.onpage, r.page));
		el.append('<table class="table table-striped">'
			+ '		<thead>'
			+ '			<tr>'
			+ '				<th>#</th>'
			+ '				<th>To Email <br> Subject</th>'
			+ '				<th>Message</th>'
			+ '				<th>Datetime <br> Status <br> Priority </th>'
			+ '			</tr>'
			+ '		</thead>'
			+ '		<tbody id="users_list">'
			+ '		</tbody>'
			+ '</table>'
		)
		for(var i in r.data){
			var em = r.data[i];
			$('#users_list').append('<tr>'
				+ '	<td>' + em.id + '</td>'
				+ '	<td><p>' + em.email + '</p><p>'  + em.subject + '</p></td>'
				+ '	<td><pre>' + em.message + '</pre></td>'
				+ '	<td><p>' + em.dt + '</p><p>Status: ' + em.status + '</p><p>Priority: ' + em.priority + ' </p></td>'
				+ '</tr>'
			)
		}
	}).fail(function(r){
		fhq.hideLoader();
		console.error(r);
		el.append(r.error);
	})
}
