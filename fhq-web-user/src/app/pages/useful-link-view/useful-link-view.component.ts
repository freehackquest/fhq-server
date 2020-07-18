import { Component, OnInit, ChangeDetectorRef } from '@angular/core';
import { SpinnerService } from '../../services/spinner.service';
import { Router, ActivatedRoute } from '@angular/router';
import { FhqService } from '../../services/fhq.service';
import { Location } from '@angular/common';

@Component({
  selector: 'app-useful-link-view',
  templateUrl: './useful-link-view.component.html',
  styleUrls: ['./useful-link-view.component.css']
})
export class UsefulLinkViewComponent implements OnInit {
  usefullinkid: number = 0;
  subscription: any = null;
  element: any = {};

  constructor(
    private _spinner: SpinnerService,
    private _cdr: ChangeDetectorRef,
    private _router: Router,
    public _fhq: FhqService,
    private _location: Location,
    private _activatedRoute: ActivatedRoute,
  ) { }

  ngOnInit(): void {
    this._spinner.hide();
    this._activatedRoute.params.subscribe( (params) => {
      if (!params['id']) {
        // this._router.navigate(['/quests']);
        return;
      }
      this.usefullinkid = parseInt(params['id'],10);
      // TODO check possible subjects
      

      if (this._fhq.connectionState == "OK") {
        this.loadData();
      }
      this.subscription = this._fhq.changedState
        .subscribe(() => this.loadData());
    });
  }

  ngOnDestroy() {
    if (this.subscription != null) {
      this.subscription.unsubscribe();
    }
  }

  successUsefulLinksRetrieve(r) {
    this._spinner.hide();
    console.log(r)
    this.element = r.data
  }

  errorUsefulLinksRetrieve(err) {
    this._spinner.hide();
    console.log(err)
  }

  loadData() {
    // console.log(this._fhq.connectionState);
    this._fhq.api().useful_links_retrieve({
      "useful_link_id": this.usefullinkid
    })
      .done((r: any) => this.successUsefulLinksRetrieve(r))
      .fail((err: any) => this.errorUsefulLinksRetrieve(err));
  }

  successUsefulLinkClicked(r: any) {
    console.log("successUsefulLinkClicked", r);
    this.loadData()
  }

  errorUsefulLinkClicked(err: any) {
    console.error("errorUsefulLinkClicked", err);
    this._spinner.hide();
  }

  linkClicked(id: number) {
    // console.log("linkClicked", id );
    this._fhq.api().useful_links_clicked({
      "useful_link_id": id
    })
      .done((r: any) => this.successUsefulLinkClicked(r))
      .fail((err: any) => this.errorUsefulLinkClicked(err));
  }

  successUsefulLinksFavorite(r: any) {
    this.loadData()
  }

  errorUsefulLinksFavorite(err: any) {
    console.error("errorResponse: ", err);
    this._spinner.hide();
    // this.resultOfChangePassword = err.error;
    // this._cdr.detectChanges();
    
  }

  addToFavorite(id: number) {
    console.log("addToFavorite", id );
    if (this._fhq.isAuthorized) {

    }
    this._spinner.show();
    this._fhq.api().useful_links_user_favorite({
      "useful_link_id": id
    })
      .done((r: any) => this.successUsefulLinksFavorite(r))
      .fail((err: any) => this.errorUsefulLinksFavorite(err));
  }

  successUsefulLinksUserUnfavorite(r: any) {
    this.loadData()
  }

  errorUsefulLinksUserUnfavorite(err: any) {
    console.error("errorResponse: ", err);
    this._spinner.hide();
    // this.resultOfChangePassword = err.error;
    // this._cdr.detectChanges();
  }

  removeFromFavorite(id: number) {
    console.log("removeFromFavorite", id );
    this._spinner.show();
    this._fhq.api().useful_links_user_unfavorite({
      "useful_link_id": id
    })
      .done((r: any) => this.successUsefulLinksUserUnfavorite(r))
      .fail((err: any) => this.errorUsefulLinksUserUnfavorite(err));
  }
}
