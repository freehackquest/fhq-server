if(!window.fhq) window.fhq = {};
if(!fhq.pages) fhq.pages = [];

// TODO redesign
class PageUsefulLinks extends SwaPage {
    name() {
        return "useful_links";
    }

    load() {

    }
}

fhq.pages['useful_links'] = function(){
    $('.nav-link.main-menu').removeClass("active");
    $('#menu_useful_links').addClass("active");
    $('#page_name').html('Useful Links');
    $('#page_content').html('');
    fhq.showLoader();
    
    var onpage = 10;
    if (fhq.containsPageParam("page_size")) {
        onpage = parseInt(fhq.pageParams['page_size'], 10);
    }

    var page_index = 0;
    if(fhq.containsPageParam("page_index")){
        page_index = parseInt(fhq.pageParams['page_index'], 10);
    }
    
    var el = $("#page_content");
    el.html('Loading...')
    
    window.fhq.changeLocationState({'useful_links': '', 'page_size': onpage, 'page_index': page_index});

    fhq.ws.useful_links_list({'page_size': onpage, 'page_index': page_index, 'filter': ''}).done(function(r){
        fhq.hideLoader();
        console.log(r);
        el.html('');
        el.append('<button id="useful_links_add" class="swa-button">Create a new one</button>');
        $('#useful_links_add').unbind().bind('click', fhq.pages['useful_links_add']);
        el.append('<hr>');
        el.append('<input id="search" placeholder="search..." value=""></input><br>');

        var pg = new SwaPaginator(0, r.data.total, r.data.page_size, r.data.page_index);
        el.append(pg.getHtml());
        pg.bindPrev(function() {
            window.fhq.changeLocationState({'useful_links': '', 'page_size': onpage, 'page_index': page_index - 1});
            fhq.pages['useful_links']();
        });

        pg.bindNext(function() {
            window.fhq.changeLocationState({'useful_links': '', 'page_size': onpage, 'page_index': page_index + 1});
            fhq.pages['useful_links']();
        });
        
        el.append(
            `<table class="swa-table">
                <thead>
                    <tr>
                        <th>#</th>
                        <th>URL / Description</th>
                        <th>Actions</th>
                    </tr>
                </thead>
                <tbody id="list">
                </tbody>
            </table>`
        );
        for(var i in r.data.items){
            var ul = r.data.items[i];
            console.log(ul);
            $('#list').append(
                `<tr>
                    <td>` + ul.id + `</td>
                    <td>` + ul.url + `<br>` + ul.description + `</td>
                    <td>
                        <div class="swa-button useful-link-edit" useful_link_id="` + ul.id + `">Edit</div>
                        <div class="swa-button useful-link-remove" useful_link_id="` + ul.id + `">Delete</div>
                    </td>
                </tr>`
            );
        }

        $('.useful-link-edit').unbind().bind('click', function(){
            var useful_link_id = $(this).attr('useful_link_id');
            fhq.pages['useful_links_edit'](useful_link_id);
        });

        $('.useful-link-remove').unbind().bind('click', function(){
            console.warn('useful-link-remove');
            var useful_link_id = $(this).attr('useful_link_id');
            
            $('#modalInfoTitle').html('Useful link {' + useful_link_id + '} confirm deletion');
            $('#modalInfoBody').html('');
            $('#modalInfoBody').append(
                '<div class=" alert alert-danger" style="display: none" id="useful_link_delete_error"></div>'
            );
            $('#modalInfoButtons').html(''
                + '<button type="button" class="swa-button" id="useful_link_delete_btn" useful_link_id="' + useful_link_id + '" onclick="fhq.usefulLinkDelete(this);">Delete</button> '
                + '<button type="button" class="swa-button" data-dismiss="modal">Close</button>'
            );
            $('#modalInfo').modal('show');
        });
        
    }).fail(function(r){
        fhq.hideLoader();
        console.error(r);
        el.append(r.error);
    })
}

fhq.usefulLinkDelete = function(el)  {
    var useful_link_id = $(el).attr('useful_link_id');
    console.log(useful_link_id);
    fhq.showLoader();
    $('#error_info').hide();
    var data = {};
    data["useful_link_id"] = parseInt(useful_link_id, 10);

    fhq.ws.useful_links_delete(data).done(function(r){
        fhq.hideLoader();
        $('#modalInfo').modal('hide');
        fhq.pages['useful_links']();
    }).fail(function(err){
        fhq.hideLoader();
        console.error(err);
        $('#error_info').show();
        $('#error_info .alert').html('ERROR: ' + err.error);
    })
};

fhq.usefulLinksCreateAndNew = function()  {
    fhq.showLoader();
    $('#error_info').hide();
    var data = {};
    data["url"] = $("#useful_link_url").val();
    data["description"] = $("#useful_link_description").val();
    data["author"] = '';

    fhq.ws.useful_links_add(data).done(function(r){
        fhq.hideLoader();
        fhq.pages['useful_links_add']();
    }).fail(function(err){
        fhq.hideLoader();
        console.error(err);
        $('#error_info').show();
        $('#error_info .alert').html('ERROR: ' + err.error);
    })
};

fhq.usefulLinksCreateAndEdit = function()  {
    fhq.showLoader();
    $('#error_info').hide();
    var data = {};
    data["url"] = $("#useful_link_url").val();
    data["description"] = $("#useful_link_description").val();
    data["author"] = '';

    fhq.ws.useful_links_add(data).done(function(r){
        console.log(r);
        fhq.hideLoader();
        fhq.pages['useful_links_edit'](parseInt(r.data.id));
    }).fail(function(err){
        fhq.hideLoader();
        console.error(err);
        $('#error_info').show();
        $('#error_info .alert').html('ERROR: ' + err.error);
    })
};

fhq.pages['useful_links_add'] = function(){
    fhq.changeLocationState({'useful_links_add':''});
    $('#page_name').html('Add useful link');
    var el = $('#page_content');
    fhq.hideLoader();
    el.html(''
        + '<div class="card">'
        + '        <div class="card-header">Add useful link</div>'
        + '        <div class="card-body">'
        + '            <div class="form-group row">'
        + '                <label for="mail_to" class="col-sm-2 col-form-label">Link</label>'
        + '             <div class="col-sm-10">'
        + '                    <input type="email" class="form-control" value="" id="useful_link_url">'
        + '                </div>'
        + '            </div>'
        + '            <div class="form-group row">'
        + '                <label for="mail_body" class="col-sm-2 col-form-label">Description</label>'
        + '             <div class="col-sm-10">'
        + '                    <textarea class="form-control" style="height: 220px;" id="useful_link_description"></textarea>'
        + '                </div>'
        + '            </div>'
        + '            <div class="form-group row">'
        + '                <label class="col-sm-2 col-form-label"></label>'
        + '             <div class="col-sm-10">'
        + '                    <div class="swa-button" onclick="fhq.usefulLinksCreateAndNew();">Create && New</div>'
        + '                    <div class="swa-button" onclick="fhq.usefulLinksCreateAndEdit();">Create && Edit</div>'
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
}


fhq.usefulLinksUpdate = function(useful_link_id)  {
    fhq.showLoader();
    $('#error_info').hide();
    var data = {};
    data["useful_link_id"] = useful_link_id;
    data["url"] = $("#useful_link_url").val();
    data["description"] = $("#useful_link_description").val();
    data["author"] = '';

    fhq.ws.useful_links_update(data).done(function(r){
        fhq.hideLoader();
        fhq.pages['useful_links_edit'](useful_link_id);
    }).fail(function(err){
        fhq.hideLoader();
        console.error(err);
        $('#error_info').show();
        $('#error_info .alert').html('ERROR: ' + err.error);
    })
};

fhq.usefulLinksUpdateAndClose = function(useful_link_id)  {
    fhq.showLoader();
    $('#error_info').hide();
    var data = {};
    data["useful_link_id"] = useful_link_id;
    data["url"] = $("#useful_link_url").val();
    data["description"] = $("#useful_link_description").val();
    data["author"] = '';

    fhq.ws.useful_links_update(data).done(function(r){
        fhq.hideLoader();
        fhq.pages['useful_links']();
    }).fail(function(err){
        fhq.hideLoader();
        console.error(err);
        $('#error_info').show();
        $('#error_info .alert').html('ERROR: ' + err.error);
    })
};

fhq.pages['useful_links_edit'] = function(useful_link_id){
    useful_link_id = useful_link_id || fhq.pageParams['useful_links_edit'];
    if (useful_link_id) {
        useful_link_id = parseInt(useful_link_id, 10);
	}
    $('#page_name').html('Edit Useful Link [useful_link#' + useful_link_id + ']');

    fhq.changeLocationState({'useful_links_edit':useful_link_id});

    var el = $('#page_content');
    fhq.hideLoader();
    el.html(''
        + '<h3>Update useful link</h3>'
        + 'Link: <br/> <input type="text" value="" style="width: 100%" id="useful_link_url"><br/><br/>'
        + 'Description: <br/><textarea style="height: 220px; width: 100%" id="useful_link_description"></textarea><br/><br/>'
        + '<div class="swa-button" onclick="fhq.usefulLinksUpdate(' + useful_link_id + ');">Update</div> '
        + '<div class="swa-button" onclick="fhq.usefulLinksUpdateAndClose(' + useful_link_id + ');">Update && Close</div>'
        + '<div class="swa-error-alert" id="error_info" style="display: none">Some error</div>'
        + '<hr>'
        + '<h3>TAGS:</h3><br>'
        + '<div class="swa-tag-list" id="useful_links_tags"></div><br><br>'
        + '<input id="useful_links_addtag_value" type="text" placeholder="new tag name" value=""/> '
        + '<div class="swa-error-alert" style="display: none" id="useful_links_addtag_error"></div>'
        + '<hr>'
        + '<h3>COMMENTS:</h3><br>'
        + '<input type="text" value="" style="width: 100%" placeholder="Add new comment..." id="useful_link_new_comment_text"><br/><hr/>'
        + '<div id="useful_links_comments"></div>'
        + '<hr>'
    );

    $('#useful_links_addtag_value').unbind().bind('keypress', function(e) {
        if (e.which == 13) {
            $('#useful_links_addtag_error').hide();
            fhq.ws.useful_links_tag_add({
                "useful_link_id": useful_link_id,
                "tag": $('#useful_links_addtag_value').val(),
            }).done(function(r){
                console.log(r)
                $('#useful_links_addtag_value').val("")
                fhq.pages['useful_links_edit'](useful_link_id);
            }).fail(function(err) {
                console.log(err)
                $('#useful_links_addtag_error').html(err.error);
                $('#useful_links_addtag_error').show();
            })
        }
    })

    function useful_link_tag_remove(e) {
        var tagval = $(this).attr('tagval');
        fhq.ws.useful_links_tag_delete({
            "useful_link_id": useful_link_id,
            "tag": tagval,
        }).done(function(r){
            console.log(r)
            fhq.pages['useful_links_edit'](useful_link_id);
        }).fail(function(err) {
            console.log(err)
            $('#useful_links_addtag_error').html(err.error);
            $('#useful_links_addtag_error').show();
        })
    }

    
    $('#useful_link_new_comment_text').unbind().bind('keypress', function(e) {
        if (e.which == 13) {
            var val = $('#useful_link_new_comment_text').val();
            fhq.ws.useful_links_comment_add({
                "useful_link_id": useful_link_id,
                "comment": val,
            }).done(function(r){
                console.log(r)
                fhq.pages['useful_links_edit'](useful_link_id);
            }).fail(function(err) {
                console.log(err)
                $('#useful_links_addtag_error').html(err.error);
                $('#useful_links_addtag_error').show();
            })
        }
    });

    fhq.ws.useful_links_retrieve({
        "useful_link_id": useful_link_id
    }).done(function(r){
        console.log(r)
        $('#useful_link_url').val(r.data.url)
        $('#useful_link_description').html(r.data.description)
        $('#useful_links_tags').html('');
        for (var i in r.data.tags) {
            $('#useful_links_tags').append(''
                + '<div class="swa-tag-item">' + r.data.tags[i] + ' '
                + '<div class="swa-tag-item-remove tag-remove" tagval="' + r.data.tags[i] + '">X</div>'
                + '</div>'
            );
        }

        $('.tag-remove').unbind().bind('click', useful_link_tag_remove)

    }).fail(function(err) {
        console.log(err)
    })

    function deleteComment(comment_id) {
        $('.useful-comments-delete-comment-error').hide();
        fhq.ws.useful_links_comment_delete({
            "useful_link_comment_id": comment_id,
        }).done(function(r){
            console.log(r)
            fhq.pages['useful_links_edit'](useful_link_id);
        }).fail(function(err) {
            console.log(err)
            $('.useful-comments-delete-comment-error[comment_id=' + comment_id + ']').html(err.error);
            $('.useful-comments-delete-comment-error[comment_id=' + comment_id + ']').show();
        })
    }

    fhq.ws.useful_links_comment_list({
        "useful_link_id": useful_link_id
    }).done(function(r){
        console.log(r);
        $('#useful_links_comments').html('');
        for (var i in r.data) {
            var c = r.data[i];
            $('#useful_links_comments').append(
                '<div class="swa-card">'
                + 'Identifier: ' + c.id + ' <br/>'
                + 'User: [' + c.user.university + '] ' + c.user.nick + ' <br/>'
                + 'Date & Time: ' + new Date(c.dt + "Z") + ' <br/></br>'
                + 'Comment: ' + c.comment + ' <br/></br>'
                + '<div class="swa-button useful-comments-delete-comment" comment_id=' + c.id + ' >Delete</div> '
                + '<div class="swa-error-alert useful-comments-delete-comment-error" style="display: none;" comment_id=' + c.id + ' ></div> '
                // + '<div class="swa-button">TODO: Issue a warning to a user for violation</div> '
                + '</div>'
            );
            
        }

        $('.useful-comments-delete-comment').unbind().bind('click', function(e) {
            var comment_id = $(e.target).attr("comment_id");
            comment_id = parseInt(comment_id, 10);
            deleteComment(comment_id);
            console.log("comment_id=", comment_id)
        })
    }).fail(function(err){
        console.error(err);
        $('#error_info').show();
        $('#error_info .alert').html('ERROR: ' + err.error);
    })
}