import { Component, OnInit, ChangeDetectorRef, ViewChild, ElementRef } from '@angular/core';
import { FhqService } from '../../services/fhq.service';
import { SpinnerService } from '../../services/spinner.service';
import { Router } from '@angular/router';
import { NgbModal } from '@ng-bootstrap/ng-bootstrap';
import { ModalDialogSignInComponent } from '../../dialogs/modal-dialog-sign-in/modal-dialog-sign-in.component';

@Component({
  selector: 'app-user-change-password',
  templateUrl: './user-change-password.component.html',
  styleUrls: ['./user-change-password.component.css']
})
export class UserChangePasswordComponent implements OnInit {
  errorMessage: string = null;
  subscription: any;
  userId: number = 0;

  @ViewChild('userOldPassword', { static: false }) userOldPassword : ElementRef;
  @ViewChild('userNewPassword', { static: false }) userNewPassword : ElementRef;
  resultOfChangePassword: string = null;

  constructor(
    private _spinner: SpinnerService,
    private _cdr: ChangeDetectorRef,
    private _fhq: FhqService,
    private _router: Router,
    private _modalService: NgbModal,
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
      this._cdr.detectChanges();
    } else {
      this.userId = 0;
    }
  }

  doChangePassword() {
    this.resultOfChangePassword = null;
    this._cdr.detectChanges();

    const userOldPasswordValue = this.userOldPassword.nativeElement.value;
    const userNewPasswordValue = this.userNewPassword.nativeElement.value;

    this._spinner.show();
    this._fhq.api().user_change_password({
      "password_old": userOldPasswordValue,
      "password_new": userNewPasswordValue,
    })
      .done((r: any) => this.successChangedPassword(r))
      .fail((err: any) => this.errorChangedPassword(err));
  }

  successChangedPassword(r: any) {
    // console.log("successResponse: ", r);
    this._spinner.hide();
    this.userOldPassword.nativeElement.value = "";
    this.userNewPassword.nativeElement.value = "";
    this._router.navigate(['/user-profile'])
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
