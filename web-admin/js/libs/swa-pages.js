// Framework for single web app

class SwaPage {
    constructor() {
    }
    name() {
        // nothing
        return "page";
    }

    load() {
        // nothing        
    }
};

function swaMakeId(length) {
    var result           = '';
    var characters       = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz';
    var charactersLength = characters.length;
    for ( var i = 0; i < length; i++ ) {
       result += characters.charAt(Math.floor(Math.random() * charactersLength));
    }
    return result;
}

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
            + " <div class='swa-pg-prev' id='" + this.randId + "_prev'>Prev</div> "
            + "<div class='swa-pg-next' id='" + this.randId + "_next'>Next</div> "
        + "</div>";
    }

    bindPrev(f) {
        document.getElementById(this.randId + "_prev").onclick = f;
    }

    bindNext(f) {
        document.getElementById(this.randId + "_next").onclick = f;
    }
}