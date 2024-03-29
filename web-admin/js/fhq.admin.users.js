if(!window.fhq) window.fhq = {};
if(!fhq.pages) fhq.pages = [];

class PageUsers extends SwaPage {
    name() {
        return "users";
    }

    load() {

    }
}

fhq.createUser = function()  {
    fhq.showLoader();
    $('#error_info').hide();
    var data = {};
    data["uuid"] = $("#newuser_uuid").val();
    data["role"] = $("#newuser_role").val();
    data["email"] = $("#newuser_login").val();
    data["password"] = $("#newuser_password").val();
    data["nick"] = $("#newuser_nick").val();
    data["university"] = $("#newuser_university").val();
    
    fhq.ws.users_add(data).done(function(r){
        fhq.hideLoader();
        fhq.pages['users']();
    }).fail(function(err){
        fhq.hideLoader();
        console.error(err);
        $('#error_info').show();
        $('#error_info .alert').html('ERROR: ' + err.error);
    });
};

fhqUserDelete = function(dialogid, userid, uuid) {
    fhq.showLoader();
    $('#error_info').hide();
    var data = {};
    data["uuid"] = uuid;
    data["userid"] = parseInt(userid);
    data["password"] = $("#user_delete_admin_password").val();
    fhq.ws.users_delete(data).done(function(r){
        fhq.hideLoader();
        fhq.pages['users']();
        swaCloseModalDialog(dialogid);
    }).fail(function(err){
        fhq.hideLoader();
        console.error(err);
        user_delete_error.style.display = 'block';
        user_delete_error.innerHTML = err.error;
    });
}

function fhqUserConfirmDeletion(el, userid, uuid) {
    var modal = new SwaModalDialog();
    modal.show({
        title: 'User confirm deletion',
        body: [
            'User {' + uuid + '} will be deleted.',
            'Admin password: <input id="user_delete_admin_password" type="password"/>',
            '<div class="swa-error-alert" style="display: none" id="user_delete_error"></div>',
        ],
        buttons: ''
            + '<button type="button" class="swa-button" '
            + '   onclick="fhqUserDelete(\'' + modal.modalId + '\', \'' + userid + '\', \'' + uuid + '\');">Delete</button> '
            + '<button type="button" class="swa-button" onclick="swaCloseModalDialog(\'' + modal.modalId + '\');">Close</button>',
    });
}

fhq.pages['user_create'] = function(){
    fhq.changeLocationState({'user_create':''});
    $('#page_name').html('User Create');
    var el = $('#page_content');
    fhq.hideLoader();
    el.html(''
        + '<div class="swa-card">'
        + '        <h3>New user</h3>'
        + '        <div class="card-body">'
        + '            <div class="form-group row">'
        + '                <label for="newuser_role" class="col-sm-2 col-form-label">Role</label>'
        + '             <div class="col-sm-10">'
        + '                    <select class="form-control" value="" id="newuser_role">'
        + '                        <option value="user">User</option>'
        + '                        <option value="admin">Admin</option>'
        + '                    </select>'
        + '                </div>'
        + '            </div>'
        + '            <div class="form-group row">'
        + '                <label for="newuser_uuid" class="col-sm-2 col-form-label">UUID</label>'
        + '             <div class="col-sm-10">'
        + '                    <input type="text" class="form-control" value="" id="newuser_uuid">'
        + '                </div>'
        + '            </div>'
        + '            <div class="form-group row">'
        + '                <label for="newuser_login" class="col-sm-2 col-form-label">Email or login</label>'
        + '             <div class="col-sm-10">'
        + '                    <input type="text" class="form-control" value="" id="newuser_login">'
        + '                </div>'
        + '            </div>'
        + '            <div class="form-group row">'
        + '                <label for="newuser_password" class="col-sm-2 col-form-label">Password</label>'
        + '             <div class="col-sm-10">'
        + '                    <input type="text" class="form-control" value="" id="newuser_password">'
        + '                </div>'
        + '            </div>'
        + '            <div class="form-group row">'
        + '                <label for="newuser_nick" class="col-sm-2 col-form-label">Nick</label>'
        + '             <div class="col-sm-10">'
        + '                    <input type="text" class="form-control" value="" id="newuser_nick">'
        + '                </div>'
        + '            </div>'
        + '            <div class="form-group row">'
        + '                <label for="newuser_university" class="col-sm-2 col-form-label">University (optional)</label>'
        + '             <div class="col-sm-10">'
        + '                    <input type="text" class="form-control" value="" id="newuser_university">'
        + '                </div>'
        + '            </div>'
        + '            <div class="form-group row">'
        + '                <label class="col-sm-2 col-form-label"></label>'
        + '             <div class="col-sm-10">'
        + '                    <div class="btn btn-secondary" onclick="fhq.createUser();">Create</div>'
        + '                </div>'
        + '            </div>'
        + '            <div class="form-group row" id="error_info" style="display: none">'
        + '                <label class="col-sm-2 col-form-label"></label>'
        + '             <div class="col-sm-10">'
        + '                    <div class="alert alert-danger"></div>'
        + '                </div>'
        + '            </div>'
        + '        </div>'
        + '</div>'
    );
    $('#newuser_uuid').val(fhq.guid());
}

fhq.pages['users'] = function(){
    $('.nav-link.main-menu').removeClass("active");
    $('#menu_users').addClass("active");

    $('#page_name').html('Users');
    $('#page_content').html('');
    fhq.showLoader();
    
    var onpage = 5;
    if (fhq.containsPageParam("onpage")) {
        onpage = parseInt(fhq.pageParams['onpage'], 10);
    }

    var page = 0;
    if (fhq.containsPageParam("page")) {
        page = parseInt(fhq.pageParams['page'], 10);
    }

    var filter_text = '';
    if (fhq.containsPageParam("filter_text")) {
        filter_text = fhq.pageParams['filter_text'];
    }
    
    var el = $("#page_content");
    el.html('Loading...')
    
    window.fhq.changeLocationState({'users': '', 'onpage': onpage, 'page': page, 'filter_text': filter_text});

    fhq.ws.users({'onpage': onpage, 'page': page, 'filter_text': filter_text}).done(function(r){
        fhq.hideLoader();
        el.html('');
        el.append(''
            + '<div>'
            + '   <input id="users_search" type="text" placeholder="Search...">'
            + '</div><br>'
        );
        $('#users_search').val(filter_text);
        $('#users_search').off().on('keyup', function(e){
            var new_filter = $('#users_search').val();
            if (new_filter != filter_text) {
                window.fhq.changeLocationState({
                    'users': '',
                    'onpage': onpage,
                    'page': page,
                    'filter_text': new_filter,
                });
                setTimeout(function() {
                    if ($('#users_search').val() == new_filter) {
                        fhq.pages['users']();
                    }
                }, 500);
            }
        });
        $('#users_search').focus();

        el.append('<button id="user_create" class="swa-button">Create User</button><hr>');
        $('#user_create').off().bind('click', fhq.pages['user_create']);

        el.append(fhq.paginator(0, r.count, r.onpage, r.page));
        el.append('<table class="swa-table">'
            + '        <thead>'
            + '            <tr>'
            + '                <th>#</th>'
            + '                <th>Email / Nick</th>'
            + '                <th>Last IP <br> Country / City / University</th>'
            + '                <th>Last Sign in <br> Status / Role</th>'
            + '                <th>Actions</th>'
            + '            </tr>'
            + '        </thead>'
            + '        <tbody id="users_list">'
            + '        </tbody>'
            + '</table>'
        )
        for(var i in r.data){
            var u = r.data[i];
            $('#users_list').append('<tr>'
                + '    <td><p>' + u.id + '</td>'
                + '    <td><p>' + u.email + '</p><p>'  + u.nick + '</p></td>'
                + '    <td><p>' + u.last_ip + '</p><p>' + u.country + ' / ' + u.city + ' / ' + u.university + '</p></td>'
                + '    <td><p>' + u.dt_last_login + '</p><p>' + '' + u.role + '</p></td>'
                + '    <td><button class="swa-button" onclick="fhqUserConfirmDeletion(this, \'' + u.id + '\', \''+ u.uuid + '\')">Delete</button></td>'
                + '</tr>'
            )
        }
 
        el.append('<div class="form-group row" id="error_info" style="display: none">'
        + '             <div class="col-sm-10">'
        + '                    <div class="alert alert-danger"></div>'
        + '                </div>'
        + '            </div>');
        
    }).fail(function(r){
        fhq.hideLoader();
        console.error(r);
        el.append(r.error);
    })
}

