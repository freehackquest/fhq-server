if(!window.fhq) window.fhq = {};

fhq.createGame = function()  {
	fhq.showLoader();
	$('#game_create_error').hide();

	var data = {};
	data["uuid"] = $("#newgame_uuid").val();
	data["name"] = $("#newgame_name").val();
	data["state"] = $("#newgame_state").val();
	data["form"] = $("#newgame_form").val();
	data["type"] = $("#newgame_type").val();
	data["date_start"] = $("#newgame_date_start").val();
	data["date_stop"] = $("#newgame_date_stop").val();
	data["date_restart"] = $("#newgame_date_restart").val();
	data["description"] = $("#newgame_description").val();
	data["organizators"] = $("#newgame_organizators").val();

	fhq.ws.game_create(data).done(function(r){
		fhq.hideLoader();
		fhq.pages['games']();
	}).fail(function(err){
		fhq.hideLoader();
		console.error(err);
		$('#game_create_error').show();
		$('#game_create_error').html(err.error);
		
	})
};

fhq.pages['game_create'] = function() {
	$('.nav-link.main-menu').removeClass("active");
	$('#menu_games').addClass("active");

	fhq.changeLocationState({'game_create':''});
	$('#page_name').html('Game Create');
	$('#page_content').html('');
	var el = $('#page_content');
	el.html('');
	fhq.hideLoader();
	
	// TODO game uuid -> generate to backend
	
	el.html(''
		+ '<div class="card">'
		+ '		<div class="card-header">New Game</div>'
		+ '		<div class="card-body">'
		+ '			<div class="form-group row">'
		+ '				<label for="newgame_uuid" class="col-sm-2 col-form-label">UUID</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<input type="text" class="form-control" value="' + fhq.guid() + '" id="newgame_uuid">'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="newgame_name" class="col-sm-2 col-form-label">Name</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<input type="text" class="form-control" value="" id="newgame_name">'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="newgame_state" class="col-sm-2 col-form-label">State</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<select class="form-control" value="" id="newgame_state">'
		+ '						<option value="original">Original</option>'
		+ '						<option value="copy">Copy</option>'
		+ '						<option value="unlicensed-copy">Unlicensed Copy</option>'
		+ '					</select>'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="newgame_form" class="col-sm-2 col-form-label">Form</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<select class="form-control" value="" id="newgame_form">'
		+ '						<option value="online">Online</option>'
		+ '						<option value="offline">Offline</option>'
		+ '					</select>'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="newgame_type" class="col-sm-2 col-form-label">Type</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<select class="form-control" value="" id="newgame_type">'
		+ '						<option value="jeopardy">Jeopardy</option>'
		+ '					</select>'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="newgame_date_start" class="col-sm-2 col-form-label">Date Start</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<input type="text" class="form-control" id="newgame_date_start" value="0000-00-00 00:00:00">'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="newgame_date_stop" class="col-sm-2 col-form-label">Date Stop</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<input type="text" class="form-control" id="newgame_date_stop" value="0000-00-00 00:00:00">'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="newgame_date_restart" class="col-sm-2 col-form-label">Date Restart</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<input type="text" class="form-control" id="newgame_date_restart" value="0000-00-00 00:00:00">'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="newgame_description" class="col-sm-2 col-form-label">Description</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<textarea type="text" class="form-control" style="height: 150px" value="" id="newgame_description"></textarea>'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="newgame_organizators" class="col-sm-2 col-form-label">Organizators</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<input type="text" class="form-control" value="" id="newgame_organizators">'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label class="col-sm-2 col-form-label"></label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<div class="btn btn-secondary" onclick="fhq.createGame();">Create</div>'
		+ '					<div class="btn btn-secondary game-create-cancel">Cancel</div>'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label class="col-sm-2 col-form-label"></label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<div id="game_create_error" class="alert alert-danger" style="display: none"></div>'
		+ '				</div>'
		+ '			</div>'
		+ '		</div>'
		+ '</div>'
	);
	
	$('#newgame_date_start').datetimepicker({
		format:'Y-m-d H:i:s',
		inline:false
	});
	
	$('#newgame_date_stop').datetimepicker({
		format:'Y-m-d H:i:s',
		inline:false
	});
	
	$('#newgame_date_restart').datetimepicker({
		format:'Y-m-d H:i:s',
		inline:false
	});

	$('.game-create-cancel').unbind().bind('click', function(){
		fhq.pages['games']();
	})
}

fhq.updateGame = function()  {
	fhq.showLoader();

	var data = {};
	data["uuid"] = $("#editgame_uuid").val();
	data["name"] = $("#editgame_name").val();
	data["state"] = $("#editgame_state").val();
	data["form"] = $("#editgame_form").val();
	data["type"] = $("#editgame_type").val();
	data["date_start"] = $("#editgame_date_start").val();
	data["date_stop"] = $("#editgame_date_stop").val();
	data["date_restart"] = $("#editgame_date_restart").val();
	data["description"] = $("#editgame_description").val();
	data["organizators"] = $("#editgame_organizators").val();

	fhq.ws.game_update(data).done(function(r){
		fhq.hideLoader();
		fhq.pages['games']();
	}).fail(function(err){
		fhq.hideLoading();
		console.error(err);
	});
};

fhq.pages['game_edit'] = function(gameuuid) {
	$('#page_name').html('Game Edit');
	$('#page_content').html('');
	fhq.showLoader();
	if(!gameuuid){
		gameuuid = fhq.pageParams['game_edit'];
	}
	window.fhq.changeLocationState({'game_edit':gameuuid});
	var el = $('#page_content');
	el.html('Loading... ' + gameuuid);

	fhq.ws.game_info({uuid: gameuuid}).done(function(r){
		fhq.hideLoader();
		el.html(''
			+ '<div class="card">'
			+ '		<div class="card-header">Game Edit</div>'
			+ '		<div class="card-body">'
			+ '			<div class="form-group row">'
			+ '				<label for="editgame_uuid" class="col-sm-2 col-form-label">UUID</label>'
			+ ' 			<div class="col-sm-10">'
			+ '					<input type="text" readonly class="form-control" value="" id="editgame_uuid">'
			+ '				</div>'
			+ '			</div>'
			+ '			<div class="form-group row">'
			+ '				<label for="editgame_name" class="col-sm-2 col-form-label">Name</label>'
			+ ' 			<div class="col-sm-10">'
			+ '					<input type="text" class="form-control" value="" id="editgame_name">'
			+ '				</div>'
			+ '			</div>'
			+ '			<div class="form-group row">'
			+ '				<label for="editgame_state" class="col-sm-2 col-form-label">State</label>'
			+ ' 			<div class="col-sm-10">'
			+ '					<select class="form-control" value="" id="editgame_state">'
			+ '						<option value="original">Original</option>'
			+ '						<option value="copy">Copy</option>'
			+ '						<option value="unlicensed-copy">Unlicensed Copy</option>'
			+ '					</select>'
			+ '				</div>'
			+ '			</div>'
			+ '			<div class="form-group row">'
			+ '				<label for="editgame_form" class="col-sm-2 col-form-label">Form</label>'
			+ ' 			<div class="col-sm-10">'
			+ '					<select class="form-control" value="" id="editgame_form">'
			+ '						<option value="online">Online</option>'
			+ '						<option value="offline">Offline</option>'
			+ '					</select>'
			+ '				</div>'
			+ '			</div>'
			+ '			<div class="form-group row">'
			+ '				<label for="editgame_type" class="col-sm-2 col-form-label">Type</label>'
			+ ' 			<div class="col-sm-10">'
			+ '					<select class="form-control" value="" id="editgame_type">'
			+ '						<option value="jeopardy">Jeopardy</option>'
			+ '					</select>'
			+ '				</div>'
			+ '			</div>'
			+ '			<div class="form-group row">'
			+ '				<label for="editgame_date_start" class="col-sm-2 col-form-label">Date Start</label>'
			+ ' 			<div class="col-sm-10">'
			+ '					<input type="text" class="form-control" id="editgame_date_start" value="0000-00-00 00:00:00">'
			+ '				</div>'
			+ '			</div>'
			+ '			<div class="form-group row">'
			+ '				<label for="editgame_date_stop" class="col-sm-2 col-form-label">Date Stop</label>'
			+ ' 			<div class="col-sm-10">'
			+ '					<input type="text" class="form-control" id="editgame_date_stop" value="0000-00-00 00:00:00">'
			+ '				</div>'
			+ '			</div>'
			+ '			<div class="form-group row">'
			+ '				<label for="editgame_date_restart" class="col-sm-2 col-form-label">Date Restart</label>'
			+ ' 			<div class="col-sm-10">'
			+ '					<input type="text" class="form-control" id="editgame_date_restart" value="0000-00-00 00:00:00">'
			+ '				</div>'
			+ '			</div>'
			+ '			<div class="form-group row">'
			+ '				<label for="editgame_description" class="col-sm-2 col-form-label">Description</label>'
			+ ' 			<div class="col-sm-10">'
			+ '					<textarea type="text" class="form-control" style="height: 150px" value="" id="editgame_description"></textarea>'
			+ '				</div>'
			+ '			</div>'
			+ '			<div class="form-group row">'
			+ '				<label for="editgame_organizators" class="col-sm-2 col-form-label">Organizators</label>'
			+ ' 			<div class="col-sm-10">'
			+ '					<input type="text" class="form-control" value="" id="editgame_organizators">'
			+ '				</div>'
			+ '			</div>'
			+ '			<div class="form-group row">'
			+ '				<label class="col-sm-2 col-form-label"></label>'
			+ ' 			<div class="col-sm-10">'
			+ '					<div class="btn btn-secondary" onclick="fhq.updateGame();">' + fhq.t('Save') + '</div>'
			+ '					<div class="btn btn-secondary edit-game-cancel">' + fhq.t('Cancel') + '</div>'
			+ '				</div>'
			+ '			</div>'
			+ '		</div>'
			+ '</div>'
		);
		
		$('#editgame_uuid').val(r.data.uuid);
		$('#editgame_name').val(r.data.title);
		$('#editgame_state').val(r.data.state);
		$('#editgame_form').val(r.data.form);
		$('#editgame_type').val(r.data.type_game);
		$('#editgame_date_start').val(r.data.date_start);
		$('#editgame_date_stop').val(r.data.date_stop);
		$('#editgame_date_restart').val(r.data.date_restart);
		$('#editgame_description').val(r.data.description);
		$('#editgame_organizators').val(r.data.organizators);

		$('.edit-game-cancel').unbind().bind('click', function(){
			fhq.pages['games']();
		});

	}).fail(function(err){
		console.error(err);
		fhq.hideLoader();
		el.html(err.error);
	})
}


fhq.gameExport = function(gameuuid){
	fhq.showLoader();
	fhq.ws.game_export({uuid: gameuuid}).done(function(r){
		fhq.hideLoader();
		console.log(r);
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

fhq.gameDelete = function(el){
	var gameuuid = $(el).attr('gameuuid');
	var admin_password = $('#game_delete_admin_password').val();
	fhq.ws.game_delete({uuid: gameuuid, admin_password: admin_password}).done(function(r){
		fhq.pages['games']();
		$('#modalInfo').modal('hide');
	}).fail(function(err){
		console.error(err);
		$('#game_delete_error').show();
		$('#game_delete_error').html(err.error);
	});	
}

fhq.gameUpdateLogo = function(gameuuid){
	var data = {};
	data.image_png_base64 = $('#game_update_logo_img').attr('src');
	// remove prefix
	data.image_png_base64 = data.image_png_base64.replace("data:image/png;base64,", "");
	data.uuid = gameuuid;
	fhq.ws.game_update_logo(data).done(function(){
		$('#modalInfo').modal('hide');
		$('#img_game_' + gameuuid).attr({'src': 'data:image/png;base64,' + data.image_png_base64});
	}).fail(function(err){
		$('#game_update_logo_error').show();
		$('#game_update_logo_error').html(err.error);
	});
	
}

fhq.gameUpdateLogoForm = function(gameuuid){
	$('#modalInfoTitle').html('[game#' + gameuuid + '] Update Logo');
	var img_src = $('#img_game_' + gameuuid).attr('src');
	$('#modalInfo').modal('show');
	$('#modalInfoBody').html('');
	$('#modalInfoBody').append('<br><center>'
		+ '<input id="game_update_logo_file" type="file"><br>'
		+ '<div class=" alert alert-danger" style="display: none" id="game_update_logo_error"></div>'
		+ '<br>'
		+ '<img id="game_update_logo_img" src="' + img_src + '" width=170px height=170px/><br><br>'
		+ '</center>');
	$('#modalInfoButtons').html(
		'<button type="button" class="btn btn-secondary" style="display: none" id="game_update_logo_btn" onclick="fhq.gameUpdateLogo(\'' + gameuuid + '\');">Update</button> '
		+ '<button type="button" class="btn btn-secondary" data-dismiss="modal">Close</button>'
	);
	
	$('#game_update_logo_file').change(function(){
		$('#game_update_logo_error').hide();
		var file = document.getElementById('game_update_logo_file').files[0];
		var reader = new FileReader();
		reader.readAsDataURL(file);
		reader.onload = function () {
			var res = reader.result;
			if(res.indexOf("data:image/png;base64,") != 0){
				$('#game_update_logo_btn').hide();
				$('#game_update_logo_error').show();
				$('#game_update_logo_error').html("Only png expected");
			}else{
				$('#game_update_logo_img').attr({
					src: reader.result
				})
				$('#game_update_logo_img').unbind().bind('error', function(){
					$('#game_update_logo_btn').hide();
					$('#game_update_logo_error').show();
					$('#game_update_logo_error').html("Only png expected");
				}).bind('load', function(){
					$('#game_update_logo_btn').show();
				});
			}
		};
	})
}

fhq.pages['games'] = function(){
	$('#page_name').html('Games');
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
	
	window.fhq.changeLocationState({'games': '', 'onpage': onpage, 'page': page});

	fhq.ws.games({'onpage': onpage, 'page': page}).done(function(r){
		fhq.hideLoader();
		console.log(r);
		el.html('');
		
		el.append('<button id="game_create" class="btn btn-secondary">Create Game</button>');
		$('#game_create').unbind().bind('click', fhq.pages['game_create']);
		
		el.append('<button id="game_import" class="btn btn-secondary">Import Game</button>');
		$('#game_import').unbind().bind('click', function(){
			alert("TODO");
		});
		
		el.append('<hr>');
		

		el.append(fhq.paginator(0, r.count, r.onpage, r.page));
		el.append('<table class="table table-striped">'
			+ '		<thead>'
			+ '			<tr>'
			+ '				<th>#</th>'
			+ '				<th>Info</th>'
			+ '				<th>Actions</th>'
			+ '			</tr>'
			+ '		</thead>'
			+ '		<tbody id="list">'
			+ '		</tbody>'
			+ '</table>'
		)
		for(var i in r.data){
			var g = r.data[i];
			console.log(g);
			$('#list').append('<tr>'
				+ '	<td> '
				+ '		<image width=100 height=100 src="' + g.logo + '" id="img_game_' + g.uuid + '"/><br><br>'
				+ '		<p><button class="btn btn-secondary game-update-logo" gameuuid="' + g.uuid + '" onclick="fhq.gameUpdateLogoForm(\'' + g.uuid + '\');">Update logo</button></p>'
				+ '</td>'
				+ '	<td>'
				+'		<p>ID: ' + g.local_id + '</p>'
				+'		<p>uuid: ' + g.uuid + '</p>'
				+'		<p>Name: ' + g.name + '</p>'
				+'		<p>Logo: ' + g.logo + '</p>'
				+ '		<p>Max Score: ' + g.maxscore + '</p>'
				+ '		<p>Type: '  + g.type_game + '</p>'
				+ '		<p>Form: ' + g.form + '</p>'
				+ '		<p>Date Start: '  + g.date_start + '</p>'
				+ '		<p>Date Stop: '  + g.date_stop + '</p>'
				+ '		<p>Date Restart: '  + g.date_restart + '</p>'
				+ '		<p>Organizators: '  + g.organizators + '</p>'
				+ '		<p>State: '  + g.state + '</p>'
				+ '		<p>Description:</p><p>'  + g.description + '</p>'
				+ '</td>'
				+ '	<td>'
				+ '		<p><button class="btn btn-secondary game-remove" gameuuid="' + g.uuid + '">Remove</button></p>'
				+ '		<p><button class="btn btn-secondary game-edit" gameuuid="' + g.uuid + '">Edit</button></p>'
				+ '		<p><button class="btn btn-secondary game-export" gameuuid="' + g.uuid + '">Export</button></p>'
				+ '</td>'
				+ '</tr>'
			);
		}
		
		$('.game-edit').unbind().bind('click', function(){
			var gameuuid = $(this).attr('gameuuid');
			fhq.pages['game_edit'](gameuuid);
		});
		
		$('.game-export').unbind().bind('click', function(){
			var gameuuid = $(this).attr('gameuuid');
			fhq.gameExport(gameuuid);
		})
		
		$('.game-remove').unbind().bind('click', function(){
			console.warn('game_remove');
			var gameuuid = $(this).attr('gameuuid');
			
			$('#modalInfoTitle').html('Game {' + gameuuid + '} confirm deletion');
			$('#modalInfoBody').html('');
			$('#modalInfoBody').append(''
				+ 'Admin password:'
				+ '<input class="form-control" id="game_delete_admin_password" type="password"><br>'
				+ '<div class=" alert alert-danger" style="display: none" id="game_delete_error"></div>'
			);
			$('#modalInfoButtons').html(''
				+ '<button type="button" class="btn btn-secondary" id="game_delet_btn" gameuuid="' + gameuuid + '" onclick="fhq.gameDelete(this);">Delete</button> '
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
