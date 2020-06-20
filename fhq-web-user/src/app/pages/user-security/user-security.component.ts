import { Component, OnInit, ChangeDetectorRef, ViewChild, ElementRef } from '@angular/core';
import { FhqService } from '../../services/fhq.service';
import { SpinnerService } from '../../services/spinner.service';
import { Router } from '@angular/router';
import { NgbModal } from '@ng-bootstrap/ng-bootstrap';
import { ModalDialogSignInComponent } from '../../dialogs/modal-dialog-sign-in/modal-dialog-sign-in.component';
import { FormControl, Validators } from '@angular/forms';

@Component({
  selector: 'app-user-security',
  templateUrl: './user-security.component.html',
  styleUrls: ['./user-security.component.css']
})
export class UserSecurityComponent implements OnInit {
  errorMessage: string = null;
  subscription: any;
  userId: number = -1;
  userLastIp: string = "";
  userCountry: string = "";
  userRegion: string = "";
  userCity: string = "";

  resultOfChangePassword: string = null;

  oldPasswordFormControl = new FormControl('', [
    Validators.required
  ]);

  newPasswordFormControl = new FormControl('', [
    Validators.required
  ]);

  constructor(
    private _spinner: SpinnerService,
    private _router: Router,
    private _modalService: NgbModal,
    private _fhq: FhqService,
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
    if (this._fhq.isAuthorized) {
      this.userId = parseInt(this._fhq.userdata.id, 10);
      this.userLastIp = this._fhq.userdata.last_ip;
      this.userCountry = this._fhq.userdata.country;
      this.userRegion = this._fhq.userdata.region;
      this.userCity = this._fhq.userdata.city;
      this._cdr.detectChanges();
    } else {
      this.userId = 0;
      this._cdr.detectChanges();
    }
    this._spinnerService.hide();
  }

  doChangePassword() {
    this.resultOfChangePassword = null;
    this._cdr.detectChanges();
    this._spinner.show();
    this._fhq.api().user_change_password({
      "password_old": this.oldPasswordFormControl.value,
      "password_new": this.newPasswordFormControl.value,
    })
      .done((r: any) => this.successChangedPassword(r))
      .fail((err: any) => this.errorChangedPassword(err));
  }

  successChangedPassword(r: any) {
    // console.log("successResponse: ", r);
    this._spinner.hide();
    this.oldPasswordFormControl.setValue("");
    this.newPasswordFormControl.setValue("");
    this._router.navigate(['/user-security'])
  }

  errorChangedPassword(err: any) {
    console.error("errorResponse: ", err);
    this._spinner.hide();
    this.resultOfChangePassword = err.error;
    this._cdr.detectChanges();
  }

  openDialogSignIn() {
    const modalRef = this._modalService.open(ModalDialogSignInComponent);
    modalRef.componentInstance.name = 'SignIn';
  }
}
