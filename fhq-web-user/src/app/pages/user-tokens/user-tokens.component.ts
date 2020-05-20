import { Component, OnInit, ChangeDetectorRef, ViewChild, ElementRef } from '@angular/core';
import { FhqService } from '../../services/fhq.service';
import { FreeHackQuestClient } from '../../services/libfhqcli-web-js.service';
import { SpinnerService } from '../../services/spinner.service';
import { Router } from '@angular/router';
import { NgbModal } from '@ng-bootstrap/ng-bootstrap';
import { ModalDialogSignInComponent } from '../../dialogs/modal-dialog-sign-in/modal-dialog-sign-in.component';

@Component({
  selector: 'app-user-tokens',
  templateUrl: './user-tokens.component.html',
  styleUrls: ['./user-tokens.component.css']
})
export class UserTokensComponent implements OnInit {
  errorMessage: string = null;
  subscription: any;
  userId: number = 0;
  listOfTokens: any = []
  constructor(
    private _spinner: SpinnerService,
    private _router: Router,
    private _modalService: NgbModal,
    private _fhq: FhqService,
    private _fhq2: FreeHackQuestClient,
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
    this._fhq2.connectToServer({"baseUrl": "ws://localhost:1234/"});

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
    console.log("successResponse: ", r);
    this._spinner.hide();
    this.listOfTokens = r.data
    // this._router.navigate(['/user-tokens'])
  }

  errorUserTokens(err: any) {
    console.error("errorResponse: ", err);
    this._spinner.hide();
    // this.resultOfChangePassword = err.error;
    // this._cdr.detectChanges();
  }

  updateListOfTokens() {
    this._spinnerService.show();
    this._fhq.api().users_tokens({
      "userid": this.userId,
    })
      .done((r: any) => this.successUserTokens(r))
      .fail((err: any) => this.errorUserTokens(err));
  }
}
