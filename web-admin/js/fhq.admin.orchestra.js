if(!window.fhq) window.fhq = {};
if(!fhq.pages) fhq.pages = [];

fhq.pages['orchestra'] = function(){
	$('.nav-link.main-menu').removeClass("active");
	$('#menu_orchestra').addClass("active");

	$('#page_name').html('<div class="header-name">Orchestra</div>'
		+ '<div class="header-btn" id="orchestra_create"><i class="fa fa-plus"></i></div>'
	);
	
	$('#orchestra_create').unbind().bind('click', fhq.pages['orchestra_create'] )
	
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
	
	window.fhq.changeLocationState({'orchestra': '', 'onpage': onpage, 'page': page});

	fhq.ws.lxd_list({'onpage': onpage, 'page': page}).done(function(r){
		console.log(r);
		fhq.hideLoader();
		el.html('');
		el.append('<table class="table table-striped">'
			+ '		<thead>'
			+ '			<tr>'
			+ '				<th>Name</th>'
			+ '				<th>State</th>'
			+ '				<th>IPv4 / IPv6</th>'
			+ '				<th>Type</th>'
			+ '				<th>Action</th>'
			+ '			</tr>'
			+ '		</thead>'
			+ '		<tbody id="data_list">'
			+ '		</tbody>'
			+ '</table>'
		)
		for(var i in r.data){
			var orch = r.data[i];
			var c_name = orch.name || orch;
			$('#data_list').append('<tr>'
				+ '	<td>' + orch + ' / ' + orch.name + '</td>'
				+ '	<td>' + orch.state + '</td>'
				+ '	<td>' + orch.ipv4 + '/' + orch.ipv6 + '</td>'
				+ '	<td>' + orch.type + '</td>'
				+ '	<td>'
				+ '			<div class="tbl-action-btn container-start" name="' + c_name + '"><i class="fa fa-play"></i></div>'
				+ '			<div class="tbl-action-btn container-stop" name="' + c_name + '"><i class="fa fa-stop"></i></div>'
				+ '			<div class="tbl-action-btn container-remove" name="' + c_name + '"><i class="fa fa-trash"></i></div>'
				+ '	</td>'
				+ '</tr>'
			)
		}
		
		$('.container-start').unbind().bind('click',function() {
			var c_name = $(this).attr('name');
			fhq.showLoader();
			var data = {action: 'start', name: c_name};
			fhq.ws.lxd_containers(data).done(function(r){
				fhq.hideLoader();
				fhq.pages['orchestra']();
			}).fail(function(err){
				fhq.hideLoader();
			});
		})
		
		$('.container-stop').unbind().bind('click',function() {
			var c_name = $(this).attr('name');
			fhq.showLoader();
			var data = {action: 'stop', name: c_name};
			fhq.ws.lxd_containers(data).done(function(r){
				fhq.hideLoader();
				fhq.pages['orchestra']();
			}).fail(function(err){
				fhq.hideLoader();
			});
		})
		
		$('.container-remove').unbind().bind('click',function() {
			var c_name = $(this).attr('name');
			fhq.showLoader();
			var data = {action: 'delete', name: c_name};
			fhq.ws.lxd_containers(data).done(function(r){
				fhq.hideLoader();
				fhq.pages['orchestra']();
			}).fail(function(err){
				fhq.hideLoader();
			});
		})
		
	}).fail(function(r){
		fhq.hideLoader();
		console.error(r);
		el.append(r.error);
	})
}

fhq.createContainer = function() {
	fhq.showLoader();
	var data = {};
	data["action"] = "create";
	data["name"] = $("#newcontainer_name").val();

	fhq.ws.lxd_containers(data).done(function(r){
		fhq.hideLoader();
		fhq.pages['orchestra']();
	}).fail(function(err){
		fhq.hideLoader();
		console.error(err);
		$('#orchestra_create_error').show();
		$('#orchestra_create_error').html(err.error);
		
	});
};


fhq.pages['orchestra_create'] = function(){
	$('#page_name').html('<div class="header-name">Orchestra Create</div>'
		+ '<div class="header-btn" id="orchestra_create"><i class="fa fa-check"></i></div>'
		+ '<div class="header-btn" id="orchestra_create_close"><i class="fa fa-times"></i></div>'
	);

	$('#orchestra_create').unbind().bind('click', fhq.createContainer);
	$('#orchestra_create_close').unbind().bind('click', fhq.pages['orchestra']);
	
	var el = $('#page_content');
	el.html('');

	window.fhq.changeLocationState({'orchestra_create':''});
	fhq.hideLoader();
	
	el.html('');
	el.html(''
		+ '<div class="card">'
		+ '		<div class="card-header">' + fhq.t('Create Container') + '</div>'
		+ '		<div class="card-body">'
		+ '			<div class="form-group row">'
		+ '				<label for="newcontainer_name" class="col-sm-2 col-form-label">' + fhq.t('Name') + '</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<input type="text" class="form-control" value="" id="newcontainer_name">'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label class="col-sm-2 col-form-label"></label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<div id="orchestra_create_error" class="alert alert-danger" style="display: none"></div>'
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



