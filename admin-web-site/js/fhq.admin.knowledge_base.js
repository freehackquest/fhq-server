if(!window.fhq) window.fhq = {};
if(!fhq.pages) fhq.pages = [];

if(!window.fhqadmin) window.fhqadmin = {};

fhqadmin.openKbItem = function(kbid) {
	// ) ''?knowledge_base=&onpage=5&page=0&parentid=' + p.classbookid + '
	window.fhq.changeLocationState({
		'knowledge_base': '',
		'parentid': kbid,
	});
	fhq.pages['knowledge_base']();
}

fhqadmin.knowledge_base_load_current_item = function(kbid, proposal_id) {
	var el = $('#current_kb_item');
	el.html('');
	fhq.ws.classbook_info({
		"classbookid": kbid,
	}).done(function(r) {
		var breadcrumb = '<nav aria-label="breadcrumb"><ol class="breadcrumb">';
		for(var i in r.data.parents) {
			var p = r.data.parents[i];
			breadcrumb += '<li class="breadcrumb-item"><a class="btn-link" href="javascript:void(0);" onclick="fhqadmin.openKbItem(' + p.classbookid + ')">' + p.name + '</a></li>';
		}
		breadcrumb += '<li class="breadcrumb-item active" aria-current="page">' + r.data.name + '</li>';
		breadcrumb += '</ol></nav>';
		el.append(breadcrumb);
		el.append(''
			+ '<div class="card">'
			+ '   <div class="card-body">'
			+ '       <div class="form-group row">'
			+ '           <label for="edit_quest_id" class="col-sm-2 col-form-label">ParentID</label>'
			+ '           <div class="col-sm-12">'
			+ '               <input type="text" class="form-control" value="" id="edit_kb_parentid">'
			+ '           </div>'
			+ '       </div>'
			+ '       <div class="form-group row">'
			+ '           <label for="edit_quest_id" class="col-sm-2 col-form-label">Name</label>'
			+ '           <div class="col-sm-12">'
			+ '               <input type="text" class="form-control" value="" id="edit_kb_name">'
			+ '           </div>'
			+ '       </div>'
			+ '       <div class="form-group row">'
			+ '           <label for="edit_quest_id" class="col-sm-2 col-form-label">Proposals</label>'
			+ '           <div id="proposal_list" class="col-sm-12">'
			+ '           </div>'
			+ '       </div>'
			+ '       <div class="form-group row">'
			+ '           <label for="edit_quest_id" class="col-sm-2 col-form-label">Content</label>'
			+ '           <div class="col-sm-12">'
			+ '               <textarea type="text" class="form-control" style="height: 150px" value="" id="edit_kb_content"></textarea>'
			+ '           </div>'
			+ '       </div>'
			+ '     <div class="form-group row">'
			+ '           <label for="edit_quest_id" class="col-sm-2 col-form-label">Ordered</label>'
			+ '           <div class="col-sm-12">'
			+ '               <input type="text" class="form-control" value="" id="edit_kb_ordered">'
			+ '           </div>'
			+ '       </div>'
			+ '    <div class="btn btn-danger kbi-approve" kbid="' + kbid + '" propid="' + proposal_id + '">Approve</div>'
			+ '    <div class="btn btn-danger kbi-save" kbid="' + kbid + '" propid="' + proposal_id + '">Save</div>'
			+ '    <div class="btn btn-danger kbi-remove" kbid="' + kbid + '" propid="' + proposal_id + '">Remove</div>'
			+ '   </div>'
			+ '</div>'
		);
		$('#edit_kb_name').val(r.data.name);
		$('#edit_kb_ordered').val(r.data.ordered);
		$('#edit_kb_parentid').val(r.data.parentid);

		window['edit_kb_content'] = new SimpleMDE({ element: $("#edit_kb_content")[0] });
		
		console.log('Success: ', r);

		const list = $('#proposal_list');

		let classes = "btn btn-primary prop_open"
		if(proposal_id === -1) {
			classes = "btn btn-secondary"
		}
		list.append(`<div class="${classes}" kbid="${kbid}" propid="-1">Original</div> `);

		fhq.classbook_proposal_list({
			"classbookid": kbid
		}).done(function(r){
			for (let idx = 0; idx < r.data.length; idx++) {
				const prop = r.data[idx];
				let classes = "btn btn-primary prop_open"
				if(prop.id === proposal_id) {
					classes = "btn btn-secondary"
				}

				list.append(`<div class="${classes}" kbid="${prop.classbookid}" propid="${prop.id}"> id: ${prop.id} (${prop.lang})</div>            `);
			}

			console.log('Success: ', r);

			if(r.data.length > 0) {
				$('.prop_open').unbind().bind('click', function() {
					const propid = parseInt($(this).attr('propid'),10);
					const kbid = parseInt($(this).attr('kbid'),10);
					window.fhq.changeLocationState({
						'knowledge_base': '',
						'parentid': kbid,
						'propid': propid
					});
					fhq.pages['knowledge_base']();
				})
			}
		}).fail(function(err){
			el.append('<div class="alert alert-danger">' + err.error + '</div>');
			console.error('Error:', err);
		});


		if (proposal_id === -1) {
			window['edit_kb_content'].value(r.data.content);
			$(".kbi-approve").hide();
		} else {
			fhq.classbook_proposal_info({
				"classbook_proposal_id": proposal_id
			}).done(function(r) {
				window['edit_kb_content'].value(r.data.content);
			}).fail(function(err) {
				el.append('<div class="alert alert-danger">' + err.error + '</div>');
				console.error('Error:', err);
			});
		}

		$('.kbi-approve').unbind().bind('click', function(){
			var kbid = parseInt($(this).attr('kbid'),10);
			var propid = parseInt($(this).attr('propid'),10);

			if (propid === -1) {
				return;
			}

			// classbook_propasal_approve
			fhq.classbook_propasal_approve({
				"classbook_proposal_id": propid
			}).done(function(r) {
				console.log('Success: ', r);

				fhq.classbook_proposal_delete_record({
					"classbook_proposal_id": propid
				}).done(function(r) {
					console.log('Success: ', r);
					window.fhq.changeLocationState({
						'knowledge_base': '',
						'parentid': kbid
					});
					fhq.pages['knowledge_base']();
				}).fail(function(err) {
					console.error('Error:', err);
				}).finally(function(){
					window.fhq.changeLocationState({
						'knowledge_base': '',
						'parentid': kbid
					});
					fhq.pages['knowledge_base']();
				})
			}).fail(function(err) {
				console.error('Error:', err);
			});
		});

		$('.kbi-save').unbind().bind('click', function(){
			var kbid = parseInt($(this).attr('kbid'),10);
			var propid = parseInt($(this).attr('propid'),10);
			
			if (propid === -1) {
				// classbook_update_record
				fhq.classbook_update_record({
					"classbookid": kbid,
					"name": $('#edit_kb_name').val(),
					"content": window['edit_kb_content'].value(),
					"ordered": parseInt($('#edit_kb_ordered').val(),10),
					"parentid": parseInt($('#edit_kb_parentid').val(),10),
				}).done(function(r) {
					console.log('Success: ', r);
					fhq.pages['knowledge_base']();
				}).fail(function(err){
					console.error('Error:', err);
				});
			} else {
				// classbook_propasal_update
				fhq.classbook_propasal_update({
					"classbook_proposal_id": propid,
					"content": window['edit_kb_content'].value(),
				}).done(function(r) {
					console.log('Success: ', r);
					fhq.pages['knowledge_base']();
				}).fail(function(err){
					console.error('Error:', err);
				});
			}
		});

		$('.kbi-remove').unbind().bind('click', function() {
			var kbid = parseInt($(this).attr('kbid'),10);
			var propid = parseInt($(this).attr('propid'),10);

			if(propid === -1) {
				// classbook_delete_record
				fhq.classbook_delete_record({
					"classbookid": kbid
				}).done(function(r) {
					console.log('Success: ', r);
					fhq.pages['knowledge_base']();
				}).fail(function(err){
					console.error('Error:', err);
				});
			} else {
				// classbook_proposal_delete_record
				fhq.classbook_proposal_delete_record({
					"classbook_proposal_id": propid
				}).done(function(r) {
					console.log('Success: ', r);
					window.fhq.changeLocationState({
						'knowledge_base': '',
						'parentid': kbid
					});
					fhq.pages['knowledge_base']();
				}).fail(function(err) {
					console.error('Error:', err);
				});
				
			}
		})

	}).fail(function(err){
		el.append('<div class="alert alert-danger">' + err.error + '</div>');
		console.error('Error:', err);
	});
}

fhq.pages['knowledge_base'] = function(){
	$('.nav-link.main-menu').removeClass("active");
	$('#menu_knowledge_base').addClass("active");

	$('#page_name').html('<div class="header-name">Knowledge Base</div>'
		+ ''
	);
	$('#page_content').html('');
	fhq.showLoader();
	
	var onpage = 5;
	if(fhq.containsPageParam("onpage")){
		onpage = parseInt(fhq.pageParams['onpage'], 10);
	}

	var page = 0;
	if (fhq.containsPageParam("page")) {
		page = parseInt(fhq.pageParams['page'], 10);
	}
	
	var parentid = 0;
	if (fhq.containsPageParam("parentid")) {
		parentid = parseInt(fhq.pageParams['parentid'], 10);
	}

	var propid = -1;
	if (fhq.containsPageParam("propid")) {
		propid = parseInt(fhq.pageParams['propid'], 10);
	}

	var knowledge_base_filter = '';
	if (fhq.containsPageParam("filter")) {
		knowledge_base_filter = fhq.pageParams['filter'];
	}

	var el = $("#page_content");
	el.html('Loading...')
	
	window.fhq.changeLocationState({
		'knowledge_base': '',
		'onpage': onpage,
		'page': page,
		'parentid': parentid,
		'propid': propid,
		'filter': knowledge_base_filter
	});

	fhq.ws.classbook_list({'onpage': onpage, 'page': page, 'parentid': parentid}).done(function(r){
		fhq.hideLoader();
		
		el.html('');
		if (parentid == 0) {
			el.append(''
				+ '<div>'
				+ '   <input id="knowledge_base_search" class="form-control" type="text" placeholder="Search...">'
				+ '</div><br>'
			);
			$('#knowledge_base_search').val(knowledge_base_filter);
			$('#knowledge_base_search').unbind().bind('keyup', function(e){
				var new_filter = $('#knowledge_base_search').val();
				if (new_filter != quests_filter) {
					window.fhq.changeLocationState({
						'knowledge_base': '',
						'onpage': onpage,
						'page': page,
						'filter': new_filter,
					});
					setTimeout(function() {
						if ($('#knowledge_base_search').val() == new_filter) {
							fhq.pages['knowledge_base']();
						}
					}, 500);
				}
			});
			$('#knowledge_base_search').focus();
		} else {
			el.append(''
				+ '<div id="current_kb_item">'
				+ '</div><br>'
			);
			fhqadmin.knowledge_base_load_current_item(parentid, propid);
		}
		
		el.append(''
			+ '<div class="btn btn-primary" id="knowledge_base_create"><i class="fa fa-plus"></i> New item</div> '
		);

		$('#knowledge_base_create').unbind().bind('click', function() {
			window.fhq.changeLocationState({
				'knowledge_base_create': '',
				'parentid': parentid,
			});
			fhq.pages['knowledge_base_create']();
		});

		el.append('<table class="table table-striped">'
			+ '		<thead>'
			+ '			<tr>'
			+ '				<th>#</th>'
			+ '				<th>ParentId</th>'
			+ '				<th>Name</th>'
			+ '				<th>Childs</th>'
			+ '				<th>Proposals</th>'
			+ '			</tr>'
			+ '		</thead>'
			+ '		<tbody id="knowledge_base_list">'
			+ '		</tbody>'
			+ '</table>'
		)

		for(var i in r.data){
			var kbi = r.data[i];
			console.log(kbi);
			
			$('#knowledge_base_list').append('<tr>'
				+ '	<td><div class="btn btn-link kbi-open" kbid="' + kbi.classbookid + '">[kbi#' + kbi.classbookid + ']</div></td>'
				+ '	<td><div class="btn btn-link kbi-open" kbid="' + kbi.parentid + '">[kbi#' + kbi.parentid + ']</div></td>'
				+ '	<td>' + kbi.name + '</td>'
				+ '	<td>' + kbi.childs + '</p></td>'
				+ '	<td>' + kbi.proposals + '</td>'
				+ '	<td> '
				+ '    <div class="btn btn-primary kbi-open" kbid="' + kbi.classbookid + '">Open</div>'
				+ '    <div class="btn btn-danger kbi-delete" kbid="' + kbi.classbookid + '">Delete</div>'
				+ ' </td>'
				+ '</tr>'
			)
		}

		$('.kbi-open').unbind().bind('click', function() {
			var kbid = parseInt($(this).attr('kbid'),10);
			window.fhq.changeLocationState({
				'knowledge_base': '',
				'parentid': kbid,
			});
			fhq.pages['knowledge_base']();
		})

		$('.kbi-delete').unbind().bind('click', function() {
			var kbid = parseInt($(this).attr('kbid'),10);
			fhq.classbook_delete_record({
				"classbookid": kbid
			}).done(function(r) {
				console.log('Success: ', r);
				fhq.pages['knowledge_base']();
			}).fail(function(err){
				console.error('Error:', err);
			});
		})

	}).fail(function(r){
		fhq.hideLoader();
		console.error(r);
		el.append(r.error);
	})
}

fhq.pages['knowledge_base_create'] = function() {
	$('#page_name').html('Knowledge Base Item Create');
	
	var el = $('#page_content');
	el.html('');

	var parentid = 0;
	if (fhq.containsPageParam("parentid")) {
		parentid = parseInt(fhq.pageParams['parentid'], 10);
	}

	window.fhq.changeLocationState({'knowledge_base_create':'', 'parentid': parentid});
	fhq.hideLoader();
	
	el.html('');
	el.html(''
		+ '<div class="card">'
		+ '		<div class="card-header">' + fhq.t('Knowledge Base Item Create') + '</div>'
		+ '		<div class="card-body">'
		+ '			<div class="form-group row">'
		+ '				<label for="newquest_quest_uuid" class="col-sm-2 col-form-label">Parent ID</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<input type="text" class="form-control" readonly value="' + parentid + '" id="new_knowledge_base_item_parentid">'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="newquest_quest_uuid" class="col-sm-2 col-form-label">' + fhq.t('UUID') + '</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<input type="text" class="form-control" value="' + fhq.guid() + '" id="new_knowledge_base_item_uuid">'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="newquest_name" class="col-sm-2 col-form-label">' + fhq.t('Name') + '</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<input type="text" class="form-control" value="" id="new_knowledge_base_item_name">'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="newquest_text" class="col-sm-2 col-form-label">' + fhq.t('Content') + '</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<textarea type="text" class="form-control" style="height: 150px" value="" id="new_knowledge_base_item_content"></textarea>'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label for="newquest_score" class="col-sm-2 col-form-label">' + fhq.t('Order') + '</label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<input type="number" class="form-control" id="new_knowledge_base_item_order">'
		+ '				</div>'
		+ '			</div>'
		+ '			<div class="form-group row">'
		+ '				<label class="col-sm-2 col-form-label"></label>'
		+ ' 			<div class="col-sm-10">'
		+ '					<div id="new_knowledge_base_item_error" class="alert alert-danger" style="display: none"></div>'
		+ '				</div>'
		+ '			</div>'
		+ '		</div>'
		+ '</div>'
		+ '<div class="btn btn-danger" id="quest_create"><i class="fa fa-check"></i> Create</div> '
		+ '<div class="btn btn-primary" id="quest_create_close"><i class="fa fa-times"></i> Cancel</div> '
	);

	$('#quest_create').unbind().bind('click', fhq.createKnowledgeBaseItem);
	$('#quest_create_close').unbind().bind('click', function() {
		window.fhq.changeLocationState({
			'knowledge_base': '',
			'parentid': parentid,
		});
		fhq.pages['knowledge_base']();
	});
	window['new_knowledge_base_item_content'] = new SimpleMDE({ element: $("#new_knowledge_base_item_content")[0] });
}

fhq.createKnowledgeBaseItem = function() {
	fhq.showLoader();
	var params = {};
	params["parentid"] = parseInt($("#new_knowledge_base_item_parentid").val(),10);
	params["uuid"] = $("#new_knowledge_base_item_uuid").val();
	params["name"] = $("#new_knowledge_base_item_name").val();
	params["content"] = window['new_knowledge_base_item_content'].value();
	params["ordered"] = parseInt($("#new_knowledge_base_item_order").val(),10);

	$('#new_knowledge_base_item_error').hide();
	fhq.ws.classbook_add_record(params).done(function(r){
		fhq.hideLoader();
		console.log(r);
		// fhq.loadQuest(r.questid);
		fhq.pages['knowledge_base']();
	}).fail(function(err){
		fhq.hideLoader();
		console.error(err);
		$('#new_knowledge_base_item_error').show();
		$('#new_knowledge_base_item_error').html(err.error);
		
	});
};