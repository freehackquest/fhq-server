import { Component, OnInit, ChangeDetectorRef, NgZone } from '@angular/core';
import { SpinnerService } from '../../services/spinner.service';
import { ActivatedRoute } from '@angular/router';
import { Router } from '@angular/router';
import { FhqService } from '../../services/fhq.service';
import { NgbModal } from '@ng-bootstrap/ng-bootstrap';
import { ModalDialogSignInComponent } from '../../dialogs/modal-dialog-sign-in/modal-dialog-sign-in.component';
import { LocaleService } from 'angular-l10n';


@Component({
  selector: 'app-knowledge-base',
  templateUrl: './knowledge-base.component.html',
  styleUrls: [
    './knowledge-base.component.css'
  ]
})

export class KnowledgeBaseComponent implements OnInit {

  public errorMessage: String = null;
  public classbookId: Number = -1;
  public articleName: String = '';
  public articleContentNaked: String = '';
  public articleContentNakedOriginal: String = '';
  public articleParents: Array<any> = [];
  public articleChilds: Array<any> = [];
  public editMode: boolean = false;
  public editLang: String = null;
  public infoMessage: String = null;

  constructor(
    public _locale: LocaleService,
    private _spinnerService: SpinnerService,
    private _route: ActivatedRoute,
    private _router: Router,
    private _modalService: NgbModal,
    private _cdr: ChangeDetectorRef,
    private _zone: NgZone,
    public _fhq: FhqService,
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
    this.articleContentNakedOriginal = this.articleContentNaked;
    this.editLang = this._locale.getCurrentLanguage();
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

  start_editing() {
    if(this._fhq.isAuthorized) {
      this.editMode = true;
    } else {
      this.openDialogSignIn()
    }
  }

  cancel_editing() {
    this.editMode = false;
    this.articleContentNaked = this.articleContentNakedOriginal;
  }

  send_editing() {
    if(this._fhq.isAuthorized) {

      this._fhq.api().classbook_proposal_add_record({
        "classbookid": this.classbookId,
        "lang": 'en'/*this.editLang*/,  // TODO: не работает в апи
        "name": this.articleName,
        "content": this.articleContentNaked
      })
      .done((r: any) => this.successSendEdition(r))
      .fail((err: any) => this.errorSendEdition(err));
    } else {
      this.openDialogSignIn()
    }
  }

  successSendEdition(r: any) {
    this.editMode = false;
    this.infoMessage = "OK, now wait for the approval of the administrator!";
    // this.update_page(this.classbookId);
    this.loadData();
  }

  errorSendEdition(err: any) {
    this.errorMessage = "Error sending propolsol: " + err.error;
    console.error(err);
  }

  openDialogSignIn() {
    const modalRef = this._modalService.open(ModalDialogSignInComponent);
    modalRef.componentInstance.name = 'SignIn';
  }

  update_page(id) {
    this._zone.run(() => this._router.navigate(['/knowledge-base', id])).then();
  }

  selectEditLang(lang) {
    this.editLang = lang;
  }
}
