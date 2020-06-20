import { Component, OnInit, ChangeDetectorRef, ViewChild, ElementRef } from '@angular/core';
import { FhqService } from '../../services/fhq.service';
// import { FreeHackQuestClient } from '../../services/libfhqcli-web-js.service';
import { SpinnerService } from '../../services/spinner.service';
import { Router } from '@angular/router';
import { NgbModal } from '@ng-bootstrap/ng-bootstrap';
import { ModalDialogSignInComponent } from '../../dialogs/modal-dialog-sign-in/modal-dialog-sign-in.component';
import { MatTableDataSource } from '@angular/material';

export interface UserTokensElement {
  id: number;
  token: string;
  status: string;
  startDate: Date;
  endDate: Date;
}

@Component({
  selector: 'app-user-tokens',
  templateUrl: './user-tokens.component.html',
  styleUrls: ['./user-tokens.component.css']
})
export class UserTokensComponent implements OnInit {
  errorMessage: string = null;
  subscription: any;
  userId: number = 0;
  dataSource = new MatTableDataSource<UserTokensElement>();
  userTokensData: UserTokensElement[] = [];
  displayedColumns: string[] = ['idToken', 'tokenValue', 'tokenStatus', 'deleteToken'];

  constructor(
    private _spinner: SpinnerService,
    private _router: Router,
    private _modalService: NgbModal,
    private _fhq: FhqService,
    // private _fhq2: FreeHackQuestClient,
    private _cdr: ChangeDetectorRef,
    private _spinnerService: SpinnerService,
  ) { }

  ngOnInit() {
    this.updatePage();
    this.subscription = this._fhq.changedState
      .subscribe(() => this.updatePage());
  }

  ngOnDestroy() {
	  this.subscription.unsubscribe();
  }

  updatePage() {
    //this._fhq2.connectToServer({"baseUrl": "ws://localhost:1234/"});

    if (this._fhq.isAuthorized) {
      this.userId = parseInt(this._fhq.userdata.id, 10);
      this._cdr.detectChanges();
      this.updateListOfTokens();
    } else {
      this.userId = 0;
      this._cdr.detectChanges();
      this._spinnerService.hide();
    }
  }

  successUserTokens(r: any) {
    this._spinner.hide();
    this.userTokensData = [];
    for (let i in r.data) {
      let userToken = r.data[i];
      this.userTokensData.push({
        id: userToken['id'],
        token: userToken['token'],
        status: userToken['status'],
        startDate: new Date(userToken['start_date']),
        endDate: new Date(userToken['end_date'])
      })
    }
    this.dataSource = new MatTableDataSource<UserTokensElement>(this.userTokensData);
    this._cdr.detectChanges();
  }

  errorUserTokens(err: any) {
    console.error("errorResponse: ", err);
    this._spinner.hide();
    // this.resultOfChangePassword = err.error;
    // this._cdr.detectChanges();
  }

  updateListOfTokens() {
    this._spinnerService.show();
    this._fhq.api().users_tokens({})
      .done((r: any) => this.successUserTokens(r))
      .fail((err: any) => this.errorUserTokens(err));
  }

  successUserTokensDelete(r: any) {
    // this._spinner.hide();
    console.log("successUserTokensDelete", r);
    this.updateListOfTokens();
  }

  errorUserTokensDelete(err: any) {
    console.error("errorResponse: ", err);
    this._spinner.hide();
    // this.resultOfChangePassword = err.error;
    this._cdr.detectChanges();
  }

  deleteToken(tokenId: number) {
    this._spinnerService.show();
    this._fhq.api().users_tokens_delete({
      "tokenid": tokenId,
    })
      .done((r: any) => this.successUserTokensDelete(r))
      .fail((err: any) => this.errorUserTokensDelete(err));
  }

  openDialogSignIn() {
    const modalRef = this._modalService.open(ModalDialogSignInComponent);
    modalRef.componentInstance.name = 'SignIn';
  }
}
