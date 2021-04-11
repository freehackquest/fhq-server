import { Component, Inject, OnInit, ChangeDetectorRef, NgZone, ViewChild, ElementRef } from '@angular/core';
import { NgbActiveModal } from '@ng-bootstrap/ng-bootstrap';
import { ActivatedRoute } from '@angular/router';
import { Router } from '@angular/router';
import { SpinnerService } from '../../services/spinner.service';
import { EmailValidatorService } from '../../services/email-validator.service';
import { MatDialog, MatDialogRef, MAT_DIALOG_DATA } from '@angular/material/dialog';

declare var fhq: any;

export interface DialogData {
  email: string;
  password: string;
}

@Component({
  selector: 'app-modal-signin-dialog',
  templateUrl: './modal-signin-dialog.component.html',
  styleUrls: ['./modal-signin-dialog.component.css']
})
export class ModalSignInDialog implements OnInit {
  errorMessage: string = null;
  name = '';
  @ViewChild('signinEmail', { static: true }) signinEmail : ElementRef;
  @ViewChild('signinPassword', { static: true }) signinPassword : ElementRef;

  email: string;
  password: string;
  width = "450px";
  constructor(
    public _activeModal: NgbActiveModal,
    private _route: ActivatedRoute,
    private _router: Router,
    private _cdr: ChangeDetectorRef,
    private _zone: NgZone,
    private _spinnerService: SpinnerService,
    private _emailValidator: EmailValidatorService,
    public dialog: MatDialog,
    public dialogRef: MatDialogRef<ModalSignInDialog>,
    @Inject(MAT_DIALOG_DATA) public data: DialogData,
  ) { }

  ngOnInit() {
    setTimeout(()=>{ // this will make the execution after the above boolean has changed
      this.signinEmail.nativeElement.focus();
    },1000);
  }

  onCloseClick(): void {
    this.dialogRef.close();
  }

  onSignInClick(): void {
    console.log("SignIn");
    this._spinnerService.show();
    this.errorMessage = null;
    fhq.login({
      "email": this.data.email,
      "password": this.data.password,
    })
      .done((r: any) => this.successResponse(r))
      .fail((err: any) => this.errorResponse(err));
  }
  
  openResetPasswordPage() {
    this._activeModal.dismiss('Reset password');
    this._zone.run(() => this._router.navigate(['/reset-password'])).then();
    this.dialogRef.close();
  }

  openResgistrationPage() {
    this._activeModal.dismiss('Registration');
    this._zone.run(() => this._router.navigate(['/registration'])).then();
    this.dialogRef.close();
  }

  successResponse(r: any) {
    console.log("successResponse: ", r);
    this._cdr.detectChanges();
    this._spinnerService.hide();
    this._activeModal.dismiss('SignIn Success');
    this.dialogRef.close();

    // location.reload();
  }

  errorResponse(err: any) {
    console.error("errorResponse: ", err);
    this._spinnerService.hide();
    this.errorMessage = err.error;
    this._cdr.detectChanges();
  }

  onKeydownEmail(event: any) {
    if (event.key === "Enter") {
      this.signinPassword.nativeElement.focus();
    }
  }

  onKeydownPassword(event: any) {
    if (event.key === "Enter") {
      this.onSignInClick()
    }
  }
}
