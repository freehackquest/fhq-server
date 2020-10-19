if(!window.fhq) window.fhq = {};
if(!fhq.pages) fhq.pages = [];

fhq.pages['quests_proposal'] = function(){
    $('.nav-link.main-menu').removeClass("active");
    $('#menu_quests_proposal').addClass("active");

    $('#page_name').html('Quests Proposal');
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
    
    window.fhq.changeLocationState({'quests_proposal': '', 'onpage': onpage, 'page': page});

    fhq.ws.quests_proposal_list({'onpage': onpage, 'page': page}).done(function(r){
        fhq.hideLoader();
        console.log(r);
        el.html('');
        
        el.append('<hr>');
        
        // var pg = new SwaPaginator(0, r.data.total, r.data.page_size, r.data.page_index);
        var pg = new SwaPaginator(0, r.count, r.onpage, r.page);
        el.append(pg.getHtml());
        pg.bindPrev(function() {
            window.fhq.changeLocationState({'quests_proposal': '', 'page_size': onpage, 'page_index': page_index - 1});
            fhq.pages['quests_proposal']();
        });

        pg.bindNext(function() {
            window.fhq.changeLocationState({'quests_proposal': '', 'page_size': onpage, 'page_index': page_index + 1});
            fhq.pages['quests_proposal']();
        });

        el.append('<table class="swa-table">'
            + '        <thead>'
            + '            <tr>'
            + '                <th>#</th>'
            + '                <th>User</th>'
            + '                <th>Quest</th>'
            + '                <th>Confirmed</th>'
            + '                <th>Action</th>'
            + '            </tr>'
            + '        </thead>'
            + '        <tbody id="list">'
            + '        </tbody>'
            + '</table>'
        )
        for(var i in r.data){
            var qp = r.data[i];
            console.log(qp);
            $('#list').append('<tr>'
                + '    <td> ' + qp.id + '</td>'
                + '    <td> '
                + '        <p> UserID: ' + qp.userid + '</p>'
                + '        <p> Email: ' + qp.email + '</p>'
                + '        <p> Nick: ' + qp.nick + '</p>'
                + '</td>'
                + '    <td> '
                + '        <p> GameID: ' + qp.gameid + '</p>'
                + '        <p> Game Title: ' + qp.game_title + '</p>'
                + '        <p> Subject: ' + qp.subject + '</p>'
                + '        <p> Name: ' + qp.name + '</p>'
				+ '        <p> Created: ' + qp.created + '</p>'
				+ '        <p> Text: </p>'
				+ '        <pre>' + qp.text + '</pre>'
                + '</td>'
                + '    <td> ' + ( qp.confirmed == 0 ? 'pending' : '?') + '</td>'
				+ '    <td>'
				// + '        <p><button class="swa-button quest-proposal-text" gameuuid="' + qp.uuid + '">Edit</button></p>'
                // + '        <p><button class="swa-button quest-proposal-open" gameuuid="' + qp.uuid + '">Edit</button></p>'
                // + '        <p><button class="swa-button quest-proposal-remove" gameuuid="' + qp.uuid + '">Remove</button></p>'
                + '</td>'
                + '</tr>'
            );
        }

    }).fail(function(r){
        fhq.hideLoader();
        console.error(r);
        el.append(r.error);
    })
}
