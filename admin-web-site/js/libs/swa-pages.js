// Framework for single web application
// Copyright sea5kg (c) 2020

function swaMakeId(length) {
    var result           = '';
    var characters       = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz';
    var charactersLength = characters.length;
    for ( var i = 0; i < length; i++ ) {
       result += characters.charAt(Math.floor(Math.random() * charactersLength));
    }
    return result;
}

class SwaPage {
    constructor() {
       this.randomPageId = swaMakeId(15);
    }
    name() {
        // nothing
        return "page";
    }

    load() {
        // nothing        
    }
};

class SwaPaginator {
    constructor(min, total, page_size, page_index) {
        this.min = min;
        this.total = total;
        this.page_size = page_size;
        this.page_index = page_index;
        this.randId = "swa_pg_" + swaMakeId(10);
    }

    getHtml() {
        var page_from = this.page_size * this.page_index;
        console.log("this.page_size: ", this.page_size);
        console.log("this.page_index: ", this.page_index);
        console.log("page_from: ", this.page_from);
        var to = this.page_size * (this.page_index + 1);
        if (to > this.total) {
            to = this.total;
        }
        return "<div class='swa-pg-container'>Items per page: [" + this.page_size + "] " 
            + page_from + "-" + to + " of " + this.total 
            + " <div class='swa-button' id='" + this.randId + "_prev'>Prev</div> "
            + "<div class='swa-button' id='" + this.randId + "_next'>Next</div> "
        + "</div>";
    }

    bindPrev(f) {
        document.getElementById(this.randId + "_prev").onclick = f;
    }

    bindNext(f) {
        document.getElementById(this.randId + "_next").onclick = f;
    }
}

class SwaMenu {
    constructor() {
    }

    onclick(e) {
        var page = e.target.getAttribute('page');
        var elems = document.getElementsByClassName("swa-menu");
        for(var i = 0; i < elems.length; i++) {
            elems[i].classList.remove('active');
        }
        e.target.classList.add('active');
        if (fhq.pages[page]) {
            fhq.pages[page]();
        } else {
            console.error("Swa: not found page '" + page + "'");
        }
    }

    init() {
        var elems = document.getElementsByClassName("swa-menu");
        console.log(elems);
        for(var i = 0; i < elems.length; i++) {
            console.log(elems[i]);
            elems[i].onclick = this.onclick
            var page = elems[i].getAttribute('page');
            if (fhq.containsPageParam(page)) {
                elems[i].classList.add('active');
            }
        }
    }
}

window.swaModalDialogs = []

function swaCloseModalDialog(modalId) {
    var newArray = []
    for (var i = 0; i < window.swaModalDialogs.length; i++) {
        var elid = window.swaModalDialogs[i];
        if (elid != modalId) {
            newArray.push(elid);
        }
    }
    window.swaModalDialogs = newArray
    // console.log("remove modal dialog: " + elid);
    var el = document.getElementById(modalId);
    if (!el) {
        console.warn("Modal dialog now found: " + modalId);
    } else {
        el.remove();
    }
}

class SwaModalDialog {
    constructor() {
        this.modalId = swaMakeId(15);
    }

    show(cnf) {
        swaModalDialogs.push(this.modalId)
        // console.log(cnf)
        var body = '';
        for (var i in cnf.body) {
            body += '<div class="swa-modal-dialog-box-content-line">' + cnf.body[i] + '</div>';
        }
        document.body.innerHTML += ''
            + '<div class="swa-modal-dialog" id="' + this.modalId + '">'
            + '  <div class="swa-modal-dialog-background"></div>'
            + '  <div class="swa-modal-dialog-box">'
            + '    <div class="swa-modal-dialog-box-head">'
            + '      <div class="swa-modal-dialog-box-title">' + cnf.title + '</div>'
            + '      <div class="swa-modal-dialog-box-close" onclick="swaCloseModalDialog(\'' + this.modalId + '\')">X</div>'
            + '    </div>'
            + '    <div class="swa-modal-dialog-box-content">' + body + '</div>'
            + '    <div class="swa-modal-dialog-box-buttons">' + cnf.buttons + ' </div>'
            + '  </div>'
            + '</div>';
    }

    close() {
        swaCloseModalDialog(this.modalId);
    }
}

function swaShowModalError(error) {
    
}

document.addEventListener("DOMContentLoaded", function(event) {
    window.swaMenu = new SwaMenu();
    swaMenu.init();
});

document.addEventListener("keyup", function(event) {
    if (event.key == "Escape" && window.swaModalDialogs.length > 0) {
        var elid = window.swaModalDialogs.pop();
        swaCloseModalDialog(elid)
        return true;
    }
});

class SwaTabs {
    constructor(tabs, activeTabId) {
        this.tabs = tabs;
        this.activeTabId = activeTabId;
        this.randId = "swa_tabs_" + swaMakeId(10);
    }

    getHtml() {
        var html = '<div class="swa-tabs" id="' + this.randId + '">';
        for (var i in this.tabs) {
            var m = this.tabs[i];
            var cl_active = this.activeTabId == m.tabid ? 'active' : '';
            html += ''
                + '  <div class="swa-tab ' + cl_active + '" onclick="' + m.onclick + '" tabid="' + m.tabId + '">'
                + '      ' + m.caption
                + '  </div>'
        }
        html += '</div>';
        return html;
    }
}