import { Directive, Component, OnInit, ChangeDetectorRef, NgZone } from '@angular/core';
import { SpinnerService } from '../../services/spinner.service';
import { ActivatedRoute } from '@angular/router';
import { Router } from '@angular/router';
import * as marked from 'marked';

declare var fhq: any;

@Component({
  selector: 'app-knowledge-base',
  templateUrl: './knowledge-base.component.html',
  styleUrls: ['./knowledge-base.component.css']
})
@Directive({
  selector: '[appMarked]'
})
export class KnowledgeBaseComponent implements OnInit {

  errorMessage: String = null;
  classbookId: Number = -1;
  articleName: String = '';
  articleContent: String = '';
  articleParents: Array<any> = [];
  articleChilds: Array<any> = [];

  constructor(
    private _spinnerService: SpinnerService,
    private _route: ActivatedRoute,
    private _router: Router,
    private _cdr: ChangeDetectorRef,
    private _zone: NgZone,
  ) {
      // nothing
  }
  
  ngOnInit() {
    console.log("ngOnInit");
    this._route.params.subscribe( (params) => this.loadData(params));
  }

  loadData(params: any) {
    // console.log(params['id']);
    if (!params['id']) {
      this._router.navigate(['/knowledge-base', 0]);
      return;
    }
    this.classbookId = parseInt(params['id'], 10);
    const _data = {
      'classbookid': this.classbookId
    }
    this._spinnerService.show();
    if (this.classbookId != 0) {
      fhq.classbook_info(_data)
      .done((r: any) => this.successResponse(r))
      .fail((err: any) => this.errorResponse(err));
    } else {
      this.articleName = "Root";
      this.loadChilds();
    }
  }

  successResponse(r: any) {
    // console.log(r);
    this.articleName = `#${this.classbookId} ${r.data.name}`;
    this.articleContent = marked(r.data.content);
    this.articleParents = []
    r.data.parents.forEach((el: any) => {
      this.articleParents.push(el);
    });
    this.articleParents.reverse();
    let curr_el : any = {}
    curr_el['classbookid'] = r.data.classbookid;
    curr_el['name'] = r.data.name;
    this.articleParents.push(curr_el);
    this.loadChilds();
  }

  errorResponse(err: any) {
    this._spinnerService.hide();
    if (err.code == 404) {
      this.errorMessage = 'Not found article #' + this.classbookId;
    } else {
      this.errorMessage = err.error;
    }
    this._cdr.detectChanges();
    console.error(err);
  }

  loadChilds() {
    const _data = {
      'parentid': this.classbookId
    }
    fhq.classbook_list(_data)
    .done((r: any) => this.successChildsResponse(r))
    .fail((err: any) => this.errorChildsResponse(err))
  }

  successChildsResponse(r: any) {
    this._spinnerService.hide();
    this.articleChilds = []
    r.data.forEach((el: any) => {
      this.articleChilds.push(el);
    });
    this._cdr.detectChanges();
  }

  errorChildsResponse(err: any) {
    this._spinnerService.hide();
    if (err.code == 404) {
      this.errorMessage = 'Not found childs for #' + this.classbookId;
    } else {
      this.errorMessage = err.error;
    }
    this._cdr.detectChanges();
    console.error(err);
  }

  openArticle(id: number) {
    // reset
    this.errorMessage = null;
    this.articleName = '';
    this.articleContent = '';
    this.articleParents = [];
    this.articleChilds = [];
    this._zone.run(() => this._router.navigate(['/knowledge-base', id])).then();
  }

}
