if(!window.fhq) window.fhq = {};
if(!fhq.pages) fhq.pages = [];


fhq.deleteQuest = function(el){
	var admin_password = $('#quest_delete_admin_password').val();
	var questid = $(el).attr('questid');
	console.log("questid: " + questid);
	var data = {};
	data.questid = parseInt(questid,10);
	data.admin_password = admin_password;
	console.log(data);

	fhq.ws.quest_delete(data).done(function(r){
		$('#modalInfo').modal('hide');
		fhq.pages['quests']();
	}).fail(function(r){
		$('#quest_delete_error').show();
		$('#quest_delete_error').html(r.error);
	});
}

fhq.exportQuest = function(questid){
	fhq.showLoader();
	fhq.ws.quest_export({questid: questid}).done(function(r){
		fhq.hideLoader();
		var a = window.document.createElement('a');
		a.href = 'data:application/octet-stream;base64,'  + r.data.zipfile_base64;
		a.download = r.data.zipfile_name;
		// Append anchor to body.
		document.body.appendChild(a);
		a.click();
		// Remove anchor from body
		document.body.removeChild(a);
	}).fail(function(err){
		fhq.hideLoader();
		console.error(err);
	});
}

fhq.pages['quests'] = function(){
	window.fhq.changeLocationState({'quests':''});
	fhq.showLoader();
	$('#page_name').html('<div class="header-name">Quests</div>'
		+ '<div class="header-btn" id="quest_create"><i class="fa fa-plus"></i></div>'
		+ '<div class="header-btn" id="quest_import"><i class="fa fa-upload"></i></div>'
	);

	$('#quest_create').unbind().bind('click', fhq.pages['quest_create']);
	$('#quest_import').unbind().bind('click', function(){
		alert("TODO");
	});

	var el = $("#page_content");
	el.html('Loading...')
	
	var onpage = 5;
	if(fhq.containsPageParam("onpage")){
		onpage = parseInt(fhq.pageParams['onpage'], 10);
	}

	var page = 0;
	if(fhq.containsPageParam("page")){
		page = parseInt(fhq.pageParams['page'], 10);
	}
	
	
	
	window.fhq.changeLocationState({'quests': '', 'onpage': onpage, 'page': page});

	fhq.ws.quests({'onpage': onpage, 'page': page}).done(function(r){
		fhq.hideLoader();
		console.log(r);
		el.html('');
		
				
		el.append('<hr>');

		// el.append(fhq.paginator(0, r.count, r.onpage, r.page));
		el.append('<table class="table table-striped">'
			+ '		<thead>'
			+ '			<tr>'
			+ '				<th>#</th>'
			+ '				<th>Info</th>'
			+ '				<th>State</th>'
			+ '				<th>Actions</th>'
			+ '			</tr>'
			+ '		</thead>'
			+ '		<tbody id="list">'
			+ '		</tbody>'
			+ '</table>'
		)


		for(var i in r.data){
			var q = r.data[i];
			$('#list').append('<tr>'
				+ '	<td>' + q.questid + '</td>'
				+ '	<td>' 
				+ "		<p>GameID: " + q.gameid+ "<p>"
				+ "		<p>Name: " + q.name+ "<p>"
				+ "		<p>Subject: " + q.subject+ "<p>"
				+ "		<p>Score: +" + q.score+ "<p>"
				+ "		<p>Solved: " + q.solved + " user(s)<p>"
				+ '</td>'
				+ '	<td>' + q.state + '</td>'
				+ '	<td>'
				+ '			<div class="tbl-action-btn quest-remove" questid="' + q.questid + '"><i class="fa fa-trash"></i></div>'
				+ '			<div class="tbl-action-btn quest-export" questid="' + q.questid + '"><i class="fa fa-download"></i></div>'
				+ '		<p><button class="btn btn-secondary quest-edit" questid="' + q.questid + '">Edit</button></p>'
				+ '</td>'
				+ '</tr>'
			);
		}
		
		/*$('.game-edit').unbind().bind('click', function(){
			var gameuuid = $(this).attr('gameuuid');
			fhq.pages['game_edit'](gameuuid);
		});
		*/

		$('.quest-export').unbind().bind('click', function(){
			var questid = $(this).attr('questid');
			fhq.exportQuest(questid);
		})

		$('.quest-remove').unbind().bind('click', function(){
			console.warn('quest_remove');
			var questid = $(this).attr('questid');
			
			$('#modalInfoTitle').html('Quest {' + questid + '} confirm deletion');
			$('#modalInfoBody').html('');
			$('#modalInfoBody').append(''
				+ 'Admin password:'
				+ '<input class="form-control" id="quest_delete_admin_password" type="password"><br>'
				+ '<div class=" alert alert-danger" style="display: none" id="quest_delete_error"></div>'
			);
			$('#modalInfoButtons').html(''
				+ '<button type="button" class="btn btn-secondary" id="quest_delet_btn" questid="' + questid + '" onclick="fhq.deleteQuest(this);">Delete</button> '
				+ '<button type="button" class="btn btn-secondary" data-dismiss="modal">Close</button>'
			);
			$('#modalInfo').modal('show');
		});
	}).fail(function(r){
		fhq.hideLoader();
		console.error(r);
		el.append(r.error);
	})
}

fhq.pages['quest_create'] = function(){
	$('#page_name').html('<div class="header-name">Quest Create</div>'
		+ '<div class="header-btn" id="quest_create"><i class="fa fa-check"></i></div>'
		+ '<div class="header-btn" id="quest_create_close"><i class="fa fa-times"></i></div>'
	);

	$('#quest_create').unbind().bind('click', fhq.createQuest);
	$('#quest_create_close').unbind().bind('click', fhq.pages['quests']);
	
	var el = $('#page_content');
	el.html('');

	window.fhq.changeLocationState({'quest_create':''});
	fhq.hideLoader();
	
	el.html('');
	el.html(''
		+ '<div class="card">'
		+ '		<div class="card-header">' + fhq.t('Create Quest') + '</div>'
		+ '		<div class="card-body">'
		+ '			<div class="form-group row">'
		+ '				<label for="newquest_quest_uuid" class="col-sm-2 col-form-label">' + fhq.t('UUID') + '</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<input type="text" class="form-control" value="' + fhq.guid() + '" id="newquest_quest_uuid">'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="newquest_gameid" class="col-sm-2 col-form-label">' + fhq.t('Game') + '</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<select class="form-control" id="newquest_gameid"></select>'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="newquest_name" class="col-sm-2 col-form-label">' + fhq.t('Name') + '</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<input type="text" class="form-control" value="" id="newquest_name">'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="newquest_text" class="col-sm-2 col-form-label">' + fhq.t('Text') + ' (Use markdown format)</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<textarea type="text" class="form-control" style="height: 150px" value="" id="newquest_text"></textarea>'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="newquest_score" class="col-sm-2 col-form-label">' + fhq.t('Score') + ' (+)</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<input type="number" class="form-control" id="newquest_score">'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="newquest_subject" class="col-sm-2 col-form-label">' + fhq.t('Subject') + '</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<select class="form-control" value="" id="newquest_subject">'
		+ '						<option value="trivia">Trivia</option>'
		+ '						<option value="hashes">Hashes</option>'
		+ '						<option value="stego">Stego</option>'
		+ '						<option value="reverse">Reverse</option>'
		+ '						<option value="recon">Recon</option>'
		+ '						<option value="crypto">Crypto</option>'
		+ '						<option value="forensics">Forensics</option>'
		+ '						<option value="network">Network</option>'
		+ '						<option value="web">Web</option>'
		+ '						<option value="ppc">PPC</option>'
		+ '						<option value="admin">Admin</option>'
		+ '						<option value="enjoy">Enjoy</option>'
		+ '						<option value="unknown">Unknown</option>'
		+ '					</select>'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="newquest_answer" class="col-sm-2 col-form-label">' + fhq.t('Answer') + '</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<input type="text" class="form-control" id="newquest_answer" value="">'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="newquest_answerformat" class="col-sm-2 col-form-label">' + fhq.t('Answer format') + '</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<input type="text" class="form-control" id="newquest_answerformat" value="">'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="newquest_author" class="col-sm-2 col-form-label">' + fhq.t('Author') + '</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<input type="text" class="form-control" value="" id="newquest_author">'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="newquest_copyright" class="col-sm-2 col-form-label">' + fhq.t('Copyright') + '</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<input type="text" class="form-control" value="" id="newquest_copyright">'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="newquest_state" class="col-sm-2 col-form-label">' + fhq.t('State') + '</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<select class="form-control" value="" id="newquest_state">'
		+ '						<option value="open">Open</option>'
		+ '						<option value="closed">Closed</option>'
		+ '						<option value="broken">Broken</option>'
		+ '					</select>'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="newquest_description_state" class="col-sm-2 col-form-label">' + fhq.t('Description State') + '</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<textarea type="text" class="form-control" style="height: 150px" value="" id="newquest_description_state"></textarea>'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label class="col-sm-2 col-form-label"></label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<div id="quest_create_error" class="alert alert-danger" style="display: none"></div>'
		+ '				</div>'
		+ '			</div>'
		+ '		</div>'
		+ '</div>'
	);
	
	fhq.ws.games().done(function(r){
		for(var i in r.data){
			$('#newquest_gameid').append('<option value="' + r.data[i]["id"] + '">' + r.data[i]["title"] + '</option>');
		}
	})
}

fhq.createQuest = function() {
	fhq.showLoader();
	var params = {};
	params["uuid"] = $("#newquest_quest_uuid").val();
	params["gameid"] = parseInt($("#newquest_gameid").val(),10);
	params["name"] = $("#newquest_name").val();
	params["text"] = $("#newquest_text").val();
	params["score"] = parseInt($("#newquest_score").val(),10);
	params["subject"] = $("#newquest_subject").val();
	params["copyright"] = $("#newquest_copyright").val();
	params["author"] = $("#newquest_author").val();
	params["answer"] = $("#newquest_answer").val();
	params["answer_format"] = $("#newquest_answerformat").val();
	params["state"] = $("#newquest_state").val();
	params["description_state"] = $("#newquest_description_state").val();

	fhq.ws.createquest(params).done(function(r){
		fhq.hideLoader();
		// fhq.loadQuest(r.questid);
		fhq.pages['quests']();
	}).fail(function(err){
		fhq.hideLoader();
		console.error(err);
		$('#quest_create_error').show();
		$('#quest_create_error').html(err.error);
		
	});
};
