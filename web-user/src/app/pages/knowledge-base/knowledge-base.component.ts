import { Component, OnInit, ChangeDetectorRef, NgZone } from '@angular/core';
import { SpinnerService } from '../../services/spinner.service';
import { ActivatedRoute } from '@angular/router';
import { Router } from '@angular/router';
import { FhqService } from '../../services/fhq.service';
import { L10nTranslationService } from 'angular-l10n';
import { DialogsService } from '../../services/dialogs.service';

@Component({
  selector: 'app-knowledge-base',
  templateUrl: './knowledge-base.component.html',
  styleUrls: [
    './knowledge-base.component.css'
  ]
})

export class KnowledgeBaseComponent implements OnInit {

  errorMessage: string = null;
  classbookId: Number = -1;
  articleName: string = '';
  articleContentNaked: string = '';
  articleContentNakedOriginal: string = '';
  articleParents: Array<any> = [];
  articleChilds: Array<any> = [];
  editLang: string = null;
  infoMessage: string = null;

  constructor(
    public _translation: L10nTranslationService,
    private _spinnerService: SpinnerService,
    private _route: ActivatedRoute,
    private _router: Router,
    private _cdr: ChangeDetectorRef,
    private _zone: NgZone,
    public _fhq: FhqService,
    public _dialogs: DialogsService,
  ) {
      // nothing
  }

  ngOnInit() {
    console.log("ngOnInit");
    this._route.params.subscribe( (params) => {
      if (!params['id']) {
        this._router.navigate(['/knowledge-base', 0]);
        return;
      }
      this.classbookId = parseInt(params['id'], 10);
      this.loadData();
    });
  }

  loadData() {
    const _data = {
      'classbookid': this.classbookId
    }
    this._spinnerService.show();
    if (this.classbookId != 0) {
      this._fhq.api().classbook_info(_data)
      .done((r: any) => this.successResponse(r))
      .fail((err: any) => this.errorResponse(err));
    } else {
      this.articleName = "Root";
      this.loadChilds();
    }
  }

  successResponse(r: any) {
    console.log(r);
    this.articleName = `#${this.classbookId} ${r.data.name}`;
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
    this.articleContentNaked = r.data.content;
    console.log("r.data.content", r.data.content);
    this.articleContentNakedOriginal = this.articleContentNaked;
    console.log("this.articleContentNaked", this.articleContentNaked);
    this.editLang = this._translation.getLocale().language;
  }

  errorResponse(err: any) {
    console.error(err);
    this._spinnerService.hide();
    if (err.code == 404) {
      this.errorMessage = 'Not found article #' + this.classbookId;
    } else {
      this.errorMessage = err.error;
    }
    this._cdr.detectChanges();
  }

  loadChilds() {
    const _data = {
      'parentid': this.classbookId
    }
    this._fhq.api().classbook_list(_data)
    .done((r: any) => this.successChildsResponse(r))
    .fail((err: any) => this.errorChildsResponse(err))
  }

  successChildsResponse(r: any) {
    console.log(r)
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
    this.articleContentNaked = '';
    this.articleParents = [];
    this.articleChilds = [];
    this.update_page(id);
  }

  update_page(id) {
    this._zone.run(() => this._router.navigate(['/knowledge-base', id])).then();
  }

  selectEditLang(lang) {
    this.editLang = lang;
  }
}
