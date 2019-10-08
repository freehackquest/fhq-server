if(!window.fhq) window.fhq = {};
if(!fhq.pages) fhq.pages = [];

fhq.pages['settings'] = function(idelem) {
	$('.nav-link.main-menu').removeClass("active");
	$('#menu_settings').addClass("active");
	fhq.changeLocationState({'settings':''});
	$('#page_name').html('Settings');
	$('#page_content').html('');
	
	var el = $('#page_content');
	el.html('');
	
	fhq.ws.server_settings().done(function(r){
		fhq.hideLoader();
		console.log(r);
		for(var name in r.data){
			var sett = r.data[name];
			var groupid = 'settings_group_' + sett.group;
			if($('#' + groupid).length == 0){
				el.append(''
					+ '<div class="card">'
					+ '  <div class="card-header">' + fhq.t(groupid) + '</div>'
					+ '  <div class="card-body">'
					+ '   <div id="' + groupid + '">'
					+ '   </div>'
					+ '  </div>'
					+ '</div><br>'
				);
			}
			
			var settid = 'setting_name_' + sett.name;
			
			var input_type = 'text';
			var edit_button = ''
				+ '  <div class="btn btn-secondary edit-settings" '
				+ '     groupid="' + groupid + '" '
				+ '     setttype="' + sett.type + '" '
				+ '     settname="' + sett.name + '" '
				+ '     settid="' + settid + '">Edit</div>';
			if (sett['readonly']) {
				edit_button = '';
			}

			if (sett.type == 'number') {
				$('#' + groupid).append(''
					+ '<div class="form-group row">'
					+ '	<label for="' + settid + '" class="col-sm-2 col-form-label">' + fhq.t(settid) + '</label>'
					+ '	<div class="col-sm-7">'
					+ '		<input type="number" readonly class="form-control" id="' + settid + '">'
					+ '	</div>'
					+ '	<div class="col-sm-2">'
					+ edit_button
					+ '	</div>'
					+ '</div>'
				);
				$('#' + settid).val(sett.value);
			} else if(sett.type == 'password') {
				$('#' + groupid).append(''
					+ '<div class="form-group row">'
					+ '	<label for="' + settid + '" class="col-sm-2 col-form-label">' + fhq.t(settid) + '</label>'
					+ '	<div class="col-sm-7">'
					+ '		<input type="password" readonly class="form-control" id="' + settid + '">'
					+ '	</div>'
					+ '	<div class="col-sm-2">'
					+ edit_button
					+ '	</div>'
					+ '</div>'
				);
				$('#' + settid).val(sett.value);
			} else if(sett.type == 'string' || sett.type == 'dir_path' || sett.type == 'file_path') {
				$('#' + groupid).append(''
					+ '<div class="form-group row">'
					+ '	<label for="' + settid + '" class="col-sm-2 col-form-label">' + fhq.t(settid) + '</label>'
					+ '	<div class="col-sm-7">'
					+ '		<input type="text" readonly class="form-control" id="' + settid + '">'
					+ '	</div>'
					+ '	<div class="col-sm-2">'
					+ edit_button
					+ '	</div>'
					+ '</div>'
				);
				$('#' + settid).val(sett.value);
			} else if(sett.type == 'text') {
				$('#' + groupid).append(''
					+ '<div class="form-group row">'
					+ '	<label for="' + settid + '" class="col-sm-2 col-form-label">' + fhq.t(settid) + '</label>'
					+ '	<div class="col-sm-7">'
					+ '		<textarea rows="10" readonly class="form-control" id="' + settid + '"></textarea>'
					+ '	</div>'
					+ '	<div class="col-sm-2">'
					+ edit_button
					+ '	</div>'
					+ '</div>'
				);
				$('#' + settid).html(sett.value);
			} else if(sett.type == 'boolean') {
				$('#' + groupid).append(''
					+ '<div class="form-group row">'
					+ '	<label for="' + settid + '" class="col-sm-2 col-form-label">' + fhq.t(settid) + '</label>'
					+ '	<div class="col-sm-7">'
					+ '		<select disabled class="form-control" id="' + settid + '">'
					+ '			<option name="no">no</option>'
					+ '			<option name="yes">yes</option>'
					+ '		<select class="form-control">'
					+ '	</div>'
					+ '	<div class="col-sm-2">'
					+ edit_button
					+ '	</div>'
					+ '</div>'
				);
				$('#' + settid).val(sett.value == true ? 'yes' : 'no');
			} else {
				console.error("Unknown type of settings: ", sett)
			}
		}
		
		
		$('.edit-settings').unbind().bind('click', function(){
			$('#modalSettings').modal('show');
			
			var setttype = $(this).attr('setttype');
			var settname = $(this).attr('settname');
			var settid = $(this).attr('settid');
			var groupid = $(this).attr('groupid');
			
			var val = $('#' + settid).val();
			
			$('#modalSettings .modal-body').html('');
			$('#modalSettings .modal-body').append('<h3>' + fhq.t(groupid) + '/' + fhq.t(settid) + '</h3>')
			
			if(setttype == 'string'){
				$('#modalSettings .modal-body').append(
					'<input type="text" class="form-control" id="modalSettings_newval">'
					+ '<p id="modalSettings_error"></p>'
				);
				$('#modalSettings_newval').val(val);
			} else if (setttype == 'text') {
				$('#modalSettings .modal-body').append(
					'<textarea rows="15" class="form-control" id="modalSettings_newval"></textarea>'
					+ '<p id="modalSettings_error"></p>'
				);
				$('#modalSettings_newval').html(val);
			} else if(setttype == 'boolean') {
				$('#modalSettings .modal-body').append(''
					+ '		<select class="form-control" id="modalSettings_newval">'
					+ '			<option name="no">no</option>'
					+ '			<option name="yes">yes</option>'
					+ '		<select class="form-control">'
					+ '<p id="modalSettings_error"></p>'
				);
				$('#modalSettings_newval').val(val);
			} else if(setttype == 'password') {
				$('#modalSettings .modal-body').append(
					'<input type="password" class="form-control" id="modalSettings_newval">'
					+ '<p id="modalSettings_error"></p>'
				);
				$('#modalSettings_newval').val('');
			} else if (setttype == 'number') {
				$('#modalSettings .modal-body').append(
					'<input type="number" class="form-control" id="modalSettings_newval">'
					+ '<p id="modalSettings_error"></p>'
				);
				$('#modalSettings_newval').val(val);
			}
			
			$('#modalSettings .save-setting').unbind().bind('click', function(){
				$('#modalSettings_newval').attr({'readonly': true});
				$('#modalSettings_newval').attr({'disabled': true});
				$('#modalSettings_error').html('');
				var data = {};
				data.name = settname;
				data.value = $('#modalSettings_newval').val();

				fhq.ws.server_settings_update(data).done(function(r){
					console.log(r);
					if (setttype != 'password') {
						$('#' + settid).val(data.value);
					}
					$('#modalSettings').modal('hide');
				}).fail(function(err){
					console.error(err);
					$('#modalSettings_newval').removeAttr('readonly');
					$('#modalSettings_newval').removeAttr('disabled');
					$('#modalSettings_error').html(err.error);
				})
				
			});
			// modalSettings
			
		});
		
	}).fail(function(err){
		fhq.ui.hideLoading();
		console.error(err);
	})
}




