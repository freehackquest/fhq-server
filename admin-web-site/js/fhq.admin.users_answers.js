if(!window.fhq) window.fhq = {};
if(!fhq.pages) fhq.pages = [];




fhq.pages['users_answers'] = function(){
	$('.nav-link.main-menu').removeClass("active");
	$('#menu_users_answers').addClass("active");

	$('#page_name').html('<div class="header-name">Users Answers</div>'
		+ ''
	);
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
	
	window.fhq.changeLocationState({'users_answers': '', 'onpage': onpage, 'page': page});

	fhq.ws.answerlist({'onpage': onpage, 'page': page}).done(function(r){
		fhq.hideLoader();
		
		el.html('');
		el.append(fhq.paginator(0, r.count, r.onpage, r.page));
		el.append('<table class="table table-striped">'
			+ '		<thead>'
			+ '			<tr>'
			+ '				<th>DT</th>'
			+ '				<th>Quest</th>'
			+ '				<th>Answer</th>'
			+ '				<th>Passed</th>'
			+ '				<th>User</th>'
			+ '			</tr>'
			+ '		</thead>'
			+ '		<tbody id="users_asnwers_list">'
			+ '		</tbody>'
			+ '</table>'
		)
		fhq.makeUserIcon = function(userid, logo, nick, university) {
			return '<div class="btn btn-default" onclick="fhq.ui.showUserInfo(' + userid + ')"> '
				+ '<img class="fhqmiddelinner" width=25px src="' + logo + '"/> ' + (university && university != "" ? '[' + university + '] ' : '') + nick + '</div>'
		}

		for(var i in r.data){
			var uqa = r.data[i];
			console.log(uqa);
			
			$('#users_asnwers_list').append('<tr>'
				+ '	<td><p>' + uqa.dt + '</td>'
				+ '	<td><p>' + uqa.quest.subject + ' / Quest ' + uqa.quest.id + '<br>' + uqa.quest.name + ' (+' + uqa.quest.score + ')</p></td>'
				+ '	<td><p>' + 'User: ' + uqa.user_answer + '<br> Quest: ' + uqa.quest_answer + '<br> Levenshtein: ' + uqa.levenshtein + '</p></td>'
				+ '	<td><p>' + uqa.passed + '</p></td>'
				+ '	<td><p>' + fhq.makeUserIcon(uqa.user.id, uqa.user.logo, uqa.user.nick) + '</p></td>'
				+ '</tr>'
			)
		}
	}).fail(function(r){
		fhq.hideLoader();
		console.error(r);
		el.append(r.error);
	})
}

