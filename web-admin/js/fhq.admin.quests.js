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

fhq.pages['quests'] = function() {
	$('.nav-link.main-menu').removeClass("active");
	$('#menu_quests').addClass("active");

	fhq.showLoader();
	$('#page_name').html('Quests');

	var el = $("#page_content");
	el.html('Loading...')
	
	var onpage = 5;
	if (fhq.containsPageParam("onpage")) {
		onpage = parseInt(fhq.pageParams['onpage'], 10);
	}

	var quests_filter = '';
	if (fhq.containsPageParam("filter")) {
		quests_filter = fhq.pageParams['filter'];
	}

	var page = 0;
	if(fhq.containsPageParam("page")){
		page = parseInt(fhq.pageParams['page'], 10);
	}
	
	window.fhq.changeLocationState({
		'quests': '',
		'onpage': onpage,
		'page': page,
		'filter': quests_filter,
	});

	el.html('');
	el.append(''
		+ '<div>'
		+ '   <input id="quests_search" class="form-control" type="text" placeholder="Search...">'
		+ '</div><br>'
	);
	$('#quests_search').val(quests_filter);
	$('#quests_search').unbind().bind('keyup', function(e){
		var new_filter = $('#quests_search').val();
		if (new_filter != quests_filter) {
			window.fhq.changeLocationState({
				'quests': '',
				'onpage': onpage,
				'page': page,
				'filter': new_filter,
			});
			setTimeout(function() {
				if ($('#quests_search').val() == new_filter) {
					fhq.pages['quests']();
				}
			}, 500);
		}
	});
	$('#quests_search').focus();
	el.append(''
		+ '<div class="btn btn-primary" id="quest_create"><i class="fa fa-plus"></i> New quest</div> '
		+ '<div class="btn btn-primary" id="quest_import"><i class="fa fa-upload"></i> Import (TODO)</div>'
	);

	$('#quest_create').unbind().bind('click', fhq.pages['quest_create']);
	$('#quest_import').unbind().bind('click', function(){
		alert("TODO");
	});
			
	el.append('<hr>');

	// el.append(fhq.paginator(0, r.count, r.onpage, r.page));
	el.append('<table class="table table-striped table-hover">'
		+ '		<thead class="thead-dark">'
		+ '			<tr>'
		+ '				<th>#</th>'
		+ '				<th>Game</th>'
		+ '				<th>Subject</th>'
		+ '             <th>Score</th>'
		+ '             <th>Name</th>'
		+ '				<th>State</th>'
		+ '				<th>Solved</th>'
		+ '				<th>Actions</th>'
		+ '			</tr>'
		+ '		</thead>'
		+ '		<tbody id="list">'
		+ '		</tbody>'
		+ '</table>'
	)

	fhq.ws.quests({'onpage': onpage, 'page': page, 'filter': quests_filter}).done(function(r){
		fhq.hideLoader();
		console.log(r);
		

		for(var i in r.data){
			var q = r.data[i];
			var row_color = '';
			if (q.state == "closed") {
				row_color = 'table-warning';
			}

			if (q.state == "broken") {
				row_color = 'table-danger';
			}

			$('#list').append('<tr class="' + row_color + '">'
				+ '	<td>' + q.questid + '</td>'
				+ '	<td>[game#' +  q.gameid + ']</td>'
				+ '	<td>' +  q.subject + '</td>'
				+ '	<td>+' +  q.score + '</td>'
				+ '	<td>' +  q.name + '</td>'
				+ '	<td>' + q.state + '</td>'
				+ '	<td>' + q.solved + ' user(s)</td>'
				+ '	<td>'
				+ '		<div class="btn btn-primary quest-edit" questid="' + q.questid + '">Edit</div>'
				+ '		<div class="btn btn-primary quest-export" questid="' + q.questid + '">Export</div>'
				+ '		<div class="btn btn-secondary quest-remove" questid="' + q.questid + '">Delete</div>'
				+ '</td>'
				+ '</tr>'
			);
		}
		
		$('.quest-edit').unbind().bind('click', function(){
			var questid = $(this).attr('questid');
			fhq.pages['quest_edit'](questid);
		});

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

fhq.pages['quest_create'] = function() {
	$('#page_name').html('Quest Create');
	
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
		+ '<div class="btn btn-danger" id="quest_create"><i class="fa fa-check"></i> Create</div> '
		+ '<div class="btn btn-primary" id="quest_create_close"><i class="fa fa-times"></i> Cancel</div> '
	);

	$('#quest_create').unbind().bind('click', fhq.createQuest);
	$('#quest_create_close').unbind().bind('click', fhq.pages['quests']);

	fhq.ws.games().done(function(r){
		console.log(r);
		for(var i in r.data) {
			$('#newquest_gameid').append('<option value="' + r.data[i]["local_id"] + '">' + r.data[i]["name"] + '</option>');
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
		console.log(r);
		// fhq.loadQuest(r.questid);
		fhq.pages['quests']();
	}).fail(function(err){
		fhq.hideLoader();
		console.error(err);
		$('#quest_create_error').show();
		$('#quest_create_error').html(err.error);
		
	});
};

function quest_edit_menu(el, active_tab, questid) {
	var menu = [{
		'page': 'quest_edit',
		'caption': 'Quest',
	}, {
		'page': 'quest_hints_edit',
		'caption': 'Hints',
	}, {
		'page': 'quest_files_edit',
		'caption': 'Files',
	}, {
		'page': 'quest_writeups_edit',
		'caption': 'Write Ups',
	}, {
		'page': 'quest_statistics_edit',
		'caption': 'Statistics',
	}];
	var html = '<ul class="nav nav-tabs">';
	for (var i in menu) {
		var m = menu[i];
		html += ''
			+ '  <li class="nav-item">'
			+ '    <div class="quest-menu-btn nav-link ' + (active_tab == m.page ? 'active' : '') + '" '
			+ '      page="' + m.page + '">' + m.caption + '</div>'
			+ '  </li>'
	}
	html += '</ul>';
	el.append(html);
	$('.quest-menu-btn').unbind().bind('click', function(){
		var page = $(this).attr('page');
		fhq.pages[page](questid);
	});
}


fhq.pages['quest_hints_edit'] = function(questid) {
	questid = questid || fhq.pageParams['quest_hints_edit'];
	if (questid) {
		questid = parseInt(questid, 10);
	}
	$('#page_name').html('Quest Hints Edit [quest#' + questid + ']');
	var el = $('#page_content');
	el.html('');
	console.log("questid: ", questid);
	window.fhq.changeLocationState({'quest_hints_edit':questid});
	fhq.hideLoader();
	
	el.html('');
	quest_edit_menu(el, 'quest_hints_edit', questid);
	el.append(''
		+ '<div class="card">'
		+ '		<div class="card-body">'
		+ '			<table class="table table-striped table-hover">'
		+ '				<thead class="thead-dark"><tr><th>#</th><th>Text</th><th>Actions</th></tr></thead>'
		+ '				<tbody id="quest_hints"></tbody>'
		+ '			</table>'
		+ '         <div class="btn btn-primary" id="add_hint" questid="' + questid + '">Add Hint</div>'
		+ '		</div>'
		+ '</div>'
	);
	$('#add_hint').unbind().bind('click', fhq.addQuestHintForm);

	fhq.ws.quest({"questid": questid}).done(function(data){
		var q = data.quest;
		var hints = data.hints;
		for (var i in hints) {
			var h = hints[i];
			console.log(h)
			$('#quest_hints').append('<tr>'
				+ '<td>[hint#' + h.id + ']</td>'
				+ '<td>' + h.text + '</td>'
				+ '<td><div class="btn btn-danger delete-hint" '
				+ '         questid="' + questid + '" hintid="' + h.id + '" >Delete Hint</div></td>'
				+ '</tr>');
		}
		$('.delete-hint').unbind().bind('click', fhq.deleteHint);
	}).fail(function(err){
		console.error(err);
	})
}

fhq.deleteHint = function() {
	var hintid = $(this).attr('hintid');
	hintid = parseInt(hintid,10);
	var questid = $(this).attr('questid');
	questid = parseInt(questid,10);

	fhq.showLoader();
	fhq.ws.deletehint({"hintid": hintid}).done(function(r){
		fhq.hideLoader();
		fhq.pages['quest_hints_edit'](questid);
	}).fail(function(err){
		fhq.hideLoader();
		console.error(err);
	});
};

fhq.addQuestHintForm = function() {
	var questid = $(this).attr('questid');
	questid = parseInt(questid,10);
	
	$('#modalInfoTitle').html('Quest {' + questid + '} add hint');
	$('#modalInfoBody').html('');
	$('#modalInfoBody').append(''
		+ 'Hint test:'
		+ '<input class="form-control" id="quest_new_hint_text" type="text"><br>'
		+ '<div class=" alert alert-danger" style="display: none" id="quest_add_hint_error"></div>'
	);
	$('#modalInfoButtons').html(''
		+ '<button type="button" class="btn btn-secondary" '
		+ ' id="quest_add_hint" questid="' + questid + '" '
		+ ' onclick="fhq.addHint(this);">Add</button> '
		+ '<button type="button" class="btn btn-secondary" data-dismiss="modal">Close</button>'
	);
	$('#modalInfo').modal('show');
};

fhq.addHint = function(el){
	var hint_text = $('#quest_new_hint_text').val();
	var questid = $(el).attr('questid');
	questid = parseInt(questid,10);
	var data = {};
	data.questid = questid;
	data.hint = hint_text;

	fhq.ws.addhint(data).done(function(r){
		$('#modalInfo').modal('hide');
		fhq.pages['quest_hints_edit'](questid);
	}).fail(function(r){
		$('#quest_add_hint_error').show();
		$('#quest_add_hint_error').html(r.error);
	});
}

fhq.pages['quest_files_edit'] = function(questid) {
	questid = questid || fhq.pageParams['quest_files_edit'];
	if (questid) {
		questid = parseInt(questid, 10);
	}
	$('#page_name').html('Quest Files Edit [quest#' + questid + ']');
	var el = $('#page_content');
	el.html('');
	console.log("questid: ", questid);
	window.fhq.changeLocationState({'quest_files_edit':questid});
	fhq.hideLoader();
	
	el.html('');
	quest_edit_menu(el, 'quest_files_edit', questid);
	el.append('TODO');
}

fhq.pages['quest_writeups_edit'] = function(questid) {
	questid = questid || fhq.pageParams['quest_writeups_edit'];
	if (questid) {
		questid = parseInt(questid, 10);
	}
	$('#page_name').html('Quest WriteUps Edit [quest#' + questid + ']');
	var el = $('#page_content');
	el.html('');
	console.log("questid: ", questid);
	window.fhq.changeLocationState({'quest_writeups_edit':questid});
	fhq.hideLoader();
	
	el.html('');
	quest_edit_menu(el, 'quest_writeups_edit', questid);
	el.append(''
		+ '<div class="card">'
		+ '		<div class="card-body">'
		+ '			<table class="table table-striped table-hover">'
		+ '				<thead class="thead-dark"><tr><th>#</th><th>Text</th><th>Actions</th></tr></thead>'
		+ '				<tbody id="quest_writeupds"></tbody>'
		+ '			</table><br>'
		+ '         <div class="btn btn-primary" id="add_writeup" questid="' + questid + '">Add Writeup</div>'
		+ '		</div>'
		+ '</div>'
	);
	$('#add_writeup').unbind().bind('click', fhq.addQuestWriteupForm);
	
	fhq.ws.quests_writeups_list({"questid": questid}).done(function(r){
		var writeups = r.data;
		for (var i in writeups) {
			var w = writeups[i];
			console.log(w)
			$('#quest_writeupds').append('<tr>'
				+ '<td>[writeup#' + w.writeupid + ']</td>'
				+ '<td>' + w.link + '</td>'
				+ '<td><div class="btn btn-danger delete-writeup" '
				+ '         questid="' + questid + '" writeupid="' + w.writeupid + '" >Delete Hint</div></td>'
				+ '</tr>');
		}
		$('.delete-writeup').unbind().bind('click', fhq.deleteWriteup);
	}).fail(function(err){
		console.error(err);
	})
	// 
	// 
}

fhq.addQuestWriteupForm = function() {
	var questid = $(this).attr('questid');
	questid = parseInt(questid,10);

	$('#modalInfoTitle').html('Quest {' + questid + '} add writeup');
	$('#modalInfoBody').html('');
	$('#modalInfoBody').append(''
		+ 'WriteUp link:'
		+ '<input class="form-control" id="quest_add_writeup_link" placeholder="https://www.youtube.com/watch?v=gJeOeTGI7T8" type="text"><br>'
		+ '<div class=" alert alert-danger" style="display: none" id="quest_add_writeup_error"></div>'
	);
	$('#modalInfoButtons').html(''
		+ '<button type="button" class="btn btn-secondary" '
		+ ' id="quest_add_writeup" questid="' + questid + '" '
		+ ' onclick="fhq.addWriteUp(this);">Add</button> '
		+ '<button type="button" class="btn btn-secondary" data-dismiss="modal">Close</button>'
	);
	$('#modalInfo').modal('show');
};

fhq.addWriteUp = function(el){
	var writeup_link = $('#quest_add_writeup_link').val();
	var questid = $(el).attr('questid');
	questid = parseInt(questid,10);
	var data = {};
	data.questid = questid;
	data.writeup_link = writeup_link;

	fhq.ws.quests_writeups_proposal(data).done(function(r){
		$('#modalInfo').modal('hide');
		fhq.pages['quest_writeups_edit'](questid);
	}).fail(function(r){
		$('#quest_add_writeup_error').show();
		$('#quest_add_writeup_error').html(r.error);
	});
}

fhq.deleteWriteup = function() {
	var writeupid = $(this).attr('writeupid');
	writeupid = parseInt(writeupid,10);
	var questid = $(this).attr('questid');
	questid = parseInt(questid,10);

	fhq.showLoader();
	fhq.ws.quests_writeups_delete({"writeupid": writeupid}).done(function(r){
		console.log(r);
		fhq.hideLoader();
		fhq.pages['quest_writeups_edit'](questid);
	}).fail(function(err){
		fhq.hideLoader();
		console.error(err);
	});
};

fhq.pages['quest_statistics_edit'] = function(questid) {
	questid = questid || fhq.pageParams['quest_statistics_edit'];
	if (questid) {
		questid = parseInt(questid, 10);
	}
	$('#page_name').html('Quest Statistics Edit [quest#' + questid + ']');
	var el = $('#page_content');
	el.html('');
	console.log("questid: ", questid);
	window.fhq.changeLocationState({'quest_statistics_edit':questid});
	fhq.hideLoader();
	
	el.html('');
	quest_edit_menu(el, 'quest_statistics_edit', questid);
	el.append('TODO');
}

fhq.pages['quest_edit'] = function(questid) {
	questid = questid || fhq.pageParams['quest_edit'];
	if (questid) {
		questid = parseInt(questid, 10);
	}
	$('#page_name').html('Quest Edit [quest#' + questid + ']');
	var el = $('#page_content');
	el.html('');
	console.log("questid: ", questid);
	window.fhq.changeLocationState({'quest_edit':questid});
	fhq.hideLoader();
	
	el.html('');
	quest_edit_menu(el, 'quest_edit', questid);
	el.append(''
		+ '<div class="card">'
		+ '		<div class="card-body">'
		+ '			<div class="form-group row">'
		+ '				<label for="edit_quest_id" class="col-sm-2 col-form-label">' + fhq.t('ID') + '</label>'
		+ ' 			<div class="col-sm-4">'
		+ '					<input type="text" readonly class="form-control" value="" id="edit_quest_id">'
		+ '				</div>'
		+ '				<label for="edit_quest_count_user_solved" class="col-sm-2 col-form-label text-right">' + fhq.t('Solved') + '</label>'
		+ ' 			<div class="col-sm-4">'
		+ '					<input type="text" readonly class="form-control" value="" id="edit_quest_count_user_solved">'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="edit_quest_uuid" class="col-sm-2 col-form-label">' + fhq.t('UUID') + '</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<input type="text" readonly class="form-control" value="" id="edit_quest_uuid">'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="edit_quest_gameid" class="col-sm-2 col-form-label">' + fhq.t('Game') + '</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<select class="form-control" id="edit_quest_gameid"></select>'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="edit_name" class="col-sm-2 col-form-label">' + fhq.t('Name') + '</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<input type="text" class="form-control" value="" id="edit_quest_name">'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="edit_text" class="col-sm-2 col-form-label">' + fhq.t('Text') + ' (markdown)</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<textarea type="text" class="form-control" style="height: 150px" value="" id="edit_quest_text"></textarea>'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="edit_score" class="col-sm-2 col-form-label">' + fhq.t('Score') + ' (+)</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<input type="number" class="form-control" id="edit_quest_score">'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="edit_subject" class="col-sm-2 col-form-label">' + fhq.t('Subject') + '</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<select class="form-control" value="" id="edit_quest_subject">'
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
		+ '				<label for="edit_quest_answer" class="col-sm-2 col-form-label">' + fhq.t('Answer') + '</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<input type="text" class="form-control" id="edit_quest_answer" value="">'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="edit_quest_answer_format" class="col-sm-2 col-form-label">' + fhq.t('Answer format') + '</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<input type="text" class="form-control" id="edit_quest_answer_format" value="">'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="edit_author" class="col-sm-2 col-form-label">' + fhq.t('Author') + '</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<input type="text" class="form-control" value="" id="edit_quest_author">'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="edit_copyright" class="col-sm-2 col-form-label">' + fhq.t('Copyright') + '</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<input type="text" class="form-control" value="" id="edit_quest_copyright">'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="edit_quest_state" class="col-sm-2 col-form-label">' + fhq.t('State') + '</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<select class="form-control" value="" id="edit_quest_state">'
		+ '						<option value="open">Open</option>'
		+ '						<option value="closed">Closed</option>'
		+ '						<option value="broken">Broken</option>'
		+ '					</select>'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="edit_description_state" class="col-sm-2 col-form-label">' + fhq.t('Description State') + '</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<textarea type="text" class="form-control" style="height: 150px" value="" id="edit_quest_description_state"></textarea>'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label class="col-sm-2 col-form-label"></label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<div id="quest_update_error" class="alert alert-danger" style="display: none"></div>'
		+ '				</div>'
		+ '			</div>'
		+ '		</div>'
		+ '</div>'
		+ '<div class="btn btn-danger" id="quest_update"><i class="fa fa-check"></i> Update</div> '
		+ '<div class="btn btn-primary" id="quest_update_close"><i class="fa fa-times"></i> Cancel</div> '
	);
	window['edit_quest_text'] = new SimpleMDE({ element: $("#edit_quest_text")[0] });

	$('#quest_update').unbind().bind('click', fhq.updateQuest);
	$('#quest_update_close').unbind().bind('click', fhq.pages['quests']);

	fhq.ws.games().done(function(r){
		console.log(r);
		for(var i in r.data) {
			$('#edit_quest_gameid').append('<option value="' + r.data[i]["local_id"] + '">' + r.data[i]["name"] + '</option>');
		}
		fhq.ws.quest({"questid": questid}).done(function(data){
			var q = data.quest;
			var hints = data.hints;
			var files = data.files;
			// console.log(data);
			for (var p in q) {
				var edit_id = 'edit_quest_' + p;
				// console.log(edit_id)
				if ($('#' + edit_id).length > 0) {
					if (edit_id == "edit_quest_text") {
						window['edit_quest_text'].value(q[p]);
					} else {
						$('#' + edit_id).val(q[p]);
					}
				} else {
					console.warn("Not found field: " + edit_id);
				}
			}
		}).fail(function(err){
			console.error(err);
		})

	})
}

fhq.updateQuest = function() {
	fhq.showLoader();
	var params = {};
	params["questid"] = parseInt($("#edit_quest_id").val(),10);
	params["gameid"] = parseInt($("#edit_quest_gameid").val(),10);
	params["name"] = $("#edit_quest_name").val();
	params["text"] = window['edit_quest_text'].value();
	params["score"] = parseInt($("#edit_quest_score").val(),10);
	params["subject"] = $("#edit_quest_subject").val();
	params["copyright"] = $("#edit_quest_copyright").val();
	params["author"] = $("#edit_quest_author").val();
	params["answer"] = $("#edit_quest_answer").val();
	params["answer_format"] = $("#edit_quest_answer_format").val();
	params["state"] = $("#edit_quest_state").val();
	params["description_state"] = $("#edit_quest_description_state").val();

	fhq.ws.quest_update(params).done(function(r){
		fhq.hideLoader();
		// fhq.loadQuest(r.questid);
		fhq.pages['quests']();
	}).fail(function(err){
		fhq.hideLoader();
		console.error(err);
		$('#quest_update_error').show();
		$('#quest_update_error').html(err.error);
	});
};