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
    
    window.fhq.changeLocationState({'useful_links': '', 'page_size': onpage, 'page_index': page});

    fhq.ws.useful_links_list({'page_size': onpage, 'page_index': page, 'filter': ''}).done(function(r){
        fhq.hideLoader();
        console.log(r);
        el.html('');
        
        el.append('<button id="useful_links_add" class="btn btn-secondary">Add</button>');
        $('#useful_links_add').unbind().bind('click', fhq.pages['useful_links_add']);
        el.append('<hr>');

        el.append(fhq.paginator(0, r.data.total, r.data.page_size, r.page_index));
        el.append(
            `<table class="table table-striped">
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
        )
        for(var i in r.data.items){
            var ul = r.data.items[i];
            console.log(ul);
            $('#list').append(
                `<tr>
                    <td>` + ul.id + `</td>
                    <td>` + ul.url + `<br>` + ul.description + `</td>
                    <td>
                        <div class="btn btn-primary useful-link-edit" useful_link_id="` + ul.id + `">Edit</div>
                        <div class="btn btn-secondary useful-link-remove" useful_link_id="` + ul.id + `">Delete</div>
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
                + '<button type="button" class="btn btn-secondary" id="useful_link_delete_btn" useful_link_id="' + useful_link_id + '" onclick="fhq.usefulLinkDelete(this);">Delete</button> '
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
        + '                    <div class="btn btn-secondary" onclick="fhq.usefulLinksCreateAndNew();">Create && New</div>'
        + '                    <div class="btn btn-secondary" onclick="fhq.usefulLinksCreateAndEdit();">Create && Edit</div>'
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
        + '<div class="card">'
        + '     <div class="card-header">Update useful link</div>'
        + '     <div class="card-body">'
        + '         <div class="form-group row">'
        + '             <label for="mail_to" class="col-sm-2 col-form-label">Link</label>'
        + '          <div class="col-sm-10">'
        + '                 <input type="email" class="form-control" value="" id="useful_link_url">'
        + '             </div>'
        + '         </div>'
        + '         <div class="form-group row">'
        + '             <label for="mail_body" class="col-sm-2 col-form-label">Description</label>'
        + '          <div class="col-sm-10">'
        + '                 <textarea class="form-control" style="height: 220px;" id="useful_link_description"></textarea>'
        + '             </div>'
        + '         </div>'
        + '         <div class="form-group row">'
        + '             <label class="col-sm-2 col-form-label"></label>'
        + '          <div class="col-sm-10">'
        + '                 <div class="btn btn-secondary" onclick="fhq.usefulLinksUpdate(' + useful_link_id + ');">Update</div>'
        + '                 <div class="btn btn-secondary" onclick="fhq.usefulLinksUpdateAndClose(' + useful_link_id + ');">Update && Close</div>'
        + '             </div>'
        + '         </div>'
        + '         <div class="form-group row" id="error_info" style="display: none">'
        + '             <label class="col-sm-2 col-form-label"></label>'
        + '          <div class="col-sm-10">'
        + '                 <div class="alert alert-danger"></div>'
        + '             </div>'
        + '         </div>'
        + '     </div>'
        + '</div>'
        + '<hr>'
        + '<h2>TAGS:</h2><br>'
        + '<div id="useful_links_tags"></div><br><br>'
        + '<input id="useful_links_addtag_value" type="text" value=""/>'
        + '<button class="btn btn-default" id="useful_links_addtag_btn">Add</button>'
        + '<div class="alert alert-danger" style="display: none" id="useful_links_addtag_error"></div>'
        + '<hr>'
    );

    $('#useful_links_addtag_btn').unbind().bind('click', function() {
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

    fhq.ws.useful_links_retrieve({
        "useful_link_id": useful_link_id
    }).done(function(r){
        console.log(r)
        $('#useful_link_url').val(r.data.url)
        $('#useful_link_description').html(r.data.description)
        $('#useful_links_tags').html('');
        for (var i in r.data.tags) {
            $('#useful_links_tags').append(''
                + '<div class="tag-item">' + r.data.tags[i] + ' '
                + '<div class="tag-item-remove tag-remove" tagval="' + r.data.tags[i] + '">-</div>'
                + '</div>'
            );
        }

        $('.tag-remove').unbind().bind('click', useful_link_tag_remove)

    }).fail(function(err) {
        console.log(err)
    }) 
}